#include "PartitionLib.h"
/**
* @brief greedy solution with filling the assignment for each day for each tanker maximising the
* fullness of tanker and minimising the distance using DP
*
* This function use DP aglorithm to find
*
* @param windows sorted by t1 array contains pair of customer and their window.
* @param adjacence matrix of adjacency that contains time to travel between i-th and j-th
* @param f_i - array of times that are needed to service i-th customer
*/

vector<vector<int>> generateTriangleInequalityMatrix(int size) {
    // Генерируем случайные координаты для точек
    vector<pair<double, double>> points = vector<pair<double, double>>(size);
    for (int i = 0; i < size; ++i) {
        points[i] = {rand() % 100, rand() % 100}; // Координаты от 0 до 99
    }

    // Создаём матрицу смежности
    vector<vector<int>> matrix(size, vector<int>(size, 0));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j) continue;
            double dx = points[i].first - points[j].first;
            double dy = points[i].second - points[j].second;
            matrix[i][j] = static_cast<int>(sqrt(dx*dx + dy*dy) + 0.5); // Округление
        }
    }
    return matrix;
}

// Пример матрицы 20x20 (зафиксируем seed для воспроизводимости)


int main() {
//    srand(123); // Фиксируем генератор случайных чисел
//    vector<vector<int>> edges = generateTriangleInequalityMatrix(10);
//
//    int cost[] = {0, 2, 3, 7, 10, 4, 5, 3, 2, 4};  // Монеты в каждом пункте (0 - база)
    vector<vector<int>> edges = {{0, 4, 5, 10},
                                 {4, 0, 5, 7},
                                 {5, 5, 0, 3},
                                 {10, 7, 3, 0}};
    vector<int> cost = {0, 2, 3, 3};
    int Qmax = 10;               // Максимальная вместимость

//    vector<int> path = findOptimalPath(edges, cost, Qmax);
//
//    cout << "Path: ";
//    for (int node : path) {
//        cout << node << " ";
//    }
//    cout << endl;
    map<int, int> freq;
    freq[1] = 2;
    freq[2] = 2;
    freq[3] = 3;
    vector<vector<int>> routes = assignVehicles(edges, 1,freq, 10, cost);
    for (auto& i: routes){
        for (auto j: i){
            cout<<j<<" ";
        }
        cout<<"\n";
    }

    return 0;
}




