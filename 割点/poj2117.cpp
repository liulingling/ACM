#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

namespace AliasCutPoint{

#include <math.h>

struct Edge{
	int pnt;
	int nxt;
};
	int *phead, *pdfn, *plow;
	Edge *pedges;
	int *pflag;
	int N;
	int times, root;
	int nowE;
	void initMap(){
		nowE = 0;
		for(int i = 0; i < N; i++){
			phead[i] = -1;
		}
	}
	void cutPointInit(){
		for(int i = 0; i < N; i++){
			pdfn[i] = 0;
			pflag[i] = 0;
		}
		times = 0;
		root = 0;	
	}
	void addedge(int u, int v){
		pedges[nowE].pnt = v;
		pedges[nowE].nxt = phead[u];
		phead[u] = nowE;
		nowE++;
	}
	void searchCutPoint(int n, int head[], Edge edges[], int dfn[],
					int low[], int flag[]){
		N = n;
		phead = head, pedges = edges;
		pdfn = dfn, plow = low, pflag = flag;
	}
	void cutPointDfs(const int u, int father){
		int v, ee, cnum = 0;
		times++;
		pdfn[u] = plow[u] = times;
		ee = phead[u];
		while(ee != -1){
			v = pedges[ee].pnt;
			if(v != father){
				if(!pdfn[v]){
					cnum++;
					cutPointDfs(v, u);
					plow[u] = min(plow[u], plow[v]);
					if(u == root && cnum >= 2){
						pflag[u] = cnum - 1;
					}
					if(u != root && plow[v] >= pdfn[u]){
						//pflag[u] = true;
						pflag[u]++;
					} 
				}else{
					if(u != v)
						plow[u] = min(plow[u], pdfn[v]);
				}
			}
			ee = pedges[ee].nxt;
		}
	}
	int solve(){
		int tree = 0;
		for(int i = 0; i < N; i++){
			if(!pdfn[i]){
				tree++;
				root = i;
				cutPointDfs(i, -1);
			}
		}
		return tree; 
	}	
}


const int MaxV = 10010;
const int MaxE = 2000010;

int head[MaxV], dfn[MaxV], low[MaxV];
int flag[MaxV];
AliasCutPoint::Edge edges[MaxE];


int main(){
	int P, C;
	
	while(scanf("%d %d", &P, &C) != EOF){
		if(P == 0 && C == 0)
			break;
		AliasCutPoint::searchCutPoint(P, head, edges, dfn, low, flag);
		AliasCutPoint::initMap();
		
		int u, v;
		for(int i = 0; i < C; i++){
			scanf("%d %d", &u, &v);
			AliasCutPoint::addedge(u, v);
			AliasCutPoint::addedge(v, u);
		}
		AliasCutPoint::cutPointInit();
		int trees = AliasCutPoint::solve();
		int maxAns = 0;
		for(int i = 0; i < P; i++){
			maxAns = max(maxAns, flag[i]);
		}
		if(trees != P)
			printf("%d\n", maxAns + trees);
		else
			printf("%d\n", trees - 1);
	}
	
	return 0;
}
