#include <bits/stdc++.h>
using namespace std;

typedef long long Long;
const Long LN = 17;
struct Grafo {
	Long n; bool bi;	
	vector<vector<Long>> ady;
	vector<Long> parte_ciclo;
	vector<Long> raices;
	vector<bool> vis;
	vector<vector<Long>> padre;
	vector<Long> depth;
	Grafo(Long N, bool B = true)
		: n(N), bi(B), ady(N), parte_ciclo(N, -1), vis(N, false), raices(N),
		depth(N), padre(LN, vector<Long>(N)) {}

	void Conecta(Long u, Long v) {
		if (bi) ady[v].push_back(u);
		ady[u].push_back(v);
	}

	vector<Long> ciclo;
	vector<char> color;

	void DetectarCiclo(Long u, Long p) {
		color[u] = ciclo.empty()? 'G' : 'N';
		for (Long v : ady[u]) {
			if (bi && v == p) continue;
			if (ciclo.empty() && color[v] == 'G')
				color[v] = 'A', ciclo.push_back(v),
				color[u] = 'R', ciclo.push_back(u);
			if (color[v] != 'B') continue;

			DetectarCiclo(v, u);
			if (color[u] == 'G' && color[v] == 'R')
				color[u] = 'R', ciclo.push_back(u);
		}
		if (color[u] == 'G') color[u] = 'N';
	}

	vector<vector<Long>> DetectarCiclos() {
		vector<vector<Long>> ciclos;
		color = vector<char>(n, 'B');
		for (Long u = 0; u < n; ++u) {
			if (color[u] != 'B') continue;
			ciclo.clear(); DetectarCiclo(u, u);
			reverse(ciclo.begin(), ciclo.end());
			if (ciclo.size() > 0) ciclos.push_back(ciclo);
		}

		init();
		for (Long i = 0; i < ciclos.size(); ++i)
			for (Long j = 0; j < ciclos[i].size(); ++j) {
				parte_ciclo[ ciclos[i][j] ] = i;								
				//Construir(ciclos[i][j]);
			}

		for (Long i = 0; i < ciclos.size(); ++i)
			for (Long j = 0; j < ciclos[i].size(); ++j)
				dfs(ciclos[i][j], -1, 0, ciclos[i][j]);

		for (auto &c : ciclos)
			sort(c.begin(), c.end());
		/*
		cout << "datos ciclos\n";
		for (auto c : ciclos) {
			for (auto d : c)
				cout << d << " ";
			cout << endl;
		}
		cout << endl << endl;		
		*/
		return ciclos;
	}

	Long busca(Long d) {
		if (vis[d])
			return raices[d];
		vis[d] = true;

		if (parte_ciclo[d] != -1)
			return d;

		for (auto nodo : ady[d]) {
			raices[d] = busca(nodo);
		}
		return raices[d];
	}

	void busquedas() {
		/*
		for (Long i = 0; i < n; ++i)
			if (!vis[i])
				raices[i] = busca(i);
		
		set<Long> R;
		for (auto var : raices)
			R.insert(var);

		for (auto var : R)
			dfs(var, -1, 0, var);
			//Construir(var);
		*/
		Construir();
	}
	/*
	Long dista(Long d) {
		if (vis[d])
			return distPadre[d];
		vis[d] = true;
		if (raices[d] == d)
			return 0;
		for (Long nodo : ady[d])
			distPadre[d] = dista(nodo) + 1;
		return distPadre[d];
	}
	void padres() {
		fill(vis.begin(), vis.end(), false);
		for (Long i = 0; i < n; ++i) {
			if (!vis[i])
				distPadre[i] = dista(i);
		}
	}
	*/
	void dfs(Long s, Long parent, Long d, int raiz) {		
		//cout << "  " << s << " -> " << d << endl;
		depth[s] = d;
		padre[0][s] = parent;
		raices[s] = raiz;

		for (Long v : ady[s]) if (depth[v] == -1 && parte_ciclo[v] == -1)
			dfs(v, s, d + 1, raiz);
	}

	Long LCA(Long u, Long v) {
		if (depth[u] < depth[v]) swap(u, v);
		for (Long i = LN; i >= 0; --i)
			if (depth[u] - (1<<i) >= depth[v])
				u = padre[i][u];

		if (u == v) return u;

		for (Long i = LN - 1; i >= 0; --i)
			if (padre[i][u] != -1 &&
				padre[i][u] != padre[i][v]) {
				u = padre[i][u];
				v = padre[i][v];
			}
		return padre[0][u];
	}

	void init() {
		for (Long i = 0; i < n; i++)
			depth[i] = -1;

		for (Long i = 0; i < LN; ++i)
			for (Long j = 0; j < n; ++j)
				padre[i][j] = -1;

	}

	void Construir() {
		//dfs(s, -1, 0);
		//cout << endl << endl;

		for (Long i = 1; i < LN; ++i)
			for (Long j = 0; j < n; ++j)
				if (padre[i - 1][j] != -1)
					padre[i][j] = padre[i - 1][ padre[i - 1][j] ];
	}
};

struct UF {
	Long n; vector<Long> padre, tam;

	UF(Long N) : n(N),
		tam(N, 1), padre(N) {
		while (--N) padre[N] = N;
	}

	Long raiz(Long u) {
		if (padre[u] == u) return u;
		return padre[u] = raiz(padre[u]);
	}

	void unir(Long u, Long v) {
		Long ru = raiz(u);
		Long rv = raiz(v);
		if (ru == rv) return;
		--n, padre[ru] = rv;
		tam[rv] += tam[ru];
	}
};

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	Long n;
	while (cin >> n) {
		UF arboles(n);
		Grafo g(n);
		for (Long i = 0; i < n; ++i) {
			Long num;
			cin >> num;
			arboles.unir(i, num - 1);
			g.Conecta(i, num - 1);
		}
		auto ciclos = g.DetectarCiclos();		
		g.busquedas();
		/*
		//g.padres();
		for (auto var : ciclos) {
			for (auto ele: var) {
				cout << ele << " ";
			}
			cout << endl << endl;
		}		
		Long conta = 0;
		for (auto var : g.raices) {
			cout << conta++  << " " << var << endl;
		}
		*/
		Long Q;
		cin >> Q;
		while(Q--) {
			Long a, b;
			cin >> a >> b;
			a--;
			b--;
			// Diferentes componentes
			if (arboles.raiz(a) != arboles.raiz(b)) {
				cout << "-1\n";
				continue;
			}

			// Dentro del ciclo
			if (g.parte_ciclo[a] == g.parte_ciclo[b] && g.parte_ciclo[b] != -1) {
				Long idx_ciclo = g.parte_ciclo[a];
				Long posA = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), a) - ciclos[idx_ciclo].begin();
				Long posB = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), b) - ciclos[idx_ciclo].begin();
				//cout << "ciclo " << min((Long)ciclos[idx_ciclo].size() - abs(posB - posA), abs(posA - posB)) << "\n";
				cout << min((Long)ciclos[idx_ciclo].size() - abs(posB - posA), abs(posA - posB)) << "\n";
				continue;
			}
			// Ciclo y arbol
			if (g.parte_ciclo[a] != -1 || g.parte_ciclo[b] != -1) {
				Long idx_ciclo = g.parte_ciclo[b];
				Long raizA = g.raices[a];
				Long centroA = b;
				Long nodo = a;
				if (g.parte_ciclo[a] > g.parte_ciclo[b]) {
					idx_ciclo = g.parte_ciclo[a];
					raizA = g.raices[b];
					centroA = a;
					nodo = b;
				}
				Long posA = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), raizA) - ciclos[idx_ciclo].begin();
				Long posB = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), centroA) - ciclos[idx_ciclo].begin();
				Long distCiclo = min((Long)ciclos[idx_ciclo].size() - abs(posB - posA), abs(posA - posB));
				//cout << "ciclo y arbol " << g.depth[nodo] + distCiclo << '\n';
				cout << g.depth[nodo] + distCiclo << '\n';
				continue;				
			}

			if (g.parte_ciclo[a] == -1 && g.parte_ciclo[b] == -1) {
				// Arboles diferentes
				//cout << "   " << g.raices[a] << " " << g.raices[b] << endl;
				if (g.raices[a] != g.raices[b]) {
					Long raizA = g.raices[a];
					Long raizB = g.raices[b];
					Long idx_ciclo = g.parte_ciclo[raizA];
					Long posA = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), raizA) - ciclos[idx_ciclo].begin();
					Long posB = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), raizB) - ciclos[idx_ciclo].begin();
					Long centro = min((Long)ciclos[idx_ciclo].size() - abs(posB - posA), abs(posA - posB));
					//cout << "diff arboles " << g.depth[a] + g.depth[b] + centro << "\n";
					cout << g.depth[a] + g.depth[b] + centro << "\n";
				} else { // LCA
					//cout << "LCA ";
					cout << g.depth[a] + g.depth[b] - 2*g.depth[g.LCA(a, b)] << "\n";
				}
			}
		}
	}
}
/*
3
2 1 2
3
1 2
1 3
1 1

7
2 1 4 5 3 5 6
5
1 3
4 7
7 4
6 2
2 1

6
2 3 1 5 6 4
4
1 2
6 1
5 4
3 2

19
2 3 1 3 4 4 6 6 2 9 9 11 11 9 10 2 16 16 1
8
1 2
1 7
17 19
12 5
15 4
14 18
9 11
19 14

ans:
1
4
4
6
5
4
1
4

*/
