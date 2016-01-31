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
#define maxn 210
#define mod 1000000007

namespace AliasMinMax{
struct Edge{
	int pnt, fnt, nxt, cns, cot;	
};
	int *phead, *pdist, *ppre, *pminflow;
	int *pque, N;
	bool *pinq;
	Edge *pedges;
	void MinMaxFlow(int n, int head[], int dist[], int pre[],
		int minflow[], int que[], Edge edges[], bool inq[]){
		N = n, phead = head, pdist = dist, ppre = pre;
		pminflow = minflow, pque = que, pedges = edges;
		pinq = inq;	
	}
	bool relax(int u, int v, int c, int e){
		if(pdist[u] >= INF)	return false;
		if(pdist[v] > pdist[u] + c){
			ppre[v] = e; pminflow[v] = min(pminflow[u], pedges[e].cns);
			pdist[v] = pdist[u] + c;
			return true;
		}
		return false;
	}
	bool spfa(int s, int t){
		for(int i = 0; i < N; i++){
			pdist[i] = INF;
			pinq[i] = false; ppre[i] = -1;
		}
		pdist[s] = 0; pminflow[s] = INF;
		int front = 0, rear = 0;
		pque[0] = s; pinq[s] = true;
		int u, v, ee;
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
				ee = pedges[ee].nxt;
			}
		}
		return pdist[t] < INF;
	}
	void update(int s, int t, int f){
		int i = t, e;
		while(i != s){
			e = ppre[i];
			pedges[e].cns -= f;
			pedges[e ^ 1].cns += f;
			i = pedges[e].fnt;
		}
	}
	int solve(int s, int t){
		int mincost = 0;
		while(spfa(s, t)){
			mincost += pdist[t] * pminflow[t];
			update(s, t, pminflow[t]);
		}
		return mincost;
	}	
}
struct Interval{
	int lhs, rhs, wg;	
};

Interval ivs[maxn];
int ID[100010];
int N, K;

const int MaxV = 410;
const int MaxE = 1210;
AliasMinMax::Edge edges[MaxE];
int head[MaxV], pre[MaxV], dist[MaxV], minflow[MaxV];
int que[MaxV * MaxV];
bool inque[MaxV];

int nowE;

void addedge(int u, int v, int cnst, int cost){
	edges[nowE].pnt = v, edges[nowE].fnt = u, edges[nowE].nxt = head[u];
	edges[nowE].cns = cnst, edges[nowE].cot = cost; head[u] = nowE++;
	
	edges[nowE].pnt = u, edges[nowE].fnt = v, edges[nowE].nxt = head[v];
	edges[nowE].cns = 0, edges[nowE].cot = -cost; head[v] = nowE++;
}
int main(){
	
	int Tcas = 0;
	while(scanf("%d", &Tcas) != EOF){
		while(Tcas--){
			scanf("%d %d", &N, &K);
			set<int> numList;
			for(int i = 0; i < N; i++){
				scanf("%d %d %d", &ivs[i].lhs, &ivs[i].rhs, &ivs[i].wg);
				numList.insert(ivs[i].lhs);
				numList.insert(ivs[i].rhs);
			}
			int id = 1;
			for(set<int>::iterator it = numList.begin(); it != numList.end(); it++){
				ID[*it] = id;
				id++;
			}
			int s = 0, t = id;	nowE = 0;
			for(int i = 0; i <= t; i++){
				head[i] = -1;
			}
			for(int i = 0; i < t; i++){
				addedge(i, i + 1, K, 0); 
			}
			for(int i = 0; i < N; i++){
				addedge(ID[ivs[i].lhs], ID[ivs[i].rhs], 1, -ivs[i].wg);
			}
			AliasMinMax::MinMaxFlow(t + 1, head, dist, pre, minflow, que, edges, inque);
			int ans = AliasMinMax::solve(s, t);
			printf("%d\n", -ans);	
		}
	}
	return 0;
}
