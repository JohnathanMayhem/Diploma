#include "PartitionLib.h"

// Главная функция для поиска минимального цикла
//int findMinLengthCycle(const vector<vector<int>>& matrix,  vector<int>* vertices) {
//    vector<int> sorted_vertices = vector<int>(*vertices);
//    sort(sorted_vertices.begin(), sorted_vertices.end());
//
//    int min_weight = INT_MAX;
//    vector<int> best_cycle;
//
//    do {
//        int current_weight = 0;
//        int n = sorted_vertices.size();
//        for (int i = 0; i < n; ++i) {
//            int from = sorted_vertices[i];
//            int to = sorted_vertices[(i + 1) % n];
//            current_weight += matrix[from][to];
//        }
//
//        if (current_weight < min_weight) {
//            min_weight = current_weight;
//            best_cycle = sorted_vertices;
//        }
//    } while (next_permutation(sorted_vertices.begin(), sorted_vertices.end()));
//
//    *vertices = best_cycle;
//    return min_weight;
//}
//
//unordered_set<int> findOptimalPath(const vector<vector<int>>& edges, const int cost[], int Qmax) {
//    int n = edges.size() - 1;  // Количество пунктов (без базы)
//    vector<vector<State>> dp = vector<vector<State>>(n + 1, vector<State>(Qmax + 1));
//    for (int i = 0; i < n + 1; ++i) {
//        for (int j = 0; j < Qmax + 1; ++ j) {
//            dp[i][j] = State();
//        }
//    }
//
//    for (int j = 1; j < Qmax+1; ++j) {
//        for (int i = 1; i < n; ++i){
//            if (j < cost[i]) {
//                dp[i][j] = dp[i-1][j-1];
//            } else {
//                State s = State(dp[i-1][j-1]);
//                s.visited.insert(i);
//                s.coins += cost[i];
//                s.route.insert(s.route.end(), s.visited.begin(), s.visited.end());
//                s.distance = findMinLengthCycle(edges, &s.route);
//                s.value = s.coins / s.distance;
//                dp[i][j] = (s.value > dp[i-1][j-cost[i]].value) ? s : dp[i-1][j];
//            }
//        }
//    }
//    return dp[n-1][n+1].visited;
//}