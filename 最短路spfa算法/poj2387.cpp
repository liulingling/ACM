#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;


namespace AliasSpfaADL{

#define LLINT int
struct Edge{
	int nxt, pnt;
	int v;	
};
	int *phead;
	LLINT *pdis;
	bool *pinq;
	Edge *pedges;
	int N;
	void SpfaADL(int n, int head[], LLINT dis[], bool inq[], Edge edges[]){
		phead = head, pdis = dis, pinq = inq, pedges = edges;
		N = n;
	}
	bool relax(int u, int v, int c){
		if(pdis[v] == -1 || pdis[v] > pdis[u] + c){
			pdis[v] = pdis[u] + c;
			return true;
		}
		return false;
	}
	void solve(int s){
		for(int i = 0; i < N; i++){
			pinq[i] = false;
			pdis[i] = -1;
		}
		pdis[s] = 0;
		pinq[s] = true;
		queue<int> que;
		que.push(s);
		while(!que.empty()){
			int u = que.front();
			pinq[u] = false;
			que.pop();
			int ee = phead[u], v;
			while(ee != -1){
				v = pedges[ee].pnt;
				if(relax(u, v, pedges[ee].v) && !pinq[v]){
					pinq[v] = true;
					que.push(v);
				}
				ee = pedges[ee].nxt;
			}
		}
	}	
}

const int MaxV = 1010;
const int MaxE = 4010;

int head[MaxV], dist[MaxV];
bool inq[MaxV];
AliasSpfaADL::Edge edges[MaxE];
int nowE = 0;

void addedge(int u, int v, int c){
	edges[nowE].pnt = v, edges[nowE].nxt = head[u];
	edges[nowE].v = c; head[u] = nowE++;
	
	edges[nowE].pnt = u, edges[nowE].nxt = head[v];
	edges[nowE].v = c, head[v] = nowE++;
}

int main(){
	
	int m, n;
	while(scanf("%d %d", &m, &n) != EOF){
		for(int i = 0; i < n; i++){
			head[i] = -1;
		}
		nowE = 0;
		int u, v, c;
		for(int i = 0; i < m; i++){
			scanf("%d %d %d", &u, &v, &c);
			u--, v--;
			addedge(u, v, c);
		}
		AliasSpfaADL::SpfaADL(n, head, dist, inq, edges);
		AliasSpfaADL::solve(0);
		printf("%d\n", dist[n - 1]);
	}
	return 0;
}
