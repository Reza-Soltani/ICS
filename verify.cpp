#include <iostream>
#include <vector>

using namespace std;

const int maxn = 100;

bool mark[maxn * maxn];
bool mat[maxn][maxn];
pair<pair<int, int>, bool> eg[maxn * maxn];
pair<int, pair<int, int> > tri[maxn * maxn * maxn];
vector<int> tri_adj[maxn * maxn * maxn], eg_adj[maxn * maxn];
int edges_count, triangles_count;

bool edge_in_triangle(pair<pair<int, int>, bool> edge, pair<int, pair<int, int> > triangle) { // is an undirected edge in a triangle?
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

void dfs(int x) {
	mark[x] = true;
	for (auto y: eg_adj[x]) {
		for (auto z: tri_adj[y]) {
			if (!mark[z])
				dfs(z);
		}
	}
}

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		int x;
		cin >> x;
	}
	int m;
	cin >> m;
	for (int i = 0; i < m; i++) {
		int fi, se, d;
		cin >> fi >> se >> d;
		mat[fi][se] = true;
		if (d)
			mat[se][fi] = true;
	}

	n = 50;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (mat[i][j] and mat[j][i]) { // reciprocal edge
				if (i < j) { // not added before
					edges_count++;
					eg[edges_count] = make_pair(make_pair(i, j), true);
				}
			}
			else if (mat[i][j]) { // directed edge
				edges_count++;
				eg[edges_count] = make_pair(make_pair(i, j), false);
			}
		}
	}
	
	for (int i = 1; i <= n; i++) {
		for (int j = i + 1; j <= n; j++) {
			for (int z = i + 1; z <= n; z++) { // For variable initialized to create node for each triangle once! 
				if (mat[i][j] and mat[j][z] and mat[z][i]) {
					triangles_count++;
					tri[triangles_count] = make_pair(i, make_pair(j, z));
				}
			}
		}
	}
	for (int i = 1; i <= edges_count; i++) {
		for (int j = 1; j <= triangles_count; j++) {
			if (edge_in_triangle(eg[i], tri[j])) {
				eg_adj[i].push_back(j);
				tri_adj[j].push_back(i);
			}
		}
	}
	int c;
	cin >> c;
	for (int i = 1; i <= edges_count; i++) {
		if (eg_adj[i].size() < c - 2) {
			cout << "WRONG DEGREE" << endl;
			return 0;
		}
	}
	dfs(1);
	for (int i = 1; i <= edges_count; i++) {
		if (!mark[i]) {
			cout << "NOT CONNECTED" << endl;
			return 0;
		}
	}
	cout << "ACCEPTED" << endl;
}
