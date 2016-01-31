#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

namespace AliasDinic{
	
#include <math.h>

const int MaxV = 410;
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
		}
		return ans;
	}
}

const int MaxV = AliasDinic::MaxV;
int C[MaxV][MaxV], dis[MaxV];


int cows[MaxV], contains[MaxV];
int cntcows = 0, cntctins = 0;

#define LLINT long long

long long limits[MaxV * MaxV];
long long tmp[MaxV * MaxV];

long long sdis[MaxV][MaxV];
int cntLimits;

bool rebuild(long long distance, int n){
	int s = 2 * n, t = 2 * n + 1;
	for(int i = 0; i < t; i++)
		for(int j = 0; j < t; j++)
			 C[i][j] = 0;
	for(int i = 0; i < n; i++){
		C[i][i + n] = AliasDinic::INF;
		if(cows[i] != 0){
			C[s][i] = cows[i];
		}
		if(contains[i] != 0){
			C[i + n][t] = contains[i];
		}
		for(int j = 0; j < n; j++){
			if(sdis[i][j] != -1 && sdis[i][j] <= distance){
				C[i][j + n] = AliasDinic::INF;
			}
		}
	}
	AliasDinic::Dinic(2 * n + 2, C, dis);
	int ans = AliasDinic::solve(s, t);
	
	if(ans >= cntcows)
		return true;
	return false;	
}
int main(){
	
	int F, P;
	while(scanf("%d %d", &F, &P) != EOF){
		cntcows = 0, cntctins = 0;
		int tmpmin;
		for(int i = 0; i < F; i++){
			scanf("%d %d", &cows[i], &contains[i]);
			cntcows += cows[i];
			cntctins += contains[i];
		}
		for(int i = 0; i < F; i++)
		{
			for(int j = 0; j < F; j++)
				sdis[i][j] = -1;
		}
		int u, v, l;
		for(int i = 0; i < P; i++){
			scanf("%d %d %d", &u, &v, &l);
			u--, v--;
			if(sdis[u][v] == -1 || sdis[u][v] > l)
				sdis[u][v] = sdis[v][u] = l;
		}
		for(int k = 0; k < F; k++){
			for(int i = 0; i < F; i++){
				if(i == k || sdis[i][k] == -1) 	continue;
				for(int j = 0; j < F; j++){
					if(j == i || j == k || sdis[k][j] == -1) continue;
					if(sdis[i][j] == -1 || sdis[i][j] > sdis[i][k] + sdis[k][j])
						sdis[i][j] = sdis[i][k] + sdis[k][j];
				}
			}
		}
		cntLimits = 0;
		for(int i = 0; i < F; i++){
			for(int j = i + 1; j < F; j++){
				if(sdis[i][j] != -1)
					tmp[cntLimits++] = sdis[i][j];
			}
		}
		
		sort(tmp, tmp + cntLimits);
		tmp[cntLimits] = -1;
		cntLimits = 0;
		for(int i = 0; tmp[i] != -1; i++){
			if(i == 0 || tmp[i] != tmp[i - 1])
				limits[cntLimits++] = tmp[i];
		}
		int left = 0, right = cntLimits - 1, mid;
		long long ans = limits[right] + 10;
		while(left <= right){
			mid = (left + right) / 2;
			if(rebuild(limits[mid], F)){
				right = mid - 1;
				ans = limits[mid];
			}else{
				left = mid + 1;
			}
		}
		if(ans > limits[cntLimits - 1])
			printf("-1\n");
		else	printf("%lld\n", ans);
	}
	return 0;
}
