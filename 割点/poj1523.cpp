#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

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


const int MaxV = 1010;
const int MaxE = 2000010;

int head[MaxV], dfn[MaxV], low[MaxV];
int flag[MaxV];
AliasCutPoint::Edge edges[MaxE];


int main(){
	
	int u, v;
	int tcase = 0;
	while(scanf("%d", &u) != EOF){
		if(u == 0)
			break;
		tcase++;
		AliasCutPoint::searchCutPoint(1001, head, edges, dfn, low, flag);
		AliasCutPoint::initMap();
		do{
			scanf("%d", &v);
			AliasCutPoint::addedge(u - 1, v - 1);
			AliasCutPoint::addedge(v - 1, u - 1);
			scanf("%d", &u); 
		}while(u != 0);
		
		AliasCutPoint::cutPointInit();
		int trees = AliasCutPoint::solve();
		bool isZero = true; 
		printf("Network #%d\n", tcase); 
		for(int i = 0; i < 1000; i++){
			if(flag[i] > 0){
				isZero = false;
				printf("  SPF node %d leaves %d subnets\n", i + 1, flag[i] + 1);
			}
		}
		if(isZero){
			printf("  No SPF nodes\n");
		}
		printf("\n");
	}
	return 0;
}
