#include <vector>
#include <climits>
#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <limits>
#include <cmath>
#include <map>
#include <set>
using namespace std;

struct State {
    int fuel;         // Количество топлива
    int distance;      // Пройденное расстояние
    double value;      // Отношение монет к расстоянию (стоимость)
    set<int> visited;
    vector<int> route;

    bool operator<(const State& other) const {
        // Сравниваем по value (чем больше, тем лучше)
        if (value != other.value) {
            return value < other.value;
        }
        // Если value одинаковы, выбираем путь с меньшим расстоянием
        return distance > other.distance;
    }

    State(int c, int d, double val, set<int>& v, vector<int>& r) : fuel(c), distance(d), value(val), visited(v), route(r.begin(), r.end()){}

    State(const State& s) {
        fuel = s.fuel;
        distance = s.distance;
        value = s.value;
        visited = set<int>(s.visited);
        route = vector<int>(s.route.begin(), s.route.end());
    }

    State() {
        fuel = 0;
        distance = 0;
        value = 0.0;
        visited = set<int>();
        visited.insert(0);
        route = vector<int>();
        route.push_back(0);
    }

    ~State(){
        delete &visited;
        delete &route;
    }
};

int findMinLengthCycle(const vector<vector<int>>& matrix,  vector<int>* vertices) {
    vector<int> sorted_vertices = vector<int>(*vertices);
    sort(sorted_vertices.begin(), sorted_vertices.end());

    int min_weight = INT_MAX;
    vector<int> best_cycle;

    do {
        int current_weight = 0;
        int n = sorted_vertices.size();
        for (int i = 0; i < n; ++i) {
            int from = sorted_vertices[i];
            int to = sorted_vertices[(i + 1) % n];
            current_weight += matrix[from][to];
        }

        if (current_weight < min_weight) {
            min_weight = current_weight;
            best_cycle = sorted_vertices;
        }
    } while (next_permutation(sorted_vertices.begin(), sorted_vertices.end()));
    *vertices = best_cycle;
    return min_weight;
}

State findOptimalPath(const vector<vector<int>>& edges, const vector<int>& demand, int Qmax, set<int>& restASU) {
    vector<int> rest(restASU.begin(), restASU.end());
    int n = rest.size()-1;
    vector<vector<State*>> dp = vector<vector<State*>>(n + 1);

    for (int i = 0; i < n + 1; ++i) {
        dp[i] = vector<State*>(Qmax + 1);

        for (int j = 0; j < Qmax + 1; ++ j) {
            dp[i][j] = new State();
        }
    }
    int i_max = 0, j_max = 0;
    for (int j = 1; j < Qmax+1; ++j) {
        for (int i = 1; i < n+1; ++i){
            if (j < demand[i]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                State* s = new State(*dp[i-1][j-demand[i]]);
                s->visited.insert(rest[i]);
                s->fuel += demand[rest[i]];
                s->route.push_back(rest[i]);
                s->distance = findMinLengthCycle(edges, &s->route);
                s->value = (float)s->fuel / pow(s->distance, 1.0 / s->route.size());
                dp[i][j] = (s->value > dp[i-1][j-demand[i]]->value) ? s : dp[i-1][j];
                if (dp[i][j]->value >= dp[i_max][j_max]->value) {
                    i_max = i;
                    j_max = j;
                }
            }
        }
    }

    for (auto i: dp[i_max][j_max]->route){
        cout<<i<<" -> ";
    }
    cout<<'\n';
    cout <<"distance:" <<dp[i_max][j_max]->distance<<'\n';
    return *dp[i_max][j_max];
}
/**
* @brief greedy solution with filling the assignment for each day for each tanker maximising the
* fullness of tanker and minimising the distance using DP
*
* This function use DP aglorithm to find routes of vehicles
*
* @param edjes matrix af adjustments
* @param vehicles amount of vehicles
* @param freq - array of amount of days that is needed to visit i ASU
* @param Qmax - number of max amount of fuel that one vehicle can transport in one route
* @param demand - for i ASU day demand (total demand / freq[i]), depo has demand 0
*/
void refreshEdjes(vector<vector<int>>& edjes,  set<int>& restASU) {
    vector<vector<int>> newEdjes = vector<vector<int>>();
    for(int i = 0; i < edjes.size(); ++i) {
        vector<int> row;

        for (int j = 0; j < edjes.size(); ++j) {
            if ((restASU.count(i) > 0 || i ==0) && (restASU.count(j) > 0 || j ==0)){
                row.push_back(edjes[i][j]);
            }
        }
        newEdjes.push_back(row);
    }
    edjes = newEdjes;
    for (auto &i: edjes){
        for (auto j: i) {
            cout<<j<<' ';
        }
        cout<<'\n';
    }
}

vector<State> assignVehicles(const vector<vector<int>>& edjes, int vehicles, map<int, int>& freq, int Qmax, vector<int>& demand) {
    set<int> restASU;
    vector<State> routes;
    vector<vector<int>> matrix;
    matrix.reserve(edjes.size());
    for (const auto & edje : edjes) {
        matrix.emplace_back(edje);
    }
    for (int i = 0; i < edjes.size(); ++i) {
        restASU.insert(i);
    }

    bool flag = false;
    while (restASU.size() != 1) {
        State route = findOptimalPath(edjes, demand, Qmax, restASU);
        for (int i = 1; i < route.route.size(); ++i) {
            if (--freq[route.route[i]] == 0) {
                restASU.erase(restASU.find(route.route[i]));
                flag = true;
            }
        }
        routes.push_back(route);
        if (flag) {
            refreshEdjes(matrix, restASU);
            flag = false;
        }
    }
    for (auto& i: routes){
        i.route.push_back(0);
    }
    return routes;
}

