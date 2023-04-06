#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

#define MAX_WEIGHT 420

class BagNode {
public:
    int depth, weight, benefit;
    vector<bool> itemsInBag;
    BagNode() { depth = 0; weight = 0; benefit = 0; }
    BagNode(int depth, int weight, int benefit, vector<bool>& itemsInBag) : depth(depth), weight(weight), benefit(benefit), itemsInBag(itemsInBag) {}
};

class BreadthFirstSearch {
public:
    BagNode solution;

    BreadthFirstSearch() {}

    void FindSolution(vector<pair<int, int> > items) {
        queue<BagNode> queue;
        queue.push(BagNode());
        BagNode current, optimal;

        while (!queue.empty()) {
            current = queue.front(); queue.pop();

            if (current.benefit > optimal.benefit) {
                optimal = current;
            }

            if (current.depth < items.size()) {
                vector<bool> temp = current.itemsInBag; temp.push_back(true);
                BagNode add = BagNode(current.depth + 1, current.weight + items[current.depth].first, current.benefit + items[current.depth].second, temp);

                if (add.weight <= MAX_WEIGHT)
                    queue.push(add);

                temp = current.itemsInBag; temp.push_back(false);
                BagNode empty = BagNode(current.depth + 1, current.weight, current.benefit, temp);

                queue.push(empty);
            }
        }
        solution = optimal;
    }
};

class DepthFirstSearch {
public:
    BagNode solution;

    DepthFirstSearch() {}

    void FindSolution(vector<pair<int, int> > items) {
        stack<BagNode> stack;
        stack.push(BagNode());
        BagNode current, optimal;

        while (!stack.empty()) {
            BagNode current = stack.top(); stack.pop();

            if (current.benefit > optimal.benefit) {
                optimal = current;
            }

            if (current.depth < items.size()) {
                vector<bool> temp = current.itemsInBag; temp.push_back(true);
                BagNode add = BagNode(current.depth + 1, current.weight + items[current.depth].first, current.benefit + items[current.depth].second, temp);

                if (add.weight <= MAX_WEIGHT)
                    stack.push(add);

                temp = current.itemsInBag; temp.push_back(false);
                BagNode empty = BagNode(current.depth + 1, current.weight, current.benefit, temp);
                stack.push(empty);
            }
        }
        solution = optimal;
    }
};

int main() {
    ifstream inputFile("knapsack_data.txt");

    vector<pair<int, int> > itemVector;
    
    if (inputFile) {
        int i, b, w;
        while (inputFile >> i >> b >> w) {
            itemVector.push_back(make_pair(w, b));
            cout << "ID : " << i << ", Benefit = " << b << ", Weight = " << w << endl;
        }
        inputFile.close();
    } else {
        cout << "Input file could't be opened." << endl;
        return 0;
    }

    BreadthFirstSearch bfs = BreadthFirstSearch();
    bfs.FindSolution(itemVector);
    cout << "BFS :> Weight: " << bfs.solution.weight << ", Benefit: " << bfs.solution.benefit << '\n' << endl;

    DepthFirstSearch dfs = DepthFirstSearch();
    dfs.FindSolution(itemVector);
    cout << "DFS :> Weight: " << dfs.solution.weight << ", Benefit: " << dfs.solution.benefit << '\n' << endl;

    return 0;
}