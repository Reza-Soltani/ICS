#include <iostream>

using namespace std;

bool mat[1000][1000], mark[1000];

int main() {
	
	srand(time(0));

	int n, m, k, c, q;
	cin >> n >> m >> k >> c >> q;
	
	cout << n << " " << m << endl;
	while (m > 0) {
		int fi = rand() % n + 1;
		int se = rand() % n + 1;
		if (fi == se or mat[fi][se])
			continue;
		mat[fi][se] = true;
		m--;
		cout << fi << " " << se << " " << ((double) (rand())) / (RAND_MAX) << endl;
	}
	cout << k << " " << c << endl;
	cout << q << endl;
	while (q > 0) {
		int x = rand() % n + 1;
		if (mark[x])
			continue;
		cout << x << " ";
		q--;
	}
	cout << endl;
}
