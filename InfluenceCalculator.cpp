//
// Created by Reza Soltani on 8/29/18.
//

// TODO: fill this class


#include <cmath>
#include <queue>
#include "InfluenceCalculator.h"

const double inf = 1e9;

InfluenceCalculator::InfluenceCalculator(int n, int m) {
    this->n = n;
    this->m = m;
}

void InfluenceCalculator::add_edge(int fi, int se, double probability) {
    v[fi].push_back(make_pair(se, probability));
    q[se].push_back(make_pair(fi, probability));
}

double InfluenceCalculator::get_influence_of_set(set<int> query_nodes) {
    double ret = 0;
    for (int i = 1; i <= n; i++) {
        ret += get_ap(i, query_nodes);
    }
    return ret;
}

void InfluenceCalculator::dijkstra(int x) {
    for (int i = 1; i <= n; i++)
        dis[i] = inf;
    dis[x] = 0;
    par[x][x] = make_pair(-1, 1);
    set<pair<double, int> > s;
    for (int i = 1; i <= n; i++)
        s.insert(make_pair(dis[i], i));
    while (s.size()) {
        int cur = s.begin()->second;
        s.erase(s.begin());
        for (auto y: q[cur]) {
            if (dis[cur] + -log(y.second) < dis[y.first]) {
                s.erase(s.find(make_pair(dis[y.first], y.first)));
                dis[y.first] = dis[cur] + -log(y.second);
                s.insert(make_pair(dis[y.first], y.first));
                par[x][y.first] = make_pair(cur, y.second);
                h[x][y.first] = h[x][cur] + 1;
            }
        }
    }

}

void InfluenceCalculator::pre_process() {
    memset(h, 0, sizeof h);
    for (int i = 1; i <= n; i++) {
        dijkstra(i);
    }
}

double InfluenceCalculator::get_ap(int x, set<int> query_set) {
    for (int i = 1; i <= n; i++)
        ap[i] = 0;
    set<pair<int, int> > s;
    for (auto y: query_set) {
        ap[y] = 1;
        s.insert(make_pair(h[x][y], y));
    }
    while (s.size()) {
        int cur = s.begin()->second;
        s.erase(s.begin());
        if (par[x][cur].first == -1)
            return ap[x];
        int p = par[x][cur].first;
        ap[p] = ap[p] + (1 - ap[p]) * par[x][cur].second;
        s.insert(make_pair(h[x][p], p));
    }
    return ap[x];
}
