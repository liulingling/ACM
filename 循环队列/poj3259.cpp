#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <string>

using namespace std;
#define LLINT long long

struct Edge{
	int pnt, nxt, value;
};

const int MaxV = 610;
const int MaxE = 6000;

int src, des;
int n, m, w;

int head[MaxV], nowE = 0;
Edge edges[MaxE];
bool inq[MaxV];
int que[MaxV];

LLINT dist[MaxV];
bool hasValue[MaxV];
 
int cntTimes[MaxV];

void addedge(int u, int v, int c){
	edges[nowE].pnt = v, edges[nowE].nxt = head[u];
	edges[nowE].value = c, head[u] = nowE++;
}

bool relax(int u, int v, int c){
	if(!hasValue[v] || dist[v] > dist[u] + c){
		dist[v] = dist[u] + c;
		hasValue[v] = true;
		return true;
	}
	return false;
}

bool spfa(int s){
	for(int i = 0; i <= n; i++){
		inq[i] = false;
		hasValue[i] = false;
		cntTimes[i] = 0;
	}
	int around = 0;
	int front = 0, rear = 0;
	que[0] = s;
	cntTimes[s] = 1;
	inq[s] = true;
	dist[s] = 0;
	hasValue[s] = true;
	int u, v, ee;
	bool hasCircle = false;
	while((around == 0 && front <= rear) || (around == 1 && rear < front)){
		u = que[front];
		inq[u] = false;
		front++;
		if(front >= MaxV){
			around--;
			front -= MaxV;
		}
		ee = head[u];
		while(ee != -1){
			v = edges[ee].pnt;
			if(relax(u, v, edges[ee].value) && !inq[v]){
				inq[v] = true;
				rear++;
				//TODO::
				if(rear >= MaxV){
					around++;
					rear -= MaxV;
				}
				que[rear] = v;
				cntTimes[v]++;
				if(cntTimes[v] > n){
					hasCircle = true;
					break;
				} 
			}
			ee = edges[ee].nxt;
		}
		if(hasCircle)
			break; 
	}
	if(hasCircle){
		return true;
	}else{
		return false;
	}
}
int main(){
	
	int tcas;
	
	while(scanf("%d", &tcas) != EOF){
	while(tcas--){
		scanf("%d %d %d", &n, &m, &w);
		for(int i = 0; i <= n; i++){
			head[i] = -1;
		}
		nowE = 0;
		int u, v, value;
		for(int i = 0; i < m; i++){
			scanf("%d %d %d", &u, &v, &value);
			u--, v--;
			addedge(u, v, value);
			addedge(v, u, value);
		}
		for(int i = 0; i < w; i++){
			scanf("%d %d %d", &u, &v, &value);
			u--, v--;
			addedge(v, u, - value);
		}
		
		bool ans = false;
		for(int i = 0; i < n; i++){
			if(spfa(i)){
				ans = true;
				break;
			}
		}
		if(ans){
			printf("YES\n"); 
		}else{
			printf("NO\n");
		}
	}	
	}
	return 0;
}
