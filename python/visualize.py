import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.path import Path
import matplotlib.patches as patches
import numpy as np

def read_data(file_path):
    with open(file_path, 'r') as f:
        lines = [line.strip() for line in f.readlines() if line.strip()]
    
    # Trouver les sections
    cities_start = lines.index("Cities:") + 1
    route_start = lines.index("Route:") + 1
    distance_line = lines[-1]
    
    # Lire les villes
    cities = []
    for line in lines[cities_start:route_start-1]:
        x, y = map(float, line.split())
        cities.append((x, y))
    
    # Lire la route
    route = list(map(int, lines[route_start].split()))
    
    # Lire la distance
    distance = float(distance_line.split(": ")[1])
    
    return np.array(cities), route, distance

def plot_tsp(cities, route, distance):
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # Affiche les villes
    ax.scatter(cities[:, 0], cities[:, 1], c='red', s=50, zorder=2)
    for i, (x, y) in enumerate(cities):
        ax.text(x, y, str(i), fontsize=8, ha='center', va='center')
    
    # Crée le chemin
    route_coords = cities[route + [route[0]]]  # Retour à la première ville
    path = Path(route_coords)
    
    # Dessine le chemin
    patch = patches.PathPatch(path, facecolor='none', lw=2, edgecolor='blue', zorder=1)
    ax.add_patch(patch)
    
    # Paramètres du graphique
    ax.set_title(f"Problème du voyageur de commerce (TSP)\nDistance totale: {distance:.2f}")
    ax.set_xlabel("Coordonnée X")
    ax.set_ylabel("Coordonnée Y")
    ax.grid(True)
    
    plt.tight_layout()
    plt.show()

def main():
    input_file = "../data/output.txt"
    cities, route, distance = read_data(input_file)
    plot_tsp(cities, route, distance)

if __name__ == "__main__":
    main()