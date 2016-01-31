#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;
namespace AliasBiMatch{

struct Edge{
	int pnt;
	int nxt;
};
	
	int *pvis, *pmatch, *phead;
	Edge *pedges;
	int gN, gM, nowE;
	int nowTimes = 0;
	void initMap(){
		nowE = 0;
		for(int i = 0; i < gN; i++){
			phead[i] = -1;
		}
	}
	void addedge(int u, int v){
		pedges[nowE].pnt = v;
		pedges[nowE].nxt = phead[u];
		phead[u] = nowE;
		nowE++;
	}
	void biMatchInit(){
		nowTimes = 0;
		for(int i = 0; i < gM; i++){
			pmatch[i] = -1;
			pvis[i] = 0;
		}
	}
	void biMatch(int gn, int gm, Edge edges[],
				int head[], int vis[], int match[]){
		gN = gn, gM = gm, phead = head;
		pvis = vis, pmatch = match, pedges = edges;
	}
	bool biMatchDfs(int u){
		int v, ee = phead[u];
		while(ee != -1){
			v = pedges[ee].pnt;
			if(pvis[v] != nowTimes){
				pvis[v] = nowTimes;
				if(pmatch[v] == -1 ||
					biMatchDfs(pmatch[v])){
					pmatch[v] = u;
					return true;	
				}
			}
			ee = pedges[ee].nxt;
		}
		return false;
	}
	int solve(){
		int i, num = 0;
		for(int i = 0; i < gN; i++){
			nowTimes++;
			if(biMatchDfs(i)){
				num++;
			}
		}
		return num; 
	}	
}
const int MaxV = 2010;
const int MaxE = 20010;

int vis[MaxV], match[MaxV], head[MaxV];
AliasBiMatch::Edge edges[MaxE];

char mat[50][20];

int main(){
	int tcase;
	scanf("%d", &tcase);
	for(int i = 0; i < tcase; i++){
		int h, w;
		scanf("%d %d", &h, &w);
		for(int i = 0; i < h; i++){
			scanf("%s", mat[i]);
		}
		AliasBiMatch::biMatch(h * w, h * w, edges, head, vis, match);
		AliasBiMatch::initMap();
		
		int cntInterest = 0;
		for(int ui = 0; ui < h; ui++){
			for(int uj = 0; uj < w; uj++){
				if(mat[ui][uj] == 'o')
					continue;
				cntInterest++;
				int idu = ui * w + uj; 
				if(uj + 1 < w && mat[ui][uj + 1] == '*'){
					int idv = idu + 1;
					AliasBiMatch::addedge(idu, idv);
					AliasBiMatch::addedge(idv, idu);
				}
				if(ui + 1 < h && mat[ui + 1][uj] == '*'){
					int idv = idu + w;
					AliasBiMatch::addedge(idu, idv);
					AliasBiMatch::addedge(idv, idu);
				}
			}
		}
		AliasBiMatch::biMatchInit();
		int matchNums = AliasBiMatch::solve();		
		printf("%d\n", cntInterest - matchNums / 2);
	}
	return 0;	
}
