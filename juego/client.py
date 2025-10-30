import asyncio
import config  # el config.py
import os
import sys

# El estado inicial debe leer del módulo config para estar sincronizado
game_state = {
    "players": {}, 
    "last_message": "¡Conectado! Esperando jugadores..."
}

def parse_message(msg):
    """
    Analiza los mensajes del servidor y actualiza nuestro game_state local.
    """
    global game_state
    
    if not msg.strip():
        return

    # Siempre guarda el último mensaje, sea cual sea
    game_state["last_message"] = msg 

    # NUEVOS JUGADORES
    # Esto sigue siendo necesario para AÑADIR jugadores al diccionario
    if "Jugador" in msg and "se unió al" in msg:
        try:
            parts = msg.split()
            name = parts[2]
            team = parts[5].strip('.')
            
            # Solo añade si no existe
            if name not in game_state["players"]:
                game_state["players"][name] = {
                    "hp": config.PLAYER_MAX_HP, 
                    "team": team, 
                    "is_defending": False 
                }
        except (IndexError, ValueError):
            pass # Error de parseo, ignorar

    elif msg.strip().startswith(">"):
        try:
            parts = msg.strip().split()
            name = parts[1] # El nombre es la segunda "palabra"

            # Si el jugador está en nuestro diccionario...
            if name in game_state["players"]:
                
                # Actualizar HP
                if "[DERROTADO]" in msg:
                    game_state["players"][name]["hp"] = 0
                else:
                    # Buscar la parte "HP:XX]"
                    hp_part = [p for p in parts if p.startswith("HP:")][0]
                    hp = int(hp_part.split(':')[1].strip(']'))
                    game_state["players"][name]["hp"] = hp
                
                # Actualizar Estado de Defensa
                if "[DEFENDIENDO]" in msg:
                    game_state["players"][name]["is_defending"] = True
                else:
                    game_state["players"][name]["is_defending"] = False
        
        except (IndexError, ValueError, TypeError):
            pass # Error de parseo, esta línea no era un estado
            
    # LÓGICA DE TURNO (YA NO MANEJA DEFENSA)
    elif "TURNO: Es el turno de" in msg:
        # Ya no necesitamos hacer nada aquí,
        # solo dejar que el mensaje se muestre
        pass


    # Refrescar la pantalla con la nueva información
    draw_screen()

def draw_screen():
    """
    Esta función borra la consola y dibuja el juego completo.
    """
    os.system('cls' if os.name == 'nt' else 'clear')

    print("--- ARENA DE COMBATE POR EQUIPOS ---")

    # Dibujar el estado de los jugadores (HP)
    print("\nEstado de Jugadores:")
    for name, data in game_state["players"].items():
        
        # Determinar el estado de HP
        status_hp = f"HP: {data['hp']}"
        if data['hp'] <= 0:
            status_hp = "DERROTADO"
        
        # Determinar estado de defensa (usamos .get() por si la clave aún no existe)
        status_def = ""
        if data.get("is_defending", False): 
            status_def = " [DEFENDIENDO]" # Añade el texto extra
        
        # Imprimir la línea completa
        print(f"  > {name} ({data['team']}) [{status_hp}]{status_def}")

    # Mostrar el último mensaje del servidor
    print("\nÚltimo Evento:")
    print(f"  {game_state['last_message']}")

    # Mostrar el prompt de input
    print("\n> ", end="", flush=True)

async def tcp_echo_client():
    try:
        reader, writer = await asyncio.open_connection(config.HOST, config.PORT)
    except ConnectionRefusedError:
        print(f"Error: No se pudo conectar al servidor en {config.HOST}:{config.PORT}.")
        print("Asegúrate de que 'server.py' esté ejecutándose.")
        return

    # Dibujar pantalla inicial
    draw_screen() 

    loop = asyncio.get_event_loop()
    
    # Tarea para recibir mensajes del servidor
    async def loop_escucha(reader):
        try:
            while True:
                data = await reader.read(4096)
                if not data:
                    print("\nDesconectado del servidor.")
                    break

                # Manejar múltiples mensajes a la vez
                # El servidor puede enviar "msg1\nmsg2" en un solo paquete
                messages = data.decode().strip().split('\n')
                for msg in messages:
                    parse_message(msg.strip())
                
        except asyncio.CancelledError:
            pass
        except Exception as e:
            print(f"\nError en loop_escucha: {e}")
        finally:
            print("Cerrando conexión.")
            writer.close()

    # Tarea para leer el input del usuario y enviarlo
    async def loop_envio(writer):
        try:
            while True:
                message = await loop.run_in_executor(None, input)
                
                if not writer.is_closing():
                    writer.write(message.encode())
                    await writer.drain()
                else:
                    break
                    
                if message.lower() == 'salir':
                    break
        except asyncio.CancelledError:
            pass
        except Exception as e:
            print(f"\nError en loop_envio: {e}")
        finally:
            if not writer.is_closing():
                writer.close()

    # Ejecutar ambas tareas concurrentemente
    listen_task = asyncio.create_task(loop_escucha(reader))
    send_task = asyncio.create_task(loop_envio(writer))

    await asyncio.wait([listen_task, send_task], return_when=asyncio.FIRST_COMPLETED)

    listen_task.cancel()
    send_task.cancel()
    
    if not writer.is_closing():
        writer.close()
        await writer.wait_closed()
    
    print("Saliendo del juego.")

if __name__ == "__main__":
    asyncio.run(tcp_echo_client())