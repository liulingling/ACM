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
#define INF 0x3fffffff
#define read freopen("in.txt","r",stdin)
#define write freopen("out.txt","w",stdout)
#define maxn 55
#define mod 1000000007

namespace AliasMinMax{
struct Edge{
	int pnt, fnt, nxt, cns, cot;	
};
	int *phead, *pdist, *ppre, *pminflow;
	int *pque, N, *pcnttms;
	bool *pinq;
	Edge *pedges;
	void MinMaxFlow(int n, int head[], int dist[], int pre[],
		int minflow[], int que[], Edge edges[], bool inq[], int cnttms[]){
		N = n, phead = head, pdist = dist, ppre = pre;
		pminflow = minflow, pque = que, pedges = edges;
		pinq = inq, pcnttms = cnttms;	
	}
	bool relax(int u, int v, int c, int e){
		if(pdist[u] >= INF)	return false;
		if(pdist[v] > pdist[u] + c){
			ppre[v] = e; pcnttms[v]++;
			pdist[v] = pdist[u] + c;
			return true;
		}
		return false;
	}
	bool spfa(int s, int t){
		for(int i = 0; i < N; i++){
			pdist[i] = INF; pcnttms[i] = 0;
			pinq[i] = false; ppre[i] = -1;
		}
		pdist[s] = 0; //pminflow[s] = INF;
		int front = 0, rear = 0;
		pque[0] = s; pinq[s] = true; pcnttms[s] = 1; 
		int u, v, ee, cirPoint;
		bool hasCir = false;
		while(front <= rear){
			u = pque[front++]; pinq[u] = false;
			ee = phead[u];
			while(ee != -1){
				v = pedges[ee].pnt;
				if(pedges[ee].cns <= 0){
					ee = pedges[ee].nxt;
					continue;
				}
				if(relax(u, v, pedges[ee].cot, ee) && !pinq[v]){
					pinq[v] = true;
					pque[++rear] = v;
				}
				if(pcnttms[v] > N){
					hasCir = true; cirPoint = v;
					break;
				}
				ee = pedges[ee].nxt;
			}
			if(hasCir)	break;
		}
		if(!hasCir)	return false;
		
		for(int i = 0; i < N; i++)	pinq[i] = false;
		while(pinq[cirPoint] == false){
			pinq[cirPoint] = true;
			cirPoint = pedges[ppre[cirPoint]].fnt;
		}
		
		while(pinq[cirPoint] == true){
			pinq[cirPoint] = false;
			pedges[ppre[cirPoint]].cns--;
			pedges[ppre[cirPoint]^1].cns++;
			cirPoint = pedges[ppre[cirPoint]].fnt;
		}
		return true;
	}
	bool solve(int s, int t){
		return spfa(s, t); 
	}	
}

const int MaxV = 410;
const int MaxE = 80010;
AliasMinMax::Edge edges[MaxE];
int head[MaxV], pre[MaxV], dist[MaxV], minflow[MaxV];
int que[MaxV * MaxV], cnttms[MaxV];
bool inque[MaxV];

int nowE;

void addedge(int u, int v, int cnst, int cnstop, int cost){
	edges[nowE].pnt = v, edges[nowE].fnt = u, edges[nowE].nxt = head[u];
	edges[nowE].cns = cnst, edges[nowE].cot = cost; head[u] = nowE++;
	
	edges[nowE].pnt = u, edges[nowE].fnt = v, edges[nowE].nxt = head[v];
	edges[nowE].cns = cnstop, edges[nowE].cot = -cost; head[v] = nowE++;
}

int getdis(int x1, int y1, int x2, int y2){
	return abs(x1 - x2) + abs(y1 - y2) + 1;
}

int N, M;

struct Build{
	int x, y, c;	
};

Build mb[MaxV], sr[MaxV];
int plan[MaxV][MaxV];
int times[MaxV][MaxV];
int cntSel[MaxV];

int main(){
	while(scanf("%d %d", &N, &M) != EOF){
		for(int i = 0; i < N; i++){
			scanf("%d %d %d", &mb[i].x, &mb[i].y, &mb[i].c);
		}
		for(int j = 0; j < M; j++){
			scanf("%d %d %d", &sr[j].x, &sr[j].y, &sr[j].c);
			cntSel[j] = 0;
		}
		for(int i = 0; i < N; i++){
			for(int j = 0; j < M; j++){
				times[i][j] = getdis(mb[i].x, mb[i].y, sr[j].x, sr[j].y);
			}
		}
		ll sum = 0;
		for(int i = 0; i < N; i++){
			for(int j = 0; j < M; j++){
				scanf("%d", &plan[i][j]);
				sum += plan[i][j] * times[i][j];
				cntSel[j] += plan[i][j];
			}
		}
		int s = 0, t = N + M + 1;
		for(int i = 0; i <= t; i++){
			head[i] = -1;
		}
		nowE = 0;
		for(int i = 0; i < N; i++){
			addedge(s, i + 1, 0, mb[i].c, 0);//正向，反向流量都加 
			for(int j = 0; j < M; j++){
				addedge(i + 1, j + N + 1, INF - plan[i][j], plan[i][j], times[i][j]); 
			}
		}
		for(int j = 0; j < M; j++)
			addedge(j + N + 1, t, sr[j].c - cntSel[j], cntSel[j], 0);

		AliasMinMax::MinMaxFlow(t + 1, head, dist, pre, minflow, que, edges, inque, cnttms);
		bool hasCir = AliasMinMax::solve(t, t);
		if(hasCir){
			printf("SUBOPTIMAL\n");
			for(int i = 1; i <= N; i++){
				int v, ee = head[i];
				while(ee != -1){
					v = edges[ee].pnt;
					if(v > N && v < t){
						plan[i][v - N] = INF - edges[ee].cns;
					}
					ee = edges[ee].nxt;
				}
			}
			for(int i = 1; i <= N; i++){
				for(int j = 1; j <= M; j++){
					printf("%d", plan[i][j]);
					if(j != M)	printf(" ");
					else	printf("\n");
				}
			}
		}else{
			printf("OPTIMAL\n");
		}
	}		
	return 0;
}
