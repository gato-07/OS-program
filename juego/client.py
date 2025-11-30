import asyncio
import config  # el config.py
import os
import collections

# --- Estado Global del Cliente ---
# Usamos un diccionario para mantener todo el estado que necesitamos para dibujar la UI.
# Esto incluye el estado del juego recibido del servidor y el input actual del usuario.
CLIENT_STATE = {
    "players": {},  # { "nombre": {"hp": 100, "team": "A", ...} }
    "event_log": collections.deque(["Conectando..."], maxlen=3), # Guarda los últimos 3 eventos
    "user_input": "",  # Lo que el usuario está escribiendo actualmente
    "is_connected": False,
    "game_over": False,
}


def parse_message(msg):
    """
    Analiza un mensaje del servidor y actualiza el CLIENT_STATE.
    NO dibuja la pantalla, solo actualiza los datos.
    """
    global CLIENT_STATE

    if not msg.strip():
        return

    # Guardar casi todos los mensajes como el "último evento"
    # Excluimos los prompts de turno para que no se queden fijos en la UI.
    if "¡Es tu turno!" not in msg:
        CLIENT_STATE["event_log"].append(msg)

    # --- Lógica de Parseo Mejorada ---

    # Detectar fin del juego para detener el input
    if "¡JUEGO TERMINADO!" in msg:
        CLIENT_STATE["game_over"] = True

    # Parsear línea de estado de un jugador
    if msg.strip().startswith(">"):
        try:
            parts = msg.strip().split()
            name = parts[1]
            
            if name not in CLIENT_STATE["players"]:
                CLIENT_STATE["players"][name] = {
                    "hp": config.PLAYER_MAX_HP,
                    "team": CLIENT_STATE.get("_current_team", "???"),
                    "is_defending": False,
                }

            player_data = CLIENT_STATE["players"][name]
            player_data["team"] = CLIENT_STATE.get("_current_team", player_data["team"])

            # Cada vez que recibimos una actualización de estado de un jugador,
            # asumimos que ya no se está defendiendo del turno anterior.
            player_data["is_defending"] = False

            # Buscar información de HP si está presente
            try:
                # Buscar el índice de la parte que contiene "HP:"
                hp_index = -1
                for i, part in enumerate(parts):
                    if "HP:" in part:
                        hp_index = i
                        break
                
                if hp_index != -1:
                    # El número puede estar en la misma parte ("HP:85") o en la siguiente ("[HP:", "85/100]")
                    full_hp_str = "".join(parts[hp_index:]) # Unimos todo desde "HP:" hasta el final
                    hp_value_str = full_hp_str.split('/')[0].replace("HP:", "").strip("[]")
                    player_data["hp"] = int(hp_value_str)
            except (ValueError, IndexError):
                pass # Si el parseo falla, no rompemos el cliente
            
            # El estado de derrota y defensa se extrae del mensaje actual
            player_data["hp"] = 0 if "[DERROTADO]" in msg else player_data["hp"]
            player_data["is_defending"] = "[DEFENDIENDO]" in msg and player_data["hp"] > 0
        except (IndexError, ValueError):
            pass
    
    elif "Jugador" in msg and "se unió al" in msg:
        try:
            parts = msg.split()
            name = parts[parts.index("Jugador") + 1]
            team = parts[parts.index("al") + 1].strip(".")
            
            if name not in CLIENT_STATE["players"]:
                CLIENT_STATE["players"][name] = {
                    "hp": config.PLAYER_MAX_HP,
                    "team": team,
                    "is_defending": False,
                }
            else:
                CLIENT_STATE["players"][name]["team"] = team
        except (IndexError, ValueError):
            pass
    
    elif msg.strip().startswith("Equipo "):
        try:
            team_name = msg.strip().replace("Equipo ", "").replace(":", "").strip()
            CLIENT_STATE["_current_team"] = team_name
        except:
            pass


def draw_screen():
    """
    Borra la consola y dibuja la UI completa basada en el CLIENT_STATE actual.
    Esta es la única función que debe imprimir en la consola.
    """
    os.system("cls" if os.name == "nt" else "clear")
    print("--- ARENA DE COMBATE POR EQUIPOS ---")

    # Agrupar jugadores por equipo para una mejor visualización
    teams = {}
    for name, data in CLIENT_STATE["players"].items():
        team_name = data.get("team", "Desconocido")
        if team_name not in teams:
            teams[team_name] = []
        teams[team_name].append((name, data))

    for team_name, players in sorted(teams.items()):
        print(f"\nEquipo {team_name}:")
        for name, data in players:
            status_hp = "DERROTADO" if data['hp'] <= 0 else f"HP: {data['hp']}"
            status_def = " [DEFENDIENDO]" if data.get("is_defending", False) else ""
            print(f"  > {name.ljust(15)} [{status_hp}]{status_def}")

    print("\n" + "="*40)
    print("ÚLTIMOS EVENTOS:")
    for event in CLIENT_STATE["event_log"]:
        print(f"- {event}")
    print("="*40 + "\n")

    # Mostrar el prompt de input con el texto que el usuario ya ha escrito
    if CLIENT_STATE["game_over"]:
        print("\nJUEGO TERMINADO. Presiona Enter para salir.")
    elif not CLIENT_STATE["is_connected"]:
        print("\nConectando al servidor...")
    else:
        print(f"COMANDO > {CLIENT_STATE['user_input']}", end="", flush=True)


async def main_client():
    try:
        reader, writer = await asyncio.open_connection(config.HOST, config.PORT)
        CLIENT_STATE["is_connected"] = True
    except ConnectionRefusedError:
        CLIENT_STATE["event_log"].append(f"Error: No se pudo conectar a {config.HOST}:{config.PORT}. ¿El servidor está activo?")
        draw_screen()
        return

    loop = asyncio.get_event_loop()
    input_queue = asyncio.Queue()

    def on_input():
        """Función que se ejecuta en un hilo para no bloquear asyncio."""
        while CLIENT_STATE["is_connected"] and not CLIENT_STATE["game_over"]:
            try:
                # Lee un solo caracter (esto es dependiente del SO)
                # Esta es una simplificación. Una librería como `blessed` o `prompt_toolkit` lo haría mejor.
                char = sys.stdin.read(1)
                asyncio.run_coroutine_threadsafe(input_queue.put(char), loop)
            except:
                break

    # Tarea 1: Escuchar mensajes del servidor
    async def listen_to_server(reader):
        try:
            while True:
                data = await reader.read(4096)
                if not data:
                    CLIENT_STATE["event_log"].append("Desconectado del servidor.")
                    CLIENT_STATE["is_connected"] = False
                    break
                messages = data.decode().strip().split("\n")
                for msg in messages:
                    parse_message(msg.strip())
        except (asyncio.CancelledError, ConnectionResetError):
            CLIENT_STATE["is_connected"] = False
        finally:
            CLIENT_STATE["is_connected"] = False
    
    # Tarea 2: Bucle principal de UI y manejo de input
    async def handle_ui_and_input(writer):
        try:
            while CLIENT_STATE["is_connected"]:
                draw_screen()
                try:
                    # Esperar por un caracter del usuario o un timeout para redibujar
                    char = await asyncio.wait_for(input_queue.get(), timeout=0.1)
                    
                    if char == '\n': # Enter
                        command = CLIENT_STATE["user_input"]
                        CLIENT_STATE["user_input"] = ""
                        if command:
                            writer.write(f"{command}\n".encode())
                            await writer.drain()
                        if command.lower() == "salir" or CLIENT_STATE["game_over"]:
                            break
                    elif char in ('\x7f', '\b'): # Backspace
                        CLIENT_STATE["user_input"] = CLIENT_STATE["user_input"][:-1]
                    else: # Cualquier otro caracter
                        CLIENT_STATE["user_input"] += char
                except asyncio.TimeoutError:
                    continue # El timeout nos permite redibujar la pantalla
        except asyncio.CancelledError:
            pass
        finally:
            CLIENT_STATE["is_connected"] = False

    # --- Configuración y ejecución ---
    # Preparar la terminal para leer caracter por caracter
    import sys, tty, termios
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setcbreak(sys.stdin.fileno())
        # Iniciar el hilo para la entrada de usuario
        input_thread = loop.run_in_executor(None, on_input)

        # Crear y ejecutar las tareas de asyncio
        listen_task = asyncio.create_task(listen_to_server(reader))
        ui_task = asyncio.create_task(handle_ui_and_input(writer))
        
        await asyncio.wait([listen_task, ui_task], return_when=asyncio.FIRST_COMPLETED)
    finally:
        # Restaurar la configuración de la terminal es MUY importante
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        if not writer.is_closing():
            writer.close()
            await writer.wait_closed()
        os.system("clear")
        print("Saliste del juego.")



if __name__ == "__main__":
    try:
        asyncio.run(main_client())
    except KeyboardInterrupt:
        print("\nJuego interrumpido.")
