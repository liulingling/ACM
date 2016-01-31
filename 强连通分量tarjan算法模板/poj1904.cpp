#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;

namespace AliasTarjan{
//HINT: 0 is the first point, n-1 is the last point
struct Edge{
	int nxt;
	int pnt;
};
	int *phead, *pdfn, *plow, *pbelong, *pstk;
	bool *pvis, *pinStk;
	Edge *pedges;
	int N;
	int times;
	int top;
	int bent;
	int nowE;
	void tarjanInit(){
		times = 0;
		top = 0;
		bent = 0;
		for(int i = 0; i < N; i++){
			pdfn[i] = 0, plow[i] = 0, pbelong[i] = i;
			pvis[i] = false, pinStk[i] = false;
		}
	}
	//init the map
	void initMap(){
		nowE = 0;
		for(int i = 0; i < N; i++){
			phead[i] = -1;
		}
	}
	//add one edge to the map
	void addEdge(int u, int v){
		pedges[nowE].pnt = v;
		pedges[nowE].nxt = phead[u];
		phead[u] = nowE;
		nowE++;
	}
	//search for one tree 
	void tarjanDfs(int u){
		int v;
		pdfn[u] = plow[u] = ++times;
		pinStk[u] = true;
		pvis[u] = true;
		pstk[++top] = u;
		int ee = phead[u];
		while(ee != -1){
			v = pedges[ee].pnt;
			if(!pdfn[v]){
				tarjanDfs(v);
				if(plow[v] < plow[u]){
					plow[u] = plow[v];
				}
			}else if(pinStk[v] && pdfn[v] < plow[u]){
				plow[u] = pdfn[v];
			}
			ee = pedges[ee].nxt;
		}
		if(pdfn[u] == plow[u]){
			bent++;
			do{
				v = pstk[top--];
				pinStk[v] = false;
				pbelong[v] = bent;
			}while(v != u);
		}
	}
	//get all kinds of data offset 
	void tarjan(int n, int head[], Edge edges[], int dfn[],
			int low[], int belong[], int stk[], bool vis[], bool inStk[]){
		phead = head, pedges = edges, pdfn = dfn, plow = low;
		pbelong = belong, pstk = stk, pvis = vis, pinStk = inStk;
		N = n;
		tarjanInit();
	}
	//search all trees
	void solve(){
		for(int i = 0; i < N; i++){
			if(!pdfn[i]){
				tarjanDfs(i);
			}
		}
	}
	
	//add one edge to rebuilded map
	void rebuildAddedge(int u, int v, int head_r[], Edge edges_r[], int &nowE_r){
		edges_r[nowE_r].pnt = v;
		edges_r[nowE_r].nxt = head_r[u];
		head_r[u] = nowE_r;
		nowE_r++;
	}
	//rebuild the map base on the original map and pbelong[]
	int rebuildMap(int head_r[], Edge edges_r[], int &nowE_r){
		for(int i = 0; i < bent; i++){
			head_r[i] = -1;
		}
		nowE_r = 0;
		int ee;
		for(int u = 0; u < N; u++){
			ee = phead[u];
			while(ee != -1){
				int v = pedges[ee].pnt;	
				if(pbelong[u] != pbelong[v]){
					rebuildAddedge(pbelong[u] - 1, pbelong[v] - 1, head_r, edges_r, nowE_r);
				}
				ee = pedges[ee].nxt;
			}
		}
		return bent;	
	}
}

const int MaxV = 40010;
const int MaxE = 300010;
int head[MaxV];
AliasTarjan::Edge edges[MaxE];
int nowE = 0;
int dfn[MaxV], low[MaxV], belong[MaxV], stk[MaxV];
bool vis[MaxV], inStk[MaxV];

bool cmp(const int &lsh, const int &rsh){
	return lsh < rsh;
}
int main(){
	
	int n;
	while(scanf("%d", &n) != EOF){
		AliasTarjan::tarjan(2 * n, head, edges, dfn, low, belong, stk, vis, inStk);
		AliasTarjan::initMap();
		
		int girl, k;
		for(int i = 1; i <= n; i++){
			scanf("%d", &k);
			for(int j = 0; j < k; j++){
				scanf("%d", &girl);
				AliasTarjan::addEdge(i - 1, girl + n - 1);
			}
		}
		for(int i = 1; i <= n; i++){
			scanf("%d", &girl);
			AliasTarjan::addEdge(girl + n - 1, i - 1);
		}
		AliasTarjan::tarjanInit();
		AliasTarjan::solve();
		
		int ans[MaxV], ansCnt;
		for(int i = 0; i < n; i++){
			int ee = head[i];
			ansCnt = 0;
			while(ee != -1){
				girl = edges[ee].pnt;
				if(belong[girl] == belong[i])
					ans[ansCnt++] = girl + 1 - n;
				ee = edges[ee].nxt;
			}
			sort(ans, ans + ansCnt, cmp);
			printf("%d", ansCnt);
			for(int i = 0; i < ansCnt; i++){
				printf(" %d", ans[i]);
			}
			printf("\n");
		}
	}
	return 0;
}
