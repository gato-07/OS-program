import asyncio
import config  # el config.py
import os
import sys

# El estado inicial debe leer del módulo config para estar sincronizado
game_state = {"players": {}, "last_message": "¡Conectado! Esperando jugadores..."}


def parse_message(msg, call_draw=True):
    """
    Analiza los mensajes del servidor y actualiza nuestro game_state local.
    call_draw: si False, no llama a draw_screen() (para procesar múltiples mensajes)
    """
    global game_state

    if not msg.strip():
        return

    # Solo guardar mensajes importantes en "last_message"
    # No guardar líneas del estado del tablero
    if (
        not msg.strip().startswith(">")
        and not msg.strip().startswith("---")
        and not msg.strip().startswith("Equipo")
    ):
        game_state["last_message"] = msg

    # PARSEAR ESTADO DEL TABLERO (líneas que empiezan con ">")
    # Formato: "  > NombreJugador     [HP: XX/100]" o "  > NombreJugador     [DERROTADO]"
    if msg.strip().startswith(">"):
        try:
            parts = msg.strip().split()
            if len(parts) < 2:
                return

            name = parts[1]  # El nombre del jugador

            # IMPORTANTE: Si el jugador NO está en nuestro diccionario, crearlo
            if name not in game_state["players"]:
                # Intentar obtener el equipo del contexto
                current_team = game_state.get("_current_team", "Desconocido")
                game_state["players"][name] = {
                    "hp": config.PLAYER_MAX_HP,
                    "team": current_team,
                    "is_defending": False,
                }
            else:
                # Si el jugador ya existe, actualizar su equipo si tenemos uno en contexto
                current_team = game_state.get("_current_team")
                if current_team:
                    game_state["players"][name]["team"] = current_team

            # Actualizar HP
            if "[DERROTADO]" in msg:
                game_state["players"][name]["hp"] = 0
            else:
                # Buscar la parte "HP:XX]" o "HP:XX/100]"
                for part in parts:
                    if "HP:" in part:
                        # Extraer el número, puede ser "HP:85]" o "HP:85/100]"
                        hp_str = (
                            part.replace("HP:", "").replace("[", "").replace("]", "")
                        )
                        if "/" in hp_str:
                            hp_str = hp_str.split("/")[0]
                        try:
                            hp = int(hp_str)
                            game_state["players"][name]["hp"] = hp
                        except ValueError:
                            pass
                        break

            # Actualizar Estado de Defensa
            game_state["players"][name]["is_defending"] = "[DEFENDIENDO]" in msg

        except (IndexError, ValueError, TypeError):
            pass  # Error de parseo

    # PARSEAR MENSAJES "se unió al" (para equipos)
    elif "Jugador" in msg and "se unió al" in msg:
        try:
            # Formato: "INFO: Jugador NombreJugador se unió al NombreEquipo."
            parts = msg.split()
            name_index = parts.index("Jugador") + 1
            team_index = parts.index("al") + 1

            name = parts[name_index]
            team = parts[team_index].strip(".")

            # Añadir o actualizar jugador
            if name not in game_state["players"]:
                game_state["players"][name] = {
                    "hp": config.PLAYER_MAX_HP,
                    "team": team,
                    "is_defending": False,
                }
            else:
                # Si ya existía (fue creado por el parser del tablero), actualizar el equipo
                game_state["players"][name]["team"] = team

        except (IndexError, ValueError):
            pass  # Error de parseo

    # PARSEAR LÍNEA "Equipo X:" del estado del tablero
    elif msg.strip().startswith("Equipo "):
        # Esta línea contiene información del equipo, extraer y actualizar
        try:
            # Formato: "Equipo NombreEquipo:"
            team_name = msg.strip().replace("Equipo ", "").replace(":", "").strip()
            # Guardar el equipo actual que estamos procesando para las siguientes líneas ">"
            game_state["_current_team"] = team_name
        except:
            pass

    # LÓGICA DE TURNO
    elif "TURNO: Es el turno de" in msg:
        # Resetear el estado de defensa de todos (la defensa solo dura un turno)
        for player_data in game_state["players"].values():
            player_data["is_defending"] = False

    # PARSEAR LÍNEA "--- Estado del Tablero ---"
    elif "--- Estado del Tablero ---" in msg:
        # No hacer nada, solo ignorar esta línea de encabezado
        pass

    # Refrescar la pantalla con la nueva información solo si se solicita
    if call_draw and msg.strip() and not msg.strip().startswith("---"):
        draw_screen()


def draw_screen():
    """
    Esta función borra la consola y dibuja el juego completo.
    """
    os.system("cls" if os.name == "nt" else "clear")

    print("--- ARENA DE COMBATE POR EQUIPOS ---")

    # Dibujar el estado de los jugadores (HP)
    print("\nEstado de Jugadores:")
    for name, data in game_state["players"].items():
        # Determinar el estado de HP
        status_hp = f"HP: {data['hp']}"
        if data["hp"] <= 0:
            status_hp = "DERROTADO"

        # Determinar estado de defensa (usamos .get() por si la clave aún no existe)
        status_def = ""
        if data.get("is_defending", False):
            status_def = " [DEFENDIENDO]"  # Añade el texto extra

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
                messages = data.decode().strip().split("\n")
                # Procesar todos los mensajes sin refrescar la pantalla
                for i, msg in enumerate(messages):
                    # Solo llamar draw_screen en el último mensaje
                    is_last = i == len(messages) - 1
                    parse_message(msg.strip(), call_draw=is_last)

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

                if message.lower() == "salir":
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
