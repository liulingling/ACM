#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <string>

using namespace std;
#define LLINT long long

const int MaxV = 51000;
const int MaxE = 200010;

struct Edge{
	int pnt, nxt;
	int value;	
};
int head[MaxV], nowE;
int weight[MaxV];

LLINT dist[MaxV];
bool inq[MaxV];
int stk[MaxV];

Edge edges[MaxE];
int n, m;

void addedge(int u, int v, int c){
	edges[nowE].pnt = v, edges[nowE].value = c;
	edges[nowE].nxt = head[u], head[u] = nowE++;
	
	edges[nowE].pnt = u, edges[nowE].value = c;
	edges[nowE].nxt = head[v], head[v] = nowE++;
}

bool relax(int u, int v, int wi){
	if(dist[v] == -1 || dist[v] > dist[u] + wi){
		dist[v] = dist[u] + wi;
		return true;
	}
	return false;
}

void spfa(int s){
	queue<int> que;
	inq[s] = true;
	dist[s] = 0;
	que.push(s);
	int u, v, ee;
	int top = 1;
	stk[0] = s;
	while(top > 0){
		top--;
		u = stk[top];
		inq[u] = false;
		ee = head[u];
		while(ee != -1){
			v = edges[ee].pnt;
			if(relax(u, v, edges[ee].value) && !inq[v]){
				inq[v] = true;
				stk[top] = v;
				top++;
			}
			ee = edges[ee].nxt;
		}
	}
	/*while(!que.empty()){
		u = que.front();
		que.pop();
		inq[u] = false;
		ee = head[u];
		while(ee != -1){
			v = edges[ee].pnt;
			if(relax(u, v, edges[ee].value) && !inq[v]){
				inq[v] = true;
				que.push(v);
			}
			ee = edges[ee].nxt;
		}
	}*/
	LLINT ans = 0;
	for(int i = 1; i < n; i++){
		if(dist[i] == -1){
			ans = -1;
			break;
		}
		ans += dist[i] * weight[i];
	}
	if(ans == -1){
		printf("No Answer\n");
	}else{
		printf("%lld\n", ans);
	}
}
int main(){
	
	int Tcas;
	scanf("%d", &Tcas);
	while(Tcas--){
		scanf("%d %d", &n, &m);
		for(int i = 0; i < n + 5; i++){
			head[i] = -1;
			inq[i] = false;
			dist[i] = -1;
		}
		nowE = 0;
		for(int i = 0; i < n; i++){
			scanf("%d", &weight[i]); 
		}
		int u, v, wi;
		for(int i = 0; i < m; i++){
			scanf("%d %d %d", &u, &v, &wi);
			u--, v--;
			addedge(u, v, wi);
		}
		
		spfa(0);		
	}
	return 0;
}
