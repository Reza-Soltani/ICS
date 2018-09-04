//
// Created by Reza Soltani on 8/29/18.
//

#include <set>

#ifndef MAXIMUMCOMMUNITYINFLUENCE_BIGRAPH_H
#define MAXIMUMCOMMUNITYINFLUENCE_BIGRAPH_H

using namespace std;

const int maxn = 52;

class BiGraph {
    int part_one_n, part_two_n;

    set<int> part_one_adjs[maxn * maxn], part_two_adjs[maxn * maxn * maxn];

    bool deleted[maxn * maxn], mark[maxn * maxn];

    void delete_node(int x);

public:
    BiGraph();

    int add_node(int part_number);

    int get_part_size(int part_number);

    void add_edge(int fi, int se);

    void remove_from_part_one_all_nodes_with_degree_less_than_c(int c);

    void delete_edge(int fi, int se);

    vector<vector<int> > get_connected_components();

    void dfs(int x, vector<int> &v);

    void remove_nodes(vector<int> v, int c);

    void undo_remove_last_nodes();
};


#endif
