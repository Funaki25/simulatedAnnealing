#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <random>
#include <string>
#include <direct.h>
#include <iomanip>
using namespace std;

struct City {
    int x, y;
};

void ensureDirectoryExists(const string& path) {
    #ifdef _WIN32
        _mkdir(path.c_str());
    #else
        mkdir(path.c_str(), 0777);
    #endif
}
// Générateur de nombres aléatoires modernes
int randomInt(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

double randomDouble() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

void generateCities(int numCities, const string& filePath) {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Erreur: Impossible d'ouvrir " << filePath << endl;
        return;
    }
    
    file << numCities << endl;
    for (int i = 0; i < numCities; ++i) {
        file << randomInt(0, 100) << " " << randomInt(0, 100) << endl;
    }
    file.close();
}

vector<City> readCities(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Erreur: Impossible d'ouvrir " + filePath);
    }

    int numCities;
    file >> numCities;
    vector<City> cities(numCities);
    for (int i = 0; i < numCities; ++i) {
        file >> cities[i].x >> cities[i].y;
    }
    return cities;
}

double calculateDistance(const City& a, const City& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double totalDistance(const vector<int>& route, const vector<City>& cities) {
    double dist = 0.0;
    for (size_t i = 0; i < route.size(); ++i) {
        int j = (i + 1) % route.size();
        dist += calculateDistance(cities[route[i]], cities[route[j]]);
    }
    return dist;
}

vector<int> generateNeighbor(const vector<int>& route) {
    vector<int> newRoute = route;
    int i = randomInt(0, route.size() - 1);
    int j = randomInt(0, route.size() - 1);
    swap(newRoute[i], newRoute[j]);
    return newRoute;
}

void printCurrentRoute(const vector<int>& route, const vector<City>& cities, int iteration, double temperature) {
    cout << "Itération " << iteration << " (Température: " << fixed << setprecision(2) << temperature << "):\n";
    cout << "Chemin: ";
    for (size_t i = 0; i < route.size(); ++i) {
        cout << route[i];
        if (i != route.size() - 1) {
            cout << " -> ";
        }
    }
    cout << "\nDistance: " << fixed << setprecision(2) << totalDistance(route, cities) << "\n\n";
}

vector<int> simulatedAnnealing(const vector<City>& cities, 
    double initialTemp, 
    double coolingRate, 
    double minTemp, 
    int maxIterations) {
vector<int> currentRoute(cities.size());
iota(currentRoute.begin(), currentRoute.end(), 0);
shuffle(currentRoute.begin(), currentRoute.end(), mt19937(random_device{}()));

double currentDist = totalDistance(currentRoute, cities);
vector<int> bestRoute = currentRoute;
double bestDist = currentDist;

double temp = initialTemp;

cout << "=== Début de l'algorithme ===\n";
printCurrentRoute(currentRoute, cities, 0, initialTemp);

for (int i = 0; i < maxIterations && temp > minTemp; ++i) {
vector<int> newRoute = generateNeighbor(currentRoute);
double newDist = totalDistance(newRoute, cities);
double delta = newDist - currentDist;

if (delta < 0 || exp(-delta / temp) > randomDouble()) {
currentRoute = newRoute;
currentDist = newDist;

if (currentDist < bestDist) {
bestRoute = currentRoute;
bestDist = currentDist;

cout << "Nouvelle meilleure solution trouvée:\n";
printCurrentRoute(bestRoute, cities, i, temp);
}
}

if (i % 100 == 0) {
printCurrentRoute(currentRoute, cities, i, temp);
}

temp *= coolingRate;
}

cout << "=== Fin de l'algorithme ===\n";
cout << "Meilleure distance trouvée: " << bestDist << "\n";
return bestRoute;
}


void saveResult(const vector<City>& cities, const vector<int>& route, const string& filePath) {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Erreur: Impossible d'écrire dans " << filePath << endl;
        return;
    }

    // Écriture des villes (doit correspondre exactement au Python)
    file << "Cities:\n";
    for (const auto& city : cities) {
        file << city.x << " " << city.y << "\n";
    }
    
    // Ligne vide avant Route:
    file << "\n";
    
    // Écriture de la route (attention à l'espace après \n)
    file << "Route:\n";
    for (int idx : route) {
        file << idx << " ";
    }
    
    // Ligne vide avant Distance
    file << "\n\nDistance: " << totalDistance(route, cities) << endl;
    file.close();
}

int main() {
    const string dataDir = "../data";
    const string inputFile = dataDir + "/input.txt";
    const string outputFile = dataDir + "/output.txt";
    const int numCities = 50;

     // Crée le répertoire data s'il n'existe pas
     ensureDirectoryExists(dataDir);
    // Génère les villes aléatoires
    generateCities(numCities, inputFile);

    // Lit les villes depuis le fichier
    vector<City> cities = readCities(inputFile);

    // Paramètres du recuit simulé
    double initialTemp = 10000.0;
    double coolingRate = 0.995;
    double minTemp = 1.0;
    int maxIterations = 10000;

    // Exécute l'algorithme
    vector<int> bestRoute = simulatedAnnealing(cities, initialTemp, coolingRate, minTemp, maxIterations);

    cout << "\n=== RESULTAT FINAL ===\n";
    printCurrentRoute(bestRoute, cities, maxIterations, 0);
    // Sauvegarde les résultats
    saveResult(cities, bestRoute, outputFile);

    cout << "Resultats sauvegardez dans " << outputFile << endl;
    cout << "Executez le script Python pour visualiser les resultats." << endl;

    return 0;
}