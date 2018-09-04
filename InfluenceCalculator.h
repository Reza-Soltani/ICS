//
// Created by Reza Soltani on 8/29/18.
//

#include <set>
#include <vector>
#include "BiGraph.h"

#ifndef MAXIMUMCOMMUNITYINFLUENCE_INFLUENCECALCULATOR_H
#define MAXIMUMCOMMUNITYINFLUENCE_INFLUENCECALCULATOR_H

using namespace std;

class InfluenceCalculator {

    int n, m;
    vector<pair<int, double> > v[maxn], q[maxn];
    double dis[maxn], ap[maxn];
    pair<int, double> par[maxn][maxn];
    int h[maxn][maxn];

public:
    InfluenceCalculator(int n, int m);

    void add_edge(int fi, int se, double probability);

    double get_influence_of_set(set<int> query_nodes);

    void pre_process();

    void dijkstra(int x);

    double get_ap(int x, set<int> query_set);
};


#endif
