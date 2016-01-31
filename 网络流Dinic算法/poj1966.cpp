#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

int minmaxFlow;

namespace AliasDinic{
	
#include <math.h>

const int MaxV = 210;
const int INF = 10000000;
	int S, T, N;
	int (*pc)[MaxV], *pdis;
	bool Dinic_bfs(){
		queue<int> q;
		for(int i = 0; i < N; i++){
			pdis[i] = -1;
		}
		q.push(S), pdis[S] = 0;
		int u, v;
		while(!q.empty()){
			u = q.front();
			q.pop();
			for(int i = 0; i < N; i++){
				if(pc[u][i] > 0 && pdis[i] == -1){
					pdis[i] = pdis[u] + 1;
					q.push(i);
				}
			}
		}
		if(pdis[T] != -1)	return true;
		return false;
	}
	int Dinic_dfs(int u, int sum){
		int os = sum;
		if(u == T){
			return sum;
		}
		for(int i = 0; i < N && sum; i++){
			if(pdis[i] == pdis[u] + 1 && pc[u][i] > 0){
				int tmp = Dinic_dfs(i, min(sum, pc[u][i]));
				sum -= tmp;
				pc[u][i] -= tmp;
				pc[i][u] += tmp;
			}
		}
		return os - sum;	
	}
	void Dinic(int n, int c[][MaxV], int dis[]){
		N = n, pc = c, pdis = dis;
	}
	int solve(int s, int t){
		S = s, T = t;
		int ans = 0;
		while(Dinic_bfs()){
			ans += Dinic_dfs(S, INF);
			if(ans > minmaxFlow)
				return ans;
		}
		return ans;
	}
}

const int MaxV = AliasDinic::MaxV;
int C[MaxV][MaxV], dis[MaxV];
int mat[MaxV][MaxV];

void rebuild(int s, int t, int n){
	int NN =  2 * n;
	for(int i = 0; i < NN; i++){
		for(int j = 0; j < NN; j++){
			mat[i][j] = 0;
		}
	}
	for(int i = 0; i < n; i++){
		mat[i][i +  n] = 1;
		for(int j = i + 1; j < n; j++){
			if(C[i][j]){
				if(j != s && i + t)
					mat[i + n][j] = AliasDinic::INF;
				if(i != s && j != t)
					mat[j + n][i] = AliasDinic::INF;	
			}
		}
	}
	mat[s][s + n] = AliasDinic::INF;
	mat[t][t + n] = AliasDinic::INF;
}
int main(){
	int n,  m;
	while(scanf("%d %d", &n, &m) != EOF){
		
		int u, v;
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				C[i][j] = 0;
			}
		}
				
		for(int i = 0; i < m; i++){
			scanf(" (%d,%d)", &u, &v);
			C[u][v] = 1;
			C[v][u] = 1;	
		}
		if(n == 0 || n == 1){
			printf("%d\n", n);
			continue;
		}
		if(m == 0){
			printf("0\n");
			continue;
		}
		
		minmaxFlow = AliasDinic::INF;
		

		for(int i = 0; i < n; i++){
			if(minmaxFlow == 0)
				break;
			for(int j = i + 1; j < n; j++){
				if(minmaxFlow == 0)
					break;
				if(C[i][j] == 1)
					continue;
				
				rebuild(i, j, n);
				
				AliasDinic::Dinic(2 * n, mat, dis);
				
				int tmp = AliasDinic::solve(i, j + n);
				if(tmp < minmaxFlow){
				
					minmaxFlow = tmp;
					
				}
			}
		}
		if(minmaxFlow >= n - 1){
			printf("%d\n", n);
		}else{
			printf("%d\n", minmaxFlow);
		}	
		
	}
	
	return 0;
}
