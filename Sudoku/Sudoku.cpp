#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#define N 9
#define EMPTY 0

bool valid(int grid[N][N], int i, int j, int value) {
    for (int col = 0; col < N; col++) {
        if (grid[i][col] == value) {
            return false;
        }
    }

    for (int row = 0; row < N; row++) {
        if (grid[row][j] == value) {
            return false;
        }
    }

    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row + (i - i % 3)][col + (j - j % 3)] == value) {
                return false;
            }

    return true;
}

bool findEmpty(int grid[N][N], int& i, int& j) {
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (grid[i][j] == EMPTY) {
                return true;
            }
        }
    }

    return false;
}

bool Solution(int grid[N][N])
{
    int row, col;

    if (!findEmpty(grid, row, col))
        return true;

    for (int num = 1; num <= 9; num++)
    {

        if (valid(grid, row, col, num))
        {

            grid[row][col] = num;

            if (Solution(grid))
                return true;

            grid[row][col] = EMPTY;
        }
    }

    return false;
}

void printGrid(int grid[N][N])
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
            std::cout << grid[row][col] << " ";
        std::cout << std::endl;
    }
}


int main(int argc, const char* argv[]) {
    int puzzles[10][N][N];

    ifstream inputFile("sudoku_data.txt");

    int puzzle_index = 0;
    int row = 0;
    string line;
    while (getline(inputFile, line)) {
        if (line.substr(0, 6) == "SUDOKU") {
            row = 0;
            puzzle_index++;
            continue;
        }
        for (int col = 0; col < 9; col++) {
            if (line[col] == '0') {
                puzzles[puzzle_index - 1][row][col] = 0;
            }
            else {
                puzzles[puzzle_index - 1][row][col] = line[col] - '0';
            }
        }
        row++;
    }

    inputFile.close();

    for(int i = 0; i < 10; ++i){
        if (Solution(puzzles[i]) == true){
            cout << "Solution found\n";
            printGrid(puzzles[i]);
        } else {
            cout << "No solution exists";
        }
        
    }

    cout << endl;

    return 0;
}