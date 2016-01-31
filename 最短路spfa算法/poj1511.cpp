#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

namespace AliasSpfaADL{

#define LLINT long long
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

const int MaxV = 1000010;
const int MaxE = 1000010;

int head[MaxV], headr[MaxV];
AliasSpfaADL::Edge edges[MaxV], edgesr[MaxV];
LLINT dis[MaxV];
bool inq[MaxV];
int nowE = 0, nowEr = 0;

void addedge(int u, int v, int c){
	edges[nowE].pnt = v, edges[nowE].nxt = head[u];
	edges[nowE].v = c, head[u] = nowE++;
}
void addedger(int u, int v, int c){
	edgesr[nowEr].pnt = v, edgesr[nowEr].nxt = headr[u];
	edgesr[nowEr].v = c, headr[u] = nowEr++;
}

int main(){
	int tcas;
	scanf("%d", &tcas);
	while(tcas--){
		int p, q;
		scanf("%d %d", &p, &q);
		nowE = 0, nowEr = 0;
		for(int i = 0; i < p; i++){
			head[i] = headr[i] = -1;
		}
		int u, v, c;
		for(int i = 0; i < q; i++){
			scanf("%d %d %d", &u, &v, &c);
			u--, v--;
			addedge(u, v, c);
			addedger(v, u, c);
		}
		LLINT ans = 0;
		AliasSpfaADL::SpfaADL(p, head, dis, inq, edges);
		AliasSpfaADL::solve(0);
		for(int i = 1; i < p; i++){
			ans += dis[i];
		}
		AliasSpfaADL::SpfaADL(p, headr, dis, inq, edgesr);
		AliasSpfaADL::solve(0);
		for(int i = 1; i < p; i++){
			ans += dis[i];
		}
		printf("%lld\n", ans);
	}	
	return 0;
}
