#include <iostream>
#include <vector>
#include "InfluenceCalculator.h"
#include "BiGraph.h"

using namespace std;

typedef pair<int, int> pie;
typedef pair<pair<int, int>, int> EG;
typedef pair<int, pair<int, int> > TR;

bool mat[maxn][maxn];
EG eg[maxn * maxn];
TR tri[maxn * maxn * maxn];

bool edge_in_triangle(EG edge, TR triangle);

bool component_contains_nodes(vector<int> &edges, vector<int> &nodes);

set<int> vec2set(vector<int> &v);

void solve(BiGraph &biGraph, vector<int> v, vector<int> q, int c, int k, InfluenceCalculator &influenceCalculator);

vector<int> get_one_community_contain_nodes(BiGraph &biGraph, vector<int> q);

vector<int> get_edges_containing_node(vector<int> v, int x);

set<int> get_nodes(vector<int> edges) {
    set<int> ret;
    for (auto x: edges) {
        ret.insert(eg[x].first.first);
        ret.insert(eg[x].first.second);
    }
    return ret;
}

int main() {


    int n, m;
    cin >> n >> m; // nodes and edges count


    InfluenceCalculator influenceCalculator(n, m);

    for (int i = 0; i < m; i++) { // get all edges from input
        int fi, se;
        double pr;
        cin >> fi >> se >> pr; // edge source, destination and probability of influence

        influenceCalculator.add_edge(fi, se, pr); // add it to influence graph
        mat[fi][se] = true;
    }

    influenceCalculator.pre_process();

    int k, c; // maximum answer size and c-truss parameter
    cin >> k >> c;

    unsigned int q_size;
    cin >> q_size; // size of Q

    if (q_size > k) {
        cout << "No Solution" << endl;
        return 0;
    }

    vector<int> Q(q_size); // Q is query nodes set
    for (int i = 0; i < q_size; i++) {
        cin >> Q[i];
    }

    clock_t st = clock();

    BiGraph biGraph;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (mat[i][j] and mat[j][i]) { // reciprocal edge
                if (i < j) { // not added before
                    int node_number = biGraph.add_node(1);
                    eg[node_number] = make_pair(make_pair(i, j), true);
                }
            } else if (mat[i][j]) { // directed edge
                int node_number = biGraph.add_node(1);
                eg[node_number] = make_pair(make_pair(i, j), false);
            }
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            for (int z = i + 1; z <= n; z++) { // For variable initialized to create node for each triangle once!
                if (mat[i][j] and mat[j][z] and mat[z][i]) {
                    int node_number = biGraph.add_node(2);
                    tri[node_number] = make_pair(i, make_pair(j, z));
                }
            }
        }
    }


    for (int i = 1; i <= biGraph.get_part_size(1); i++) {
        for (int j = 1; j <= biGraph.get_part_size(2); j++) {
            if (edge_in_triangle(eg[i], tri[j])) {
                biGraph.add_edge(i, j);
            }
        }
    }


    biGraph.remove_from_part_one_all_nodes_with_degree_less_than_c(c - 2);

    vector<vector<int> > cc = biGraph.get_connected_components();
    for (auto v: cc) {
        if (component_contains_nodes(v, Q)) {
            solve(biGraph, v, Q, c, k, influenceCalculator);
        }
    }

    clock_t en = clock();

    cout << "Time: " << (double (en - st)) / (CLOCKS_PER_SEC);
}




void solve(BiGraph &biGraph, vector<int> v, vector<int> q, int c, int k, InfluenceCalculator &influenceCalculator) {
    set<int> Q = vec2set(q);
    while (get_nodes(v).size() > k) {
    /*    cout << "Graph Nodes Are: " << endl;
        for (auto x: get_nodes(v))
            cout << x <<  " ";
        cout << endl; */

        set<int> nodes = get_nodes(v);

        double best_exp = -1;
        int best_node = -1;

        for (auto x: nodes) {
            if (Q.find(x) != Q.end()) // we can't remove query nodes
                continue;

            biGraph.remove_nodes(get_edges_containing_node(v, x), c - 2);

            vector<int> nc = get_one_community_contain_nodes(biGraph, q);

            if (nc.empty()) {
                biGraph.undo_remove_last_nodes();
                continue;
            }

            double d = influenceCalculator.get_influence_of_set(get_nodes(nc));
        //    cout << "Removing " << x << " Will make influence: " << d << endl;
            if (d >= best_exp) {
                best_exp = d;
                best_node = x;
            }

            biGraph.undo_remove_last_nodes();
        }

    //    cout << "So LETS REMOVE " << best_node << endl;

        if (best_node == -1) {
            cout << "BEST ANSWER " << get_nodes(v).size() << endl;
            cout << "No Solution" << endl;
            return ;
        } else {
            biGraph.remove_nodes(get_edges_containing_node(v, best_node), c - 2);
            v = get_one_community_contain_nodes(biGraph, q);
        }
    }
    if (get_nodes(v).empty()) {
        cout << "No Solution" << endl;
    } else {
        cout << get_nodes(v).size() << endl;
        for (auto x: get_nodes(v)) {
            cout << x << endl;
        }
        cout << v.size() << endl;
        for (auto x: v) {
            cout << eg[x].first.first << " " << eg[x].first.second << " " << eg[x].second << endl;
        }
    }
}

vector<int> get_edges_containing_node(vector<int> v, int x) {
    vector<int> ret;
    for (auto y: v) {
        if (eg[y].first.first == x or eg[y].first.second == x)
            ret.push_back(y);
    }
    return ret;
}

vector<int> get_one_community_contain_nodes(BiGraph &biGraph, vector<int> q) {
    vector<vector<int> > cc = biGraph.get_connected_components();
    for (auto v: cc) {
        if (component_contains_nodes(v, q))
            return v;
    }
    return vector<int>(0);
}

bool component_contains_nodes(vector<int> &edges, vector<int> &nodes) {
    set<int> nodes_set = vec2set(nodes);
    for (auto x: edges) {
        if (nodes_set.find(eg[x].first.first) != nodes_set.end())
            nodes_set.erase(eg[x].first.first);

        if (nodes_set.find(eg[x].first.second) != nodes_set.end())
            nodes_set.erase(eg[x].first.second);
    }
    return nodes_set.empty();
}

set<int> vec2set(vector<int> &v) {
    set<int> s;
    for (auto x: v) {
        s.insert(x);
    }
    return s;
}

bool edge_in_triangle(EG edge, TR triangle) { // is an edge in a triangle?
    int t1, t2, t3, fi, se;
    t1 = triangle.first;
    t2 = triangle.second.first;
    t3 = triangle.second.second;
    fi = edge.first.first;
    se = edge.first.second;
    bool ans = ((t1 == fi and t2 == se) or (t2 == fi and t3 == se) or (t3 == fi and t1 == se));
    if (edge.second) {
        ans |= ((t1 == se and t2 == fi) or (t2 == se and t3 == fi) or (t3 == se and t1 == fi));
    }
    return ans;
}