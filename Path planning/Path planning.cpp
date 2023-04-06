#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ifstream inputFile1("connection_data.txt"), inputFile2("straight_distance_data.txt");
    

    if (inputFile1 && inputFile2) {
        string i, b;
        int w;
        while (inputFile1 >> i >> b >> w) {
            cout << "City 1 : " << i << ", City 2 = " << b << ", Distance = " << w << endl;
        }
        inputFile1.close();

        string x;
        int y;
        while (inputFile2 >> x >> y) {
            cout << "City : " << x << ", Straight Line Distance = " << y << endl;
        }
        inputFile2.close();
    } else {
        cout << "Input file could't be opened." << endl;
        return 0;
    }


    return 0;
}