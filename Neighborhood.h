#include "PartitionLib.h"
#include <random>

struct Move {
    int sourceIdx; //номер машины из которой достаем клиента
    int targetIdx; //номер машины в которую добавлям клиента
    int srcDay; //номер дня из которого
    int trgDay; //номер дня в который
    int client; //номер клиента
    int insertPos; //номер клиента перед которым вставляем
    int srcFuelDecrease; //уменьшение количества топлива для машины
    int trgFuelIncrease; // увеличеие топлива для машины
    int cost; //стоимость вставки пути
};

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dist(0.0, 1.0);

vector<Move> N_q_move(const vector<vector<int>>     &edges,
                      const vector<vector<State*>> &routes,
                      const vector<int>            &demand,
                      double                             q // вероятность
                      ) {
    vector<Move> possibleMoves;
    for (int srcIndDay = 0; srcInd < routes.size(); ++srcInd) {
        for (int trgIndDay = 0; trgInd < routes.size(); ++trgInd) {
            for (int srcRouteInd = 0; srcRouteInd < routes[srcIndDay].size(); ++srcRouteInd) {
                for (int trgRouteInd = 0; trgRouteInd < routes[trgIndDay].size(); ++trgRouteInd) {
                    if (trgInd == srcInd || routes[trgInd].fuel > routes[trgInd]) continue; //только не перегруженные
                    Move m = findMoveInsertions(routes[srcInd], routes[trgInd], demand, edges, srcInd, trgInd, Qmax);
                    possibleMoves.insert(possibleMoves.end(), m.begin(), m.end());
                }
            }

        }
    }
    return possibleMoves;
}

vector<Move>* findMoveInsertions(const State                  *src,
                                const State                  *trg,
                                const vector<int>         &demand,
                                const vector<vector<int>> &matrix,
                                const DayStates&     srcDayStates,
                                const DayStates&     trgDayStates,
                                int                        srcInd,
                                int                        trgInd,
                                int                          Qmax
                                ) {
    vector<Move>* moves = new vector<Move>;

    int minCost = INT_MAX;
    int minTrgInd = -1;
    int srcVer, srcVerNext, srcVerPrev, trgVer, trgVerPrev;
    for (int i = 1; i < src->route.size() - 1; ++i) {
        if (trg.visited.count(src->route[i]) || (trgDayStates.day != srcDayStates.day && trgDayStates.visited.count(src->route[i]))) continue;
        srcVer = src->route[i];
        srcVerNext = src->route[i+1];
        srcVerPrev = src->route[i-1];

        for (int j = 1; j < trg->route.size(); ++j) {
            trgVer = trg->route[j];
            trgVerPrev = trg->route[j-1];
            cost = matrix[srcVer][trgVer] + matrix[srcVer][trgVerPrev] - matrix[srcVerPrev][srcVerNext];
            if (cost < minCost){
                minCost = cost;
                minTrgInd = j;
            }
        }
        Move m;

        m.cost = cost;
        m.insertPos = minTrgInd;
        m.srcDay = srcDayStates.day;
        m.trgDay = trgDayStates.day;
        m.client = srcVer;
        m.sourceIdx = srcInd;
        m.targetIdx = trgVer;
        m.srcFuelDecrease = m.trgFuelIncrease = demand[srcVer];
        if (m.cost < 0 || (Qmax - src->fuel + m.trgFuelIncrease > 0) ) {
            moves->push_back(m);
        }
    }
    return moves;

}

vector<Move> N_move_strict(const vector<vector<int>>   &edges,
                           const vector<DayStates> &dayRoutes,
                           const vector<int>          &demand,
                           int                           Qmax
                           ) {
    vector<Move> possibleMoves;
    for (int srcIndDay = 0; srcInd < dayRoutes.size(); ++srcInd) {
        for (int trgIndDay = 0; trgInd < dayRoutes.size(); ++trgInd) {
            for (int srcRouteInd = 0; srcRouteInd < dayRoutes[srcIndDay].routes.size(); ++srcRouteInd) {
                if (dayRoutes[srcIndDay].routes[srcRouteInd]->fuel <= Qmax) continue; //только перегруженные
                for (int trgRouteInd = 0; trgRouteInd < dayRoutes[trgIndDay].size(); ++trgRouteInd) {
                    if (trgRouteInd == srcRouteInd && srcIndDay == trgIndDay
                    || dayRoutes[trgIndDay].routes[trgRouteInd]->fuel >= Qmax) continue; //только не перегруженные
                    vector<Move>* m = findMoveInsertions(dayRoutes[srcIndDay].routes[srcRouteInd],
                                                        dayRoutes[trgIndDay].routes[trgRouteInd],
                                                        demand, edges,
                                                        dayRoutes[srcIndDay],
                                                        dayRoutes[trgIndDay],
                                                        srcRouteInd,
                                                        trgRouteInd,
                                                        Qmax);
                    if (!m->empty()) {
                        possibleMoves.insert(possibleMoves.end(), m->begin(), m->end());
                    }
                }
            }
        }
    }
    return possibleMoves;
}

