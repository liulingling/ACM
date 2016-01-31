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


const int MaxV = 10010;
const int MaxE = 50010;
int head[MaxV];
AliasTarjan::Edge edges[MaxE];
int nowE = 0;
int dfn[MaxV], low[MaxV], belong[MaxV], stk[MaxV];
bool vis[MaxV], inStk[MaxV];
//
AliasTarjan::Edge edges_r[MaxE];
int head_r[MaxE];

int main(){

	int n, m;
	while(scanf("%d %d", &n, &m) != EOF){
		
		AliasTarjan::tarjan(n, head, edges, dfn, low, belong, stk, vis, inStk);
		AliasTarjan::initMap();
		
		int tmpu, tmpv;
		for(int i = 0; i < m; i++){
			scanf("%d %d", &tmpu, &tmpv);
			AliasTarjan::addEdge(tmpu - 1, tmpv - 1);
		}
		
		AliasTarjan::tarjanInit();
		AliasTarjan::solve();
		bool alone = false;
		for(int i = 0; i < n; i++){
			if(dfn[i] == 0){
				alone = true;
				break;
			}
		}
		if(alone){
			printf("0\n");
			continue;
		}
		int cntE;
		int N_r = AliasTarjan::rebuildMap(head_r, edges_r, cntE);
		int cntzero = 0, point = 0;
		for(int i = 0; i < N_r; i++){
			if(head_r[i] == -1){
				point = i + 1;
				cntzero++;
				if(cntzero > 1)
					break;
			}
		}
		if(cntzero > 1 || cntzero == 0){
			printf("0\n");
		}else{
			int ans = 0;
			for(int i = 0; i < n; i++){
				if(belong[i] == point){
					ans++;
				}
			}
			printf("%d\n", ans); 
		}
	}	
	return 0;	
}
