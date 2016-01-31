#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;
#define LLINT long long

namespace AliasDinicADL{
	
struct Edge{
	int pnt, nxt;
	LLINT flow;
};
	Edge *pedges;
	int *phead, *pdis;
	int N, nowE;
	void Dinic(int n, int head[], int dis[], Edge edges[]){
		N = n, phead = head, pdis = dis, pedges = edges;
	}
	void initMap(){
		nowE = 0;
		for(int i = 0; i < N; i++)
			phead[i] = -1;
	}
	void addedge(int u, int v, LLINT f){
		pedges[nowE].pnt = v, pedges[nowE].nxt = phead[u];
		pedges[nowE].flow = f, phead[u] = nowE++;
		
		pedges[nowE].pnt = u, pedges[nowE].nxt = phead[v];
		pedges[nowE].flow = 0, phead[v] = nowE++;
	}
	bool DinicBfs(int s, int t){
		queue<int> q;
		for(int i = 0; i < N; i++)
			pdis[i] = -1;
		int u, v;
		q.push(s);
		pdis[s] = 0;
		while(!q.empty()){
			u = q.front();
			q.pop();
			int ee = phead[u];
			while(ee != -1){
				v = pedges[ee].pnt;
				if(pdis[v] == -1 && pedges[ee].flow > 0){
					pdis[v] = pdis[u] + 1;
					q.push(v);
				}
				ee = pedges[ee].nxt;
			}
		}
		if(pdis[t] != -1) return true;
		return false;
	}
	LLINT mymin(LLINT a, LLINT b){
		if(a > b)
			return b;
		return a;
	}
	LLINT DinicDfs(int s, int t, int u, LLINT os){
		if(u == t)
			return os;
		LLINT sum = os;
		int ee = phead[u];
		while(ee != -1 && sum > 0){
			int v = pedges[ee].pnt;
			if(pdis[v] == pdis[u] + 1 && pedges[ee].flow > 0){
				int tmp = DinicDfs(s, t, v, mymin(sum, pedges[ee].flow));
				pedges[ee].flow -= tmp;
				pedges[ee ^ 1].flow += tmp;
				sum -= tmp;
			}
			ee = pedges[ee].nxt;
		}
		return os - sum;
	}
	LLINT solve(int s, int t){
		LLINT ans = 0;
		
		LLINT INF = 100000000;
		INF *= 1000;
		while(DinicBfs(s, t)){
			ans += DinicDfs(s, t, s, INF);
		}
		return ans;
	}
	
}

const int MaxV = 10010;
const int MaxE = 230000;

int head[MaxV], dis[MaxV];
AliasDinicADL::Edge edges[MaxE];

bool flag[MaxV];
int n, m;

void dfs(int u){
	if(flag[u] == true)
		return ;
	flag[u] = true;
	int ee = head[u];
	while(ee != -1){
		int v = edges[ee].pnt;
		if(v != 0 && v != (n + 1) /*&& ((ee & 1) == 0)*/ && edges[ee].flow > 0){
			dfs(v);
		}
		ee = edges[ee].nxt;
	}
}
int main(){
	
	while(scanf("%d %d", &n, &m) != EOF){
		LLINT sum = 0;
		int profit;
		
		AliasDinicADL::Dinic(n + 2, head, dis, edges);
		AliasDinicADL::initMap();
		
		for(int i = 1; i <= n; i++){
			scanf("%d", &profit);
			if(profit >= 0){
				sum += profit;
				AliasDinicADL::addedge(0, i, profit);
			}else if(profit < 0){
				AliasDinicADL::addedge(i, n + 1, -profit);
			}	
		}
		int u, v;
		LLINT INF = 100000000;
		INF *= 1000;
		
		for(int i = 0; i < m; i++){
			scanf("%d %d", &u, &v);
			AliasDinicADL::addedge(u, v, INF);
		}
		
		LLINT ans = AliasDinicADL::solve(0, n + 1);
		
		for(int i = 1; i <= n; i++)
			flag[i] = false;
		int nums = 0;
		int ee = head[0];
		
		while(ee != -1){
			v = edges[ee].pnt;
			if(edges[ee].flow > 0)
				dfs(v);
			ee = edges[ee].nxt;
		}
		
		for(int i = 1; i <= n; i++){
			if(flag[i] == true)
				nums++;
		}
		printf("%d %lld\n", nums, sum - ans);
	}	
	
	return 0;
}
