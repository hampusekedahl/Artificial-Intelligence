#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
using namespace std;

class CityData{
public:
    string cityName;
    int straightLineDistance;
    vector<pair<string, int> > cityConnections;

    CityData() {}
    CityData(string name, int straightDistance) : cityName(name), straightLineDistance(straightDistance) {}
};

class Node {
public:
    Node* parentNode;
    string cityName;
    int depth, cost;

    Node() {}
    Node(Node* parent, string name, int d, int c) {
        parentNode = parent;
        cityName = name;
        depth = d;
        cost = c;
    }
};

bool operator<(const Node& left, const Node& right) {
    return left.cost > right.cost;
}

class GreedyBestFirstSearch {
public:
    GreedyBestFirstSearch() {}

    Node FindSolution(unordered_map<string, CityData> umap) {
        priority_queue<Node> openList;
        unordered_map<string, int> closedList;
        Node* root = new Node(NULL, "Malaga", 0, umap["Malaga"].straightLineDistance);
        openList.push(*root);

        while (!openList.empty()) {
            Node* current = new Node(openList.top().parentNode, openList.top().cityName, openList.top().depth, openList.top().cost);
            openList.pop();

            if (current->cityName == "Valladolid") {
                return *current;
            }

            for (pair<string, int> city : umap[current->cityName].cityConnections) {
                if (closedList.count(city.first) == 0) {
                    if (current->parentNode == NULL) {
                        openList.push(*new Node(current, city.first, current->depth+1, umap[city.first].straightLineDistance));
                        closedList[city.first] = 0;
                    }
                    else if (city.first != current->parentNode->cityName) {
                        openList.push(*new Node(current, city.first, current->depth+1, umap[city.first].straightLineDistance));
                        closedList[city.first] = 0; 
                    }
                }
            }
        }
        return Node();
    }

};

void printListReverse(Node* node){
    if(node == nullptr){
        return;
    }
    printListReverse(node->parentNode);
    cout << "City = " << node->cityName << '\n';
}

int main() {
    unordered_map<string, CityData> umap;

    ifstream inputFile1("straight_distance_data.txt"), inputFile2("connection_data.txt");

    if (inputFile1 && inputFile2) {
        string cityName;
        int straightLineDistance;
        while (inputFile1 >> cityName >> straightLineDistance) {
            umap[cityName] = CityData(cityName, straightLineDistance);
            cout << "City name : " << cityName << ", Straight line distance to Valladolid = " << straightLineDistance << endl;
        }
        inputFile1.close();

        string cityNameA, cityNameB;
        int distanceBetween;
        while (inputFile2 >> cityNameA >> cityNameB >> distanceBetween) {
            umap[cityNameA].cityConnections.push_back(make_pair(cityNameB, distanceBetween));
            umap[cityNameB].cityConnections.push_back(make_pair(cityNameA, distanceBetween));
            cout << "City A = " << cityNameA << ", City B = " << cityNameB << ", Distance between = " << distanceBetween << endl;
        }
        inputFile2.close();
    } else {
        cout << "Input files could't be opened." << endl;
        return 0;
    }

    GreedyBestFirstSearch GBFS = GreedyBestFirstSearch();
    Node soultion = GBFS.FindSolution(umap);
    cout << "\n\nStart to goal\n";
    printListReverse(&soultion);

    return 0;
}