import os
from dotenv import load_dotenv
from pathlib import Path

# Obtener la ruta del directorio raíz del proyecto
# __file__ es la ruta a este archivo (juego/config.py)
# .parent es la carpeta que lo contiene (juego/)
# .parent.parent es la carpeta raíz (OS-PR.../)
PROJECT_ROOT = Path(__file__).parent.parent

# 2. Construir la ruta completa al archivo .env
ENV_PATH = PROJECT_ROOT / ".env"

# 3. Cargar el .env desde esa ruta específica
print(f"Cargando variables de entorno desde: {ENV_PATH}")
load_dotenv(dotenv_path=ENV_PATH)
# ---- Configuración del Servidor ----
# Usa un valor por defecto si no se encuentra la variable
HOST = os.environ.get('SERVER_HOST', '127.0.0.1')
PORT = int(os.environ.get('SERVER_PORT', 9999))

# ---- Reglas del Juego ----
# Convertimos a entero (int) ya que las variables de entorno siempre son texto

DIE_SIDES_R = int(os.environ.get('GAME_DIE_SIDES_R', 6))
PLAYER_MAX_HP = int(os.environ.get('PLAYER_MAX_HP', 100))
MIN_TEAMS = int(os.environ.get('GAME_MIN_TEAMS', 2))
MAX_TEAMS = int(os.environ.get('GAME_MAX_TEAMS', 4))
MIN_PLAYERS_PER_TEAM = int(os.environ.get('GAME_MIN_PLAYERS_PER_TEAM', 2))
MAX_PLAYERS_PER_TEAM = int(os.environ.get('GAME_MAX_PLAYERS_PER_TEAM', 5))