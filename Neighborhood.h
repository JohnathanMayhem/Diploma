#include "PartitionLib.h"
#include <random>

struct Move {
    int sourceIdx;
    int targetIdx;
    int client;
    int insertPos;
    int cost; //стоимость вставки пути

    Move(int s, int t, int c, int p, int co) : sourceIdx(s), targetIdx(t), client(c), insertPos(p), cost(co) {}
};

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dist(0.0, 1.0);

vector<vector<int>> N_q_move(const vector<vector<int>> &edges, const vector<State> &routes, const vector<int> &demand, double q) {
    vector<Move> possibleMoves;

    for (int srcIdx = 0; srcIdx < routes.size(); ++srcIdx) {
        State &srcRoute = routes[srcIdx];
        for (size_t i = 0; i < srcRoute.route.size(); ++i) {
            int client = srcRoute.route[i];
            if (client == 0 || dist(gen) > q) continue;

            for (int tgtIdx = 0; tgtIdx < routes.size(); ++tgtIdx) {
                if (tgtIdx == srcIdx || routes[tgtIdx].visited.count(client)) continue;
                State &tgtRoute = routes[tgtIdx];

                int minCost = INT_MAX;
                int bestPos = -1;
                vector<int> &path = tgtRoute.route;
                for (int j = 0; j < path.size() - 1; ++j) {
                    int a = path[j];
                    int b = path[j + 1];
                    int cost = matrix[a][client] + matrix[client][b] - matrix[a][b];
                    if (cost < minCost) {
                        minCost = cost;
                        bestPos = j;
                    }
                }

                if (bestPos != -1) {
                    possibleMoves.emplace_back(srcIdx, tgtIdx, client, bestPos, minCost);
                }
            }
        }
    }

    if (possibleMoves.empty()) return routes;

    auto bestMove = min_element(possibleMoves.begin(), possibleMoves.end(),
                                [](const Move &a, const Move &b) { return a.cost < b.cost; });

    int srcIdx = bestMove->sourceIdx;
    int tgtIdx = bestMove->targetIdx;
    int client = bestMove->client;
    int insertPos = bestMove->insertPos;

    State &srcRoute = routes[srcIdx];
    State &tgtRoute = routes[tgtIdx];

    vector<State> newRoutes = vector<State>(routes.size());

    for (int i = 0; i < routes.size(); ++i) {
        if (i == srcIdx) {

        }
        if (i == tgtIdx) {

        }
        newRoutes[i] = State(routes[i]);
    }
    // Удаление клиента из исходного маршрута
    auto &srcPath = srcRoute.route;
    auto it = find(srcPath.begin(), srcPath.end(), client);
    if (it != srcPath.end()) {
        srcPath.erase(it);
        srcRoute.distance = 0;
        for (size_t i = 0; i < srcPath.size() - 1; ++i) {
            srcRoute.distance += matrix[srcPath[i]][srcPath[i + 1]];
        }
        srcRoute.fuel -= demand[client];
        srcRoute.visited.erase(client);
    }

    // Вставка клиента в целевой маршрут
    auto &tgtPath = tgtRoute.route;
    tgtPath.insert(tgtPath.begin() + insertPos + 1, client);
    tgtRoute.distance = 0;
    for (size_t i = 0; i < tgtPath.size() - 1; ++i) {
        tgtRoute.distance += matrix[tgtPath[i]][tgtPath[i + 1]];
    }
    tgtRoute.fuel += demand[client];
    tgtRoute.visited.insert(client);

    return routes;
}

Move findBestMove(const State                  &src,
                  const State                  &trg,
                  const vector<int>         &demand,
                  const vector<vector<int>> &matrix,
                  int                        srcInd,
                  int                        trgInd,
                  int                          Qmax)
{
    Move bestMove{srcInd, trgInd, -1, -1, INT_MAX};
    int cost;

    int penalty = (src.fuel - Qmax) / (trg.fuel - Qmax);
    int bestPenalty = penalty;

    for (int i = 1; i < src.route.size() - 1; ++i) {
        if (trg.visited.count(stc.route[i])) continue;
        for (int j = 1; j < trg.route.size() - 1; ++j) {
            cost = matrix[i][trg.route[j]] + matrix[i][trg.route[j - 1]] - matrix[j][j - 1] - matrix[i][i - 1] -
                   matrix[i][i + 1] + matrix[i - 1][i + 1];
            if (bestMove.cost > cost && (src.fuel - demand[i] - Qmax) / (trg.fuel + demand[i] - Qmax) >= penalty) {
                bestMove.cost = cost;
                bestMove.client = i;
                bestMove.insertPos = j;
            }
        }
    }
    return bestMove;
}

vector<vector<int>> N_move_strict(const vector<vector<int>> &edges, const vector<State> &routes, const vector<int> &demand, int Qmax) {
    vector<Move> possibleMoves;
    for (int srcInd = 0; srcInd < routes.size(); ++srcInd) {
        if (routes[srcInd].fuel < Qmax) continue;
        for (int trgInd = 0; trgInd < routes.size(); ++trgInd) {
            if (trgInd == srcInd || routes[trgInd].fuel > Qmax) continue;
            Move m = findBestMove(routes[srcInd], routes[trgInd], demand, edges, srcInd, trgInd, Qmax);
            if (m.cost != INT_MAX) {
                possibleMoves.push_back(m);
            }
        }
    }
}

