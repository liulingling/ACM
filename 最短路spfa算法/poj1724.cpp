#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

struct Edge{
	int pnt, nxt, c;
	int l;
};
int k, n, r, N, K;

const int MaxV = 1200000;

bool inq[MaxV];
int dist[MaxV];

Edge edges[10010];
int head[110], nowE = 0;

void addedge(int u, int v, int l, int c){
	edges[nowE].pnt = v, edges[nowE].nxt = head[u];
	edges[nowE].c = c; edges[nowE].l = l;
	head[u] = nowE++;
}

bool relax(int u, int v, int l){
	if(dist[v] == -1 || dist[v] > dist[u] + l){
		dist[v] = dist[u] + l;
		return true;
	}
	return false;
}
void spfa(){
	for(int i = 0; i < N; i++){
		inq[i] = false;
		dist[i] = -1;
	}
	dist[0] = 0;
	queue<int> que;
	que.push(0);
	inq[0] = true;
	int su, sv, u, v, cu, cv;
	while(!que.empty()){
		su = que.front();
		que.pop();
		inq[su] = false;
		u = su / K;
		cu = su % K;
		int ee = head[u];
		while(ee != -1){
			v = edges[ee].pnt;
			if(cu + edges[ee].c < K){
				sv = v * K + cu + edges[ee].c;
				if(relax(su, sv, edges[ee].l) && !inq[sv]){
					inq[sv] = true;
					que.push(sv);
				}
			}
			ee = edges[ee].nxt;
		}
	}
}
int main(){
	
	while(scanf("%d", &k) != EOF){
		scanf("%d %d", &n, &r);
		K = k + 1;
		N = n * K;
		for(int i = 0; i < n; i++){
			head[i] = -1;
		}
		nowE = 0;		
		int u, v, l, c;
		for(int i = 0; i < r; i++){
			scanf("%d %d %d %d", &u, &v, &l, &c);
			u--, v--;
			addedge(u, v, l, c);
		}
		spfa();
		int offset = K * (n - 1), j;
		int ans = -1;
		for(int i = 0; i < K; i++){
			j = offset + i;
			if(dist[j] == -1)
				continue;
			if(ans == -1 || dist[j] < ans)
				ans = dist[j];
		}
		printf("%d\n", ans);
	}	
	return 0;
}
