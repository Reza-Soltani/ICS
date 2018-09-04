//
// Created by Reza Soltani on 8/29/18.
//

#include <vector>
#include <queue>
#include <iostream>
#include "BiGraph.h"

BiGraph::BiGraph() {
    part_one_n = 0;
    part_two_n = 0;
    for (bool &i : deleted)
        i = false;
}

int BiGraph::add_node(int part_number) {
    if (part_number == 1)
        return ++part_one_n;
    else
        return ++part_two_n;
}

int BiGraph::get_part_size(int part_number) {
    if (part_number == 1)
        return part_one_n;
    else
        return part_two_n;
}

void BiGraph::add_edge(int fi, int se) {
    part_one_adjs[fi].insert(se);
    part_two_adjs[se].insert(fi);
}

void BiGraph::remove_from_part_one_all_nodes_with_degree_less_than_c(int c) {
    queue<int> Q;
    for (int i = 1; i <= part_one_n; i++) {
        if (deleted[i])
            continue;
        if (part_one_adjs[i].size() < c) {
            delete_node(i);
            Q.push(i);
        }
    }
    while (!Q.empty()) {
        int cur = Q.front();
        Q.pop();

        vector<pair<int, int> > v;
        set<int> maybe_changed;
        for (auto x: part_one_adjs[cur]) {
            for (auto y: part_two_adjs[x]) {
                v.push_back(make_pair(y, x));
                if (!deleted[y]) {
                    maybe_changed.insert(y);
                }
            }
        }

        for (auto p: v) {
            delete_edge(p.first, p.second);
        }

        for (auto x: maybe_changed) {
            if (part_one_adjs[x].size() < c) {
                delete_node(x);
                Q.push(x);
            }
        }
    }
}

void BiGraph::delete_node(int x) {
    deleted[x] = true;
}

void BiGraph::delete_edge(int fi, int se) {
    part_one_adjs[fi].erase(se);
    part_two_adjs[se].erase(fi);
}

vector<vector<int> > BiGraph::get_connected_components() {
    vector<vector<int> > ret;
    memset(mark, false, sizeof mark);
    for (int i = 1; i <= part_one_n; i++) {
        if (deleted[i])
            continue;
        if (!mark[i]) {
            vector<int> cur;
            dfs(i, cur);
            ret.push_back(cur);
        }
    }
    return ret;
}

void BiGraph::dfs(int x, vector<int> &v) {
    mark[x] = true;
    v.push_back(x);
    for (auto y: part_one_adjs[x]) {
        for (auto z: part_two_adjs[y]) {
            if (!deleted[z] and !mark[z]) {
                dfs(z, v);
            }
        }
    }
}

vector<pair<int, int> > history;

void BiGraph::remove_nodes(vector<int> v, int c) {
    history.clear();

    queue<int> Q;
    for (auto x: v) {
        if (!deleted[x]) {
            history.push_back(make_pair(-1, x));
            delete_node(x);
            Q.push(x);
        }
    }

    while (!Q.empty()) {
        int cur = Q.front();
        Q.pop();

        vector<pair<int, int> > vec;
        set<int> maybe_changed;
        for (auto x: part_one_adjs[cur]) {
            for (auto y: part_two_adjs[x]) {
                vec.push_back(make_pair(y, x));
                if (!deleted[y]) {
                    maybe_changed.insert(y);
                }
            }
        }

        for (auto p: vec) {
            history.push_back(make_pair(p.first, p.second));
            delete_edge(p.first, p.second);
        }

        for (auto x: maybe_changed) {
            if (part_one_adjs[x].size() < c) {
                history.push_back(make_pair(-1, x));
                delete_node(x);
                Q.push(x);
            }
        }
    }
}

void BiGraph::undo_remove_last_nodes() {
    for (auto p: history) {
        if (p.first == -1)
            deleted[p.second] = false;
        else
            add_edge(p.first, p.second);
    }
}
