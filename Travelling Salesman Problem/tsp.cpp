#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

auto seed = std::chrono::system_clock::now().time_since_epoch().count();
mt19937 g(seed);
uniform_int_distribution<> dis(1, 249);

int GlobalEye = 0;

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
    GlobalEye++;
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

vector<int> rouletteWheelSelection(vector<Individual> pool) {
    double totalFitness = 0;
    for(Individual x : pool){
        totalFitness += x.fitness;
    }

    vector<double> selectionProbabilities(pool.size());
    for (int i = 0; i < pool.size(); i++) {
        selectionProbabilities[i] = pool[i].fitness / totalFitness;
    }

    vector<int> selectedIndices(250);
    for (int i = 0; i < 250; i++) {
        double random_num = static_cast<double>(rand()) / RAND_MAX;
        double sum = 0.0;
        int j = 0;
        while (j < pool.size() && sum < random_num) {
            sum += selectionProbabilities[j];
            j++;
        }
        selectedIndices[i] = j - 1;
    }

    return selectedIndices;
}


string orderBasedCrossover (string parent1, string parent2) {
    string subString = parent1.substr(10, 20);
    string returnStr = "";
    
    for(int i = 0; i < parent2.length()-1; i++){
        if(subString.find(parent2[i]) == -1){
            returnStr += parent2[i];
        }    
    }
    return returnStr+=subString+='1';
}


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
        
        /*
        for(Individual x : pool){
            cout << x.gnome << " : " << x.fitness << endl;
        }
        */
    }

    void FindSolution() {

        
        //vector<int> selectionIndieces (250);
       
        
        
        int rand1;
        
        for(int ab = 0; ab < 1000; ab++){
        vector<int> selectionIndieces (250);
        vector<Individual> newPool;
        
        //newPool.reserve(500);
        
        selectionIndieces = rouletteWheelSelection(pool);


        for (int index : selectionIndieces){
            Individual tempi;
            rand1 = dis(g);
            tempi.gnome = orderBasedCrossover(pool[2].gnome, pool[1].gnome);
            tempi.fitness = fitnessFunc(tempi.gnome);
            newPool.push_back(tempi);
        }

        
        
        for(int a = 0; a < selectionIndieces.size(); a++){
            newPool.push_back(pool[selectionIndieces[a]]);
        }

        pool = newPool;
        newPool.clear();
        
        }

        
        for(Individual x : pool){
            cout << x.gnome << " : " << x.fitness << endl;
        }

        cout << "\nasdasdasd" << pool.size() << endl;

        //cout << "\n" << pool[t[0]].gnome << "\n" << pool[t[1]].gnome << '\n';
        //cout << orderBasedCrossover(pool[t[0]].gnome, pool[t[1]].gnome) << endl;
        
    }

};

int main() {
    ifstream inputFile("Assignment 3 berlin52.tsp");
    
    if (inputFile) {
        int i;
        float x, y;
        while (inputFile >> i >> x >> y) {
            cityList[i] = CityData(i, x, y);
            //cout << "ID:" << i << ", X = " << x << ", Y = " << y << '\n';
        }
    } else {
        cout << "Input files could't be opened." << endl;
        return 0;
    }
    string gnome = "";
    gnome += intToChar(cityList[1].id);

    
    for(auto& it: cityList){
        if(it.first != 1){
            gnome += intToChar(it.second.id);  
        } 
        //cout << it.first << " : " << intToChar(it.second.id) << '\n';
    }

    gnome += intToChar(cityList[1].id);

    Individual test = Individual(gnome);
    test.fitness = fitnessFunc(test.gnome);

    GeneticAlgorithm GA = GeneticAlgorithm(250000, 500, test.gnome);
    GA.FindSolution();


    return 0;
}