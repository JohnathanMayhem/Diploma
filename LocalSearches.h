#include "Neighborhood.h"
vector<vector<int>> localSearchDeterministic(
    vector<vector<int>>(*neighborhood)(const vector<vector<int>> &edges, const vector<DayStates> &dayStated, const vector<int> &demand, int Qmax),
    vector<vector<int>>& routes,
    const vector<State> &routes,
    const vector<int> &demand,
    int Qmax, unsigned long long maxIterations
    ) {
    unsigned long long iterations = 0;
    while (iterations < maxIterations) {

    }
}
