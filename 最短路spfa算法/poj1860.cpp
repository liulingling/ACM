#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

int n, m;

const int MaxV = 110;
const double INF =  -999999999.0;

double R[MaxV][MaxV], C[MaxV][MaxV];
bool ex[MaxV][MaxV], inq[MaxV];
int cnt[MaxV];
double dist[MaxV];

bool relax(int u, int v){
	
	double cur = (dist[u] - C[u][v]) * R[u][v];
	//printf("%d %d %lf %lf\n", u, v, dist[u], cur); 
	if(dist[v] < cur){
		dist[v] = cur;
		return true;
	}
	return false;
}
bool spfa(int s, double sc){
	for(int i = 0; i < n; i++){
		dist[i] = INF;
		cnt[i] = 0;
		inq[i] = false;		
	}
	inq[s] = true;
	dist[s] = sc;
	queue<int> que;
	que.push(s);
	cnt[s] = 1;
	int u, v;
	while(!que.empty()){
		u = que.front();
		que.pop();
		inq[u] = false;
		for(int i = 0; i < n; i++){
			if(ex[u][i]){
				if(relax(u, i) && !inq[i]){
					inq[i] = true;
					cnt[i]++;
					if(cnt[i] > n)
						return true;
					que.push(i);
					
				}
			}
		}
	}
	return false; 
}
int main(){
	int s;
	double stv;
	while(scanf("%d %d %d %lf", &n, &m, &s, &stv) != EOF){
		s--;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				ex[i][j] = false;
		int u, v;
		double ruv, cuv, rvu, cvu;
		//是否存在u,v 出现多次 ？？ 
		for(int i = 0; i < m; i++){
			scanf("%d %d %lf %lf %lf %lf", &u, &v, &ruv, &cuv, &rvu, &cvu);
			u--, v--;
			ex[u][v] = true;
			ex[v][u] = true;
			R[u][v] = ruv, C[u][v] = cuv;
			R[v][u] = rvu, C[v][u] = cvu;
		}
		bool flag = spfa(s, stv);
		printf("%s\n", flag ? "YES" : "NO");		
	}	
	return 0;
}
