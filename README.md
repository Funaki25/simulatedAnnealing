# 🚗 Problème du Voyageur de Commerce - Recuit Simulé

**Optimisation en C++ avec visualisation Python**

## 📁 Structure du projet
TSP_Project/
├── cpp/
│ └── tsp_solver.cpp # Algorithme principal
├── data/
│ ├── input.txt # Villes générées
│ └── output.txt # Solution optimale
├── python/
│ ├── visualize.py # Visualisation
│

## 🛠️ Installation
```bash
git clone https://github.com/votre-utilisateur/TSP_Project.git
cd TSP_Project/python
1. Compiler le C++ :
cd ../cpp
g++ -std=c++11 -o tsp_solver tsp_solver.cpp
2. Lancer l'optimisation :
./tsp_solver
3. Visualiser :
cd ../python
python visualize.py
