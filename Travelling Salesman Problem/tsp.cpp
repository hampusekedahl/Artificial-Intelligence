#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

char intToChar(int value) {
    return (char)(value+48);
}

int charToInt(char value) {
    return (int)(value) - 48;
}

float calcCost(int x1, int x2, int y1, int y2){
    return sqrtf(powf(x2-x1, 2.0)+powf(y2-y1, 2.0));
}

string shuffleGnome(string gnome){
    string middle = gnome.substr(1, gnome.length() - 2);
    random_device rd;
    mt19937 g(rd());
    shuffle(middle.begin(), middle.end(), g);
    return gnome.substr(0, 1) + middle + gnome.substr(gnome.length() - 1, 1);
}

class CityData{
public:
    int id;
    float x, y;

    CityData() {}
    CityData(int I, float X, float Y): id(I), x(X), y(Y) {}
};

unordered_map<int ,CityData> cityList;

int fitnessFunc (string gnome){
    float totalCost = 0;
    for(int i = 0; i < gnome.length() - 1; ++i){ 
        totalCost += calcCost(cityList[charToInt(gnome[i])].x, cityList[charToInt(gnome[i+1])].x, cityList[charToInt(gnome[i])].x, cityList[charToInt(gnome[i+1])].x);
    }
    return totalCost;
}

class Individual {
public:
    string gnome;
    float fitness;

    Individual () {}
    Individual (string g) : gnome(g) {}
};

class GeneticAlgorithm {
public:
    vector<Individual> pool;
    int maxFitnessUses;
    int poolSize;
    Individual goldenGoose;


    GeneticAlgorithm(int max, int size, string startGnome) : maxFitnessUses(max), poolSize(size) {
        Individual temp;
        for (int i = 0; i < poolSize; i++) {
            temp.gnome = shuffleGnome(startGnome);
            temp.fitness = fitnessFunc(temp.gnome);
            pool.push_back(temp);
        }
        for(Individual x : pool){
            cout << x.gnome << " : " << x.fitness << endl;
        }
    }

    void FindSolution() {
        
    }

};

int main() {
    ifstream inputFile("Assignment 3 berlin52.tsp");
    
    if (inputFile) {
        int i;
        float x, y;
        while (inputFile >> i >> x >> y) {
            cityList[i] = CityData(i, x, y);
            cout << "ID:" << i << ", X = " << x << ", Y = " << y << '\n';
        }
    } else {
        cout << "Input files could't be opened." << endl;
        return 0;
    }
    string gnome = "";
    gnome += intToChar(cityList[1].id);

    for(auto& it: cityList){
        gnome += intToChar(it.second.id);
        cout << it.first << " : " << intToChar(it.second.id) << '\n';
    }

    Individual test = Individual(gnome);
    cout << test.gnome << endl;

    test.fitness = fitnessFunc(test.gnome);

    cout << test.fitness << endl;

    test.gnome = shuffleGnome(test.gnome);
    cout << test.gnome << endl;
    test.fitness = fitnessFunc(test.gnome);
    cout << test.fitness << endl;

    GeneticAlgorithm GA = GeneticAlgorithm(250000, 50, test.gnome);


    return 0;
}