import asyncio
import config  # Importa la configuración de .env
from game_logic import GameState, Player  # Importa las CLASES del cerebro

# El estado global del juego. Solo hay UNA instancia.
GAME = GameState()

async def broadcast(message):
    
    # Imprime en la consola del servidor para depuración
    print(f"[BROADCAST]: {message}")
    
    # Evitar "dictionary changed size during iteration"
    all_writers = list(GAME.players_by_writer.keys())
    
    for writer in all_writers:
        try:
            # Añade un salto de línea para que el cliente lo lea
            writer.write(f"{message}\n".encode())
            await writer.drain()
        except (ConnectionError, BrokenPipeError):
            # El cliente se desconectó, se limpiará en handle_client
            print("Error de broadcast: Cliente desconectado.")
            pass

async def broadcast_turn_info():
    """Función auxiliar para anunciar de quién es el turno."""
    if GAME.game_started and not GAME.check_win_condition():
        player = GAME.get_current_player()
        await broadcast(f"TURNO: Es el turno de {player.name} (Equipo {player.team.name}).")
        
        # Enviar el prompt "susurrado" solo al jugador actual
        try:
            prompt = "¡Es tu turno! (COMANDOS: atacar <nombre>, defender, estado (mostrar estado de la partida)): \n"
            player.writer.write(prompt.encode())
            await player.writer.drain()
        except (ConnectionError, BrokenPipeError):
            pass # Se manejará en el bucle principal

async def handle_client(reader, writer):
    """
    Esta función se ejecuta para CADA cliente que se conecta.
    Gestiona el login, el bucle del juego y la desconexión.
    """
    addr = writer.get_extra_info('peername')
    print(f"Nueva conexión desde {addr}")
    
    new_player = None  # Variable para guardar el jugador y limpiarlo al final

    # FASE DE LOGIN Y REGISTRO ---
    try:
        # Pedir nombre
        writer.write("Bienvenido! Ingresa tu nombre: \n".encode())
        await writer.drain()
        data = await reader.read(100)
        name = data.decode().strip()

        # Pedir equipo
        writer.write(f"Hola {name}! Ingresa el nombre de tu equipo: \n".encode())
        await writer.drain()
        data = await reader.read(100)
        team_name = data.decode().strip()

        # CREAR Y AÑADIR JUGADOR AL JUEGO
        new_player = Player(name, writer)
        success, message = GAME.add_player(new_player, team_name)
        
        if not success:
            writer.write(f"Error: {message}\nCerrando conexión.\n".encode())
            await writer.drain()
            return # Termina esta función handle_client

        # Anunciar la llegada del nuevo jugador
        await broadcast(f"INFO: {message}")
        await broadcast(GAME.get_status_summary())

        # REVISAR SI EL JUEGO COMIENZA ---
        if not GAME.game_started and GAME.check_start_condition():
            GAME.game_started = True
            await broadcast("\n" + "="*30)
            await broadcast("¡¡¡COMIENZA EL JUEGO!!!")
            await broadcast(f"Orden de turnos: {[p.name for p in GAME.turn_order]}")
            await broadcast(GAME.get_status_summary())
            await broadcast_turn_info()
        elif GAME.game_started:
             writer.write("INFO: El juego ya ha comenzado. Entrando como espectador (WIP) o reconectando...\n".encode())
             # Por ahora, unirse a un juego empezado es complejo
             # Lo más simple es que solo puedan unirse antes de que empiece.
             writer.write(f"INFO: Te has unido al juego en curso.\n".encode())
             await writer.drain()


        #BUCLE PRINCIPAL DEL JUEGO (importante)
        while True:
            # El bucle espera aquí hasta recibir datos
            data = await reader.read(4096) 
            
            if not data:
                # El cliente cerró la conexión (ej. cerró la ventana)
                raise ConnectionError("Cliente cerró la conexión limpiamente")
            
            command_str = data.decode().strip()
            if not command_str:
                continue # Ignorar envíos vacíos

            # Procesar el comando
            if not GAME.game_started:
                writer.write("INFO: El juego aún no ha comenzado. Espera...\n".encode())
                await writer.drain()
                continue
                
            # MANEJO DE TURNOS
            if GAME.get_current_player() == new_player:
                
                # Es el turno de este jugador, procesar su acción
                status, message = GAME.process_action(new_player, command_str)

                # INTERPRETAR RESPUESTA DE GAME_LOGIC (CRÍTICO)
                
                if status == "Error" or status == "Espera":
                    # FUE UN ERROR. Enviar mensaje SÓLO al jugador actual.
                    writer.write(f"ERROR: {message}\n".encode())
                    await writer.drain()
                    # No se avanza el turno, puede intentarlo de nuevo.
                
                elif status == "Info":
                    # FUE UNA PETICIÓN DE INFO. Enviar SÓLO al jugador actual.
                    writer.write(f"{message}\n".encode())
                    await writer.drain()

                elif status == "Accion":
                    # si es valida, se tira mensaje por broadcast
                    await broadcast(f"ACCION: {message}")
                    await broadcast(GAME.get_status_summary()) # Enviar estado actualizado
                    # (process_action ya llamó a next_turn(), así que anunciamos el siguiente)
                    await broadcast_turn_info()
                
                elif status == "Ganador":
                    # termino del juego
                    await broadcast("\n" + "="*30)
                    await broadcast(f"¡JUEGO TERMINADO! El ganador es el EQUIPO {message}")
                    await broadcast("="*30 + "\n")
                    await broadcast(GAME.get_status_summary())
                    
                    GAME.game_started = False # Detener el juego
                    # aca se pueden agregar lógicas de reinicio si se desea
                    break # Rompe el bucle while True para este cliente

            else:
                writer.write("ESPERA: No es tu turno.\n".encode())
                await writer.drain()

    except (ConnectionError, asyncio.IncompleteReadError, BrokenPipeError) as e:
        # manejar desconexiones limpias y errores de conexión
        print(f"Desconexión de {addr}: {e}")
        if new_player:
            print(f"Jugador {new_player.name} se ha desconectado.")
            # Marcar como derrotado
            new_player.is_defeated = True
            new_player.hp = 0
            
            # Limpiar del estado del juego
            if new_player.writer in GAME.players_by_writer:
                del GAME.players_by_writer[new_player.writer]
            if new_player.name.lower() in GAME.players_by_name:
                del GAME.players_by_name[new_player.name.lower()]

            # Anunciar la desconexión
            await broadcast(f"INFO: ¡{new_player.name} se ha desconectado del juego!")
            
            # Comprobar si la desconexión da la victoria a alguien
            if GAME.game_started:
                winner_team = GAME.check_win_condition()
                if winner_team:
                    GAME.game_started = False # Detener el juego
                    await broadcast(f"¡{new_player.name} se desconectó!")
                    await broadcast(f"¡El último equipo en pie es el EQUIPO {winner_team.name}!")
                    await broadcast(f"¡JUEGO TERMINADO!")
                else:
                    # Si el juego sigue, anunciar el estado y el siguiente turno
                    await broadcast(GAME.get_status_summary())
                    # Si era el turno del jugador desconectado, avanzar
                    if GAME.get_current_player() == new_player:
                        GAME.next_turn()
                    await broadcast_turn_info()

    finally:
        # salir y limpiar todo
        print(f"Cerrando conexión final con {addr}")
        if writer and not writer.is_closing():
            writer.close()
            await writer.wait_closed()


async def main():
    # iniciar el server
    server = await asyncio.start_server(
        handle_client, config.HOST, config.PORT
    )

    addrs = ', '.join(str(sock.getsockname()) for sock in server.sockets)
    print(f"Servidor iniciado en {addrs}")
    print(f"Reglas: Tablero=HP={config.PLAYER_MAX_HP}")

    async with server:
        await server.serve_forever()

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nServidor detenido manualmente.")