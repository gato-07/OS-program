import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from dotenv import load_dotenv
import os
import numpy as np


def plot_damage_per_player(df, output_dir, match_id):
    """Gráfico A: Barras de daño total por jugador."""
    print(f"Generando gráfico de daño por jugador para la partida {match_id}...")
    df_sorted = df.sort_values(by='damage_dealt', ascending=False)
    
    plt.figure(figsize=(12, 7))
    # Usamos seaborn para un look más moderno y la paleta 'viridis'
    ax = sns.barplot(x='player_name', y='damage_dealt', hue='team_name', data=df_sorted, dodge=False, palette='viridis')
    
    # Añadir el valor exacto del daño sobre cada barra
    for container in ax.containers:
        ax.bar_label(container, fmt='%d', fontsize=10, color='black', weight='bold')
        
    plt.title(f"Daño Total por Jugador - Partida: {match_id}")
    plt.xlabel("Jugador")
    plt.ylabel("Daño Infligido")
    plt.xticks(rotation=45, ha="right")
    ax.legend(title="Equipos")
    plt.tight_layout()

    output_path = os.path.join(output_dir, f"match_{match_id}_damage_per_player.png")
    plt.savefig(output_path)
    plt.close()
    print(f"Gráfico guardado en: {output_path}")


def plot_actions_per_player(df, output_dir, match_id):
    """Gráfico B: Barras agrupadas de acciones (ataques vs defensas)."""
    print(f"Generando gráfico de acciones por jugador para la partida {match_id}...")
    player_names = df['player_name']
    attacks = df['attacks_made']
    defenses = df['defenses_made']

    x = np.arange(len(player_names))  # las localizaciones de las etiquetas
    width = 0.35  # el ancho de las barras

    fig, ax = plt.subplots(figsize=(12, 7))
    
    # Usamos una paleta de colores de seaborn
    colors = sns.color_palette("pastel")
    rects1 = ax.bar(x - width/2, attacks, width, label='Ataques', color=colors[0])
    rects2 = ax.bar(x + width/2, defenses, width, label='Defensas', color=colors[1])

    ax.set_ylabel('Cantidad de Acciones')
    ax.set_title(f'Acciones por Jugador - Partida: {match_id}')
    ax.set_xticks(x)
    ax.set_xticklabels(player_names, rotation=45, ha="right")
    ax.legend()
    ax.bar_label(rects1, padding=3)
    ax.bar_label(rects2, padding=3)

    fig.tight_layout()

    output_path = os.path.join(output_dir, f"match_{match_id}_actions_per_player.png")
    plt.savefig(output_path)
    plt.close()
    print(f"Gráfico guardado en: {output_path}")


def plot_team_damage_contribution(df, output_dir, match_id):
    """Gráfico C: Torta de contribución de daño por equipo."""
    print(f"Generando gráfico de contribución de daño para la partida {match_id}...")
    team_damage = df.groupby('team_name')['damage_dealt'].sum()

    # Evitar gráfico de torta si no hay daño
    if team_damage.sum() == 0:
        print("No se generó gráfico de contribución porque el daño total fue 0.")
        return

    # "Explotar" la porción del equipo ganador o con más daño
    explode = [0] * len(team_damage)
    if not team_damage.empty:
        max_damage_idx = team_damage.argmax()
        explode[max_damage_idx] = 0.1 # Separa la porción más grande

    colors = sns.color_palette("Set2")

    plt.figure(figsize=(8, 8))
    plt.pie(team_damage, labels=team_damage.index, autopct='%1.1f%%', startangle=140, 
            colors=colors, wedgeprops={'edgecolor': 'white'}, shadow=True, explode=explode)
    plt.title(f'Contribución de Daño por Equipo - Partida: {match_id}', weight='bold')
    plt.axis('equal')  # Asegura que la torta sea un círculo.

    output_path = os.path.join(output_dir, f"match_{match_id}_team_damage.png")
    plt.savefig(output_path)
    plt.close()
    print(f"Gráfico guardado en: {output_path}")


def main():
    # Aplicar un tema de seaborn a todos los gráficos para un look más profesional
    sns.set_theme(style="whitegrid")

    load_dotenv()
    # La ruta por defecto ahora apunta a 'juego/gameplots'
    output_dir = os.getenv("PLOT_OUTPUT_DIR", os.path.join(os.path.dirname(__file__), "gameplots"))
    log_dir = os.path.join(os.path.dirname(__file__), "gamelogs")

    # 1. Pedir ID de partida al usuario
    match_id = input("Ingresa el ID de la partida (ej: 20251130-221530): ").strip()
    if not match_id:
        print("Error: El ID de la partida no puede estar vacío.")
        return

    # 2. Buscar y validar el archivo CSV
    match_filename = f"match_{match_id}.csv"
    match_filepath = os.path.join(log_dir, match_filename)

    if not os.path.exists(match_filepath):
        print(f"Error: No se encontró el archivo de la partida '{match_filename}' en la carpeta '{log_dir}'.")
        return

    print(f"Archivo de partida encontrado: {match_filepath}")
    df = pd.read_csv(match_filepath)

    # 3. Asegurarse de que el directorio de salida exista
    os.makedirs(output_dir, exist_ok=True)

    # 4. Generar y guardar los gráficos
    plot_damage_per_player(df, output_dir, match_id)
    plot_actions_per_player(df, output_dir, match_id)
    plot_team_damage_contribution(df, output_dir, match_id)

if __name__ == "__main__":
    main()