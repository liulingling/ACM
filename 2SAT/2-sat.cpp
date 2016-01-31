#include <map>
#include <set>
#include <list>
#include <queue>
#include <stack>
#include <cmath>
#include <ctime>
#include <vector>
#include <bitset>
#include <cstdio>
#include <string>
#include <numeric>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;
typedef long long ll;
#define Exp 1e-8
#define INF 0x7fffffff
#define read freopen("in.txt","r",stdin)
#define write freopen("out.txt","w",stdout)
#define maxn 55
#define mod 1000000007

namespace Alias2SAT{
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

const int MaxV = 1010;
const int MaxE = 4000010;
int head[MaxV];
Alias2SAT::Edge edges[MaxE];
int nowE = 0;
int dfn[MaxV], low[MaxV], belong[MaxV], stk[MaxV];
bool vis[MaxV], inStk[MaxV];

int info[MaxV][2];

int main(){
	int n, m;
	while(scanf("%d %d", &n, &m) != EOF){
		bool ok = true;
		for(int i = 0; i < m; i++){
			scanf("%d %d", &info[i][0], &info[i][1]);
			if(info[i][0] < 0 || info[i][1] >= n) ok = false;
			if(info[i][1] < 0 || info[i][1] >= n) ok = false;
			if(info[i][0] > info[i][1])
				swap(info[i][0], info[i][1]);
		}
		Alias2SAT::tarjan(2 * m, head, edges, dfn, low, belong, stk, vis, inStk);		
		Alias2SAT::initMap();
		for(int i = 0; i < m; i++){
			for(int j = i + 1; j < m; j++){
				if(info[j][0] > info[i][0] && info[j][0] < info[i][1] && info[j][1] > info[i][1]
					|| info[j][1] > info[i][0] && info[j][1] < info[i][1] && info[j][0] < info[i][0]){
					Alias2SAT::addEdge(i, j + m);
					Alias2SAT::addEdge(j + m, i);
					Alias2SAT::addEdge(j, i + m);
					Alias2SAT::addEdge(i + m, j);
				}
			}
		}
		Alias2SAT::tarjanInit();
		Alias2SAT::solve();
		
		for(int i = 0; i < m; i++){
			if(belong[i] == belong[i + m])
			{ok = false; break;}
		}
		if(ok)	printf("panda is telling the truth...\n");
		else	printf("the evil panda is lying again\n");
	}
	return 0;
}
