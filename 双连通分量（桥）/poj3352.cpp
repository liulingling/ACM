#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;

namespace AliasSearchBridge{

#include <math.h>

struct Edge{
	int pnt;
	int nxt;
	bool isBridge;	
};
	int *phead, *pdfn, *plow, *pbridges;
	Edge *pedges;
	int N;
	int times;
	int nowE;
	int cntBridges;
	void searchBridgeInit(){
		times = 0;
		cntBridges = 0;
		for(int i = 0; i < N; i++){
			pdfn[i] = 0, plow[i] = 0;
		}
	}
	void initMap(){
		nowE = 0;
		for(int i = 0; i < N; i++){
			phead[i] = -1;
		}
	}
	void addEdge(int u, int v){
		pedges[nowE].isBridge = false;
		pedges[nowE].pnt = v;
		pedges[nowE].nxt = phead[u];
		phead[u] = nowE++;
	}
	void searchBridge(int n, int head[], Edge edges[], int dfn[],
				int low[], int bridges[]){
		phead = head, pedges = edges;
		pdfn = dfn, plow = low;
		pbridges = bridges;
		N = n;
	}
	//for undirected graph 
	void bridgeDfs(int u, int father){
		pdfn[u] = plow[u] = ++times;
		int minc = plow[u];
		int v;
		int ee = phead[u];
		while(ee != -1){
			v = pedges[ee].pnt;
			if(father != v){
				if(pdfn[v] == 0)
					bridgeDfs(v, u);
				if(plow[v] < minc)
					minc = plow[v];
			}
			ee = pedges[ee].nxt;
		}
		if(minc < plow[u])
			plow[u] = minc;
	}
	int solve(){
		for(int i = 0; i < N; i++){ 
			if(pdfn[i] == 0) 
				bridgeDfs(i, -1);
		}
		return cntBridges;
	}	
}


const int MaxV = 5010;
const int MaxE = 20010;

int head[MaxV];
AliasSearchBridge::Edge edges[MaxV];
int dfn[MaxV], low[MaxV];
int bridges[MaxE];


int deg[MaxV];
bool hash[MaxV][MaxV];
int main(){
	
	int n, m;
	while(scanf("%d %d", &n, &m) != EOF){
		AliasSearchBridge::searchBridge(n, head, edges, dfn,
										low, bridges);
		AliasSearchBridge::initMap();
		
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				hash[i][j] = false;
		int u, v;
		for(int i = 0; i < m; i++){
			scanf("%d %d", &u, &v);
			if(hash[u - 1][v - 1])
				continue;
			hash[u - 1][v - 1] = true;
			hash[v - 1][u - 1] = true;
			AliasSearchBridge::addEdge(u - 1, v - 1);
			AliasSearchBridge::addEdge(v - 1, u - 1);
		}
		AliasSearchBridge::searchBridgeInit();
		AliasSearchBridge::solve();
		
		for(int i = 0; i < n; i++){
			deg[i] = 0;
		}
		for(int i = 0; i < n; i++){
			int ee = head[i];
			while(ee != -1){
				int v = edges[ee].pnt;
				if(low[i] != low[v]){
					deg[low[i] - 1]++;
				}
				ee = edges[ee].nxt; 
			}
		}
		int ans = 0;
		for(int i = 0; i < n; i++){
			if(deg[i] == 1)
				ans++;
		}
		printf("%d\n",(ans + 1)/ 2);
	}
	return 0;
}
