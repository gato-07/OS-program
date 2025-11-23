import sys
import matplotlib.pyplot as plt
import os
import re

def parsear_log_tuplas(archivo):
    threads = []
    tiempos = []
    
    print(f"Leyendo archivo: {archivo}")
    with open(archivo, 'r') as f:
        contenido = f.read()
        # Buscar todas las coincidencias de (NUMERO, NUMERO)
        coincidencias = re.findall(r'\((\d+),\s*(\d+)\)', contenido)
        
        for c in coincidencias:
            t_count = int(c[0])
            time_ms = int(c[1])
            threads.append(t_count)
            tiempos.append(time_ms)
            print(f"  -> Leído: {t_count} hilos, {time_ms} ms")
            
    # Ordenar por hilos
    datos = sorted(zip(threads, tiempos))
    if not datos:
        return [], []
    return zip(*datos)

def main():
    if len(sys.argv) < 2:
        print("Uso: python3 graficador.py <archivo_log>")
        sys.exit(1)

    archivo_log = sys.argv[1]

    if not os.path.exists(archivo_log):
        print(f"Error: No se encuentra el archivo {archivo_log}")
        sys.exit(1)

    try:
        list_threads, list_tiempos = parsear_log_tuplas(archivo_log)
        
        if not list_threads:
            print("[ERROR] No se encontraron datos válidos (tuplas) en el log.")
            sys.exit(1)

        plt.figure(figsize=(10, 6))
        plt.plot(list_threads, list_tiempos, marker='o', linestyle='-', color='#1f77b4', linewidth=2)
        
        plt.title('Análisis de Rendimiento: Índice Invertido Paralelo')
        plt.xlabel('Cantidad de Threads (N_THREADS)')
        plt.ylabel('Tiempo de Ejecución (ms)')
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.xticks(list_threads) 

        for x, y in zip(list_threads, list_tiempos):
            plt.annotate(f'{int(y)}ms', (x, y), textcoords="offset points", xytext=(0,10), ha='center')

        output_file = 'grafico_rendimiento.png'
        plt.savefig(output_file)
        print(f"\n[OK] Gráfico guardado como: {output_file}")

    except Exception as e:
        print(f"Error generando el gráfico: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()