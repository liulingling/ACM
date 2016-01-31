#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

namespace AliasSPFA{

const int MaxV = 110;

	int (*pmat)[MaxV], *pdist;
	bool *pinq;
	int N;
	void SPFA(int n, int mat[][MaxV], int dist[],
		bool inq[]){
		N = n, pmat = mat, pdist = dist;
		pinq = inq;
	}
	bool relax(int u, int v, int l){
		if(pdist[v] == -1 || pdist[v] > pdist[u] + l){
			pdist[v] = pdist[u] + l;
			return true;
		}
		return false;
	}
	void solve(int s){
		queue<int> que;
		for(int i = 0; i < N; i++){
			pdist[i] = -1;
			pinq[i] = false;
		}
		pinq[s] = true;
		pdist[s] = 0;
		que.push(s);
		while(!que.empty()){
			int u = que.front();
			pinq[u] = false;
			que.pop();
			for(int i = 0; i < N; i++){
				if(pmat[u][i] != -1){
					if(relax(u, i, pmat[u][i]) && !pinq[i]){
						pinq[i] = true;
						que.push(i);
					}
				}
			}
		}
	}
		
};

const int MaxV = AliasSPFA::MaxV;

int mat[MaxV][MaxV], dist[MaxV];
int mat1[MaxV][MaxV];
bool inq[MaxV];
int levels[MaxV];
int tmpl[MaxV];

int main(){
	int m, n;
	while(scanf("%d %d", &m, &n) != EOF){
		for(int i = 0; i <= n; i++){
			for(int j = 0; j <= n; j++){
				mat[i][j] = -1;
				mat1[i][j] = -1;
			}
		}
		int lstd = 0;
		int p, l, x;
		for(int i = 1; i <= n; i++){
			scanf("%d %d %d", &p, &l, &x);
			if(i == 1){
				lstd = l;
			}
			levels[i] = l;
			tmpl[i - 1] = l;
			mat[0][i] = p;
			mat1[0][i] = p;
			int t, v;
			for(int j = 0; j < x; j++){
				scanf("%d %d", &t, &v);
				if(abs(lstd - l) <= m)
					mat[t][i] = v;
					mat1[t][i] = v;
			}
		}
		sort(tmpl, tmpl + n);			
		int ans = -1;
		for(int i = 0; i < n; i++){
			if(i != 0 && tmpl[i] == tmpl[i - 1])
				continue;
			for(int j = 1; j <= n; j++){
				for(int k = 1; k <= n; k++){
					if(mat[j][k] != -1 && (levels[j] >= tmpl[i] && levels[j] <= tmpl[i] + m
						&&levels[k] >= tmpl[i] && levels[k] <= tmpl[i] + m)){
						mat1[j][k] = mat[j][k];
						
					}else{
						mat1[j][k] = -1;
					}
				}
			}
			//printf("!!%d %d\n", mat1[4][2], levels[2]);
			AliasSPFA::SPFA(n + 1, mat1, dist, inq);
			AliasSPFA::solve(0);
			//printf("##%d %d\n", dist[1], tmpl[i]);
			if(ans == -1 || (ans > dist[1] && dist[1] != -1))
				ans = dist[1];
		}
		printf("%d\n", ans);	
	}
	return 0;
}
