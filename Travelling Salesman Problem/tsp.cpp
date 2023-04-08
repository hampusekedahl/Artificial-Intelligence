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

int rand_num(int start, int end)
{
	int r = end - start;
	int rnum = start + rand() % r;
	return rnum;
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
    float totalCost = 0;
    for(int i = 0; i < gnome.length() - 1; ++i){ 
        totalCost += calcCost(cityList[charToInt(gnome[i])].x, cityList[charToInt(gnome[i+1])].x, cityList[charToInt(gnome[i])].x, cityList[charToInt(gnome[i+1])].x);
    }
    GlobalEye++;
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
    string subString = parent1.substr(10, 13);
    string returnStr = "";
    
    for(int i = 0; i < parent2.length()-1; i++){
        if(subString.find(parent2[i]) == -1){
            returnStr += parent2[i];
        }    
    }
    return returnStr+=subString+='1';
}

string mutatedGene(string gnome)
{
	while (true) {
		int r = rand_num(1, 52);
		int r1 = rand_num(1, 52);
		if (r1 != r) {
			char temp = gnome[r];
			gnome[r] = gnome[r1];
			gnome[r1] = temp;
			break;
		}
	}
	return gnome;
}

bool lessthan(Individual t1,Individual t2)
{
	return t1.fitness < t2.fitness;
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
    }

    void FindSolution() {

        
        while (GlobalEye <= maxFitnessUses) {
            sort(pool.begin(), pool.end(), lessthan);
            vector<Individual> newPool;

            for (int i = 0; i < pool.size(); i++) {
                Individual p1 = pool[i];
                while (true) {
                    Individual new_gnome;

                    //cross
                    if(i <= 350) {
                        while (true) {
                            int r = rand_num(0, 500);
                            if (i != r) {
                                new_gnome.gnome = orderBasedCrossover(p1.gnome, pool[r].gnome);
                                break;
                            }
                        }
                        
                        new_gnome.fitness = fitnessFunc(new_gnome.gnome);
                        if (new_gnome.fitness <= pool[i].fitness) {
                            newPool.push_back(new_gnome);
                            break;
                        }
                    }
                    
                    //mutation
                    new_gnome.gnome = mutatedGene(p1.gnome);

                    new_gnome.fitness = fitnessFunc(new_gnome.gnome);
                    if (new_gnome.fitness <= pool[i].fitness) {
					newPool.push_back(new_gnome);
					break;
				    }
                }
            }
            pool = newPool;
        }

        int onlyPrint = 0;
        for(Individual x : pool){
            if (onlyPrint == 10){
                break;
            }
            cout << x.gnome << " : " << x.fitness << endl;
            onlyPrint++;
        }
    }

};

int main() {
    ifstream inputFile("Assignment 3 berlin52.tsp");
    
    if (inputFile) {
        int i;
        float x, y;
        while (inputFile >> i >> x >> y) {
            cityList[i] = CityData(i, x, y);
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
    }

    gnome += intToChar(cityList[1].id);

    Individual start = Individual(gnome);
    start.fitness = fitnessFunc(start.gnome);

    GeneticAlgorithm GA = GeneticAlgorithm(250000, 500, start.gnome);
    GA.FindSolution();

    return 0;
}