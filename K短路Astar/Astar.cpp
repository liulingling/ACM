#include <iostream>
#include <algorithm>

using namespace std;
const int MaxV  = 1010;
const int INF = 1000000000;

int n, m, x, ct;
int g[MaxV][MaxV], gr[MaxV][MaxV];
int dist[MaxV], v[MaxV];
//gi = gi + hi
struct Node{
	int id, fi, gi;
	friend bool operator < (Node a, Node b){
		if(a.gi == b.gi) return a.fi > b.fi;
		return a.gi > b.gi;
	}
};
Node s[2000010];
int init(){
	for(int i = 0; i <= n; i++){
		dist[i] = INF;
		v[i] = 1;
	}
	dist[n - 1] = 0;
	for(int i = 0; i < n; i++){
		int k = n;
		for(int j = 0; j < n; j++){
			if(v[j] && dist[j] < dist[k]) k = j;
		}
		if(k == n)	break;
		v[k] = 0;
		for(int j = 0; j < n; j++){
			if(v[j] && dist[k] + gr[k][j] < dist[j]){
				dist[j] = dist[k] + gr[k][j];
			}
		}
	}
	return 1;
}

int solve(){
	if(dist[0] == INF)	return -1;
	ct = 0;
	s[ct].id = 0;
	s[ct].fi = 0;
	s[ct++].gi = dist[0];
	int cnt = 0;
	while(ct){
		int id = s[0].id, fi = s[0].fi, gi = s[0].gi;
		if(id == n - 1)	cnt++;
		if(cnt == x) return fi;
		pop_heap(s, s + ct);
		ct--;
		for(int j = 0; j < n; j++){
			if(g[id][j] < INF){
				s[ct].id = j;
				s[ct].fi = fi + g[id][j];
				s[ct++].gi = s[ct].fi + dist[j];
				push_heap(s, s + ct);
			}
		}
	}
	return -1;	
}
int main(){
	
	while(scanf("%d %d %d", &n, &m, &x) != EOF){
		for(int i = 0; i < n; i++){
			for(int j = 0; j < m; j++){
				g[i][j] = gr[i][j] = INF;
			}
		}
		int u, v, c;
		for(int i = 0; i < m; i++){
			scanf("%d %d %d", &u, &v, &c);
			u--, v--;
			g[u][v] <?= c;
			gr[v][u] <?= c;
		}
		init();
		printf("%d\n", solve());		
	}	
	return 0;
}
