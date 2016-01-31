#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;

namespace AliasEKADL{
#include <math.h>
const int INF = 100000000;

struct Edge{
	int pnt, nxt, ope;
	int fnt, flow;
};

	int *phead, *ppre, *pminflow, *pque;
	Edge *pedges;
	int N, nowE, ans;
	
	void initMap(){
		for(int i = 0; i < N; i++){
			phead[i] = -1;
		}
		nowE = 0;
	}
	void addedge(int u, int v, int f){
		pedges[nowE].pnt = v, pedges[nowE].flow = f;
		pedges[nowE].nxt = phead[u], pedges[nowE].ope = nowE + 1;
		phead[u] = nowE, pedges[nowE].fnt = u;
		nowE++;
		//·´Ïò±ß 
		pedges[nowE].pnt = u, pedges[nowE].flow = 0;
		pedges[nowE].nxt = phead[v], pedges[nowE].ope = nowE - 1;
		phead[v] = nowE, pedges[nowE].fnt = v;
		nowE++;
	}
	void EKADLInit(){
		ans = 0;
	}
	void update(int s, int t, int f){
		int i = t;
		while(i != s){
			int e = ppre[i];
			pedges[e].flow -= f;
			int ope = pedges[e].ope;
			pedges[ope].flow += f;
			i = pedges[e].fnt;
		}
	}
	bool EKADLBfs(int s, int t){
		int front = 0, rear = 0;
		pque[0] = s;
		for(int i = 0; i < N; i++){
			ppre[i] = -1;
		}
		pminflow[s] = INF;
		while(front <= rear){
			int u = pque[front];
			front++;
			int ee = phead[u];
			while(ee != -1){
				int v = pedges[ee].pnt;
				if(ppre[v] == -1 && pedges[ee].flow > 0){
					ppre[v] = ee;
					pminflow[v] = min(pminflow[u], pedges[ee].flow);
					pque[++rear] = v;
					if(v == t)	return true;
				}
				ee = pedges[ee].nxt;
			}
		}
		return false;		
	}
	void EKADL(int n, int head[], Edge edges[], int pre[], int que[],
			int minflow[]){
		N = n, phead = head, pedges = edges;
		ppre = pre, pque = que, pminflow = minflow;
	}
	int solve(int s, int t){
		while(EKADLBfs(s, t)){
			ans += pminflow[t];
			update(s, t, pminflow[t]);
		}
		return ans;
	}
}

const int MaxV = 600;
const int MaxE = 20000;
const int INF = AliasEKADL::INF;
int head[MaxV], pre[MaxV], minflow[MaxV], que[MaxV * 2];
AliasEKADL::Edge edges[MaxE];

int eid[MaxV];

int main(){
	
	int n, s, t;
	while(scanf("%d %d %d", &n, &s, &t) != EOF){
		s--, t--;
		int totalNums = 2 * n;
		AliasEKADL::EKADL(totalNums, head, edges, pre, que, minflow);
		AliasEKADL::initMap();
		int mat[MaxV][MaxV];
		for(int i = 0; i < n; i++){
			//C[i][i + n] = 1;
			if(i == s || i == t){
				AliasEKADL::addedge(i, i + n, INF);
				eid[i] = AliasEKADL::nowE - 2;
			}else{
				AliasEKADL::addedge(i, i + n, 1);
				eid[i] = AliasEKADL::nowE - 2;
			}
			for(int j = 0; j < n; j++){
				scanf("%d", &mat[i][j]);
				if(mat[i][j] == 1){
					if(j == s)
						continue;
					if(i == t)
						continue;
					//C[i + n][j] = 1;
					AliasEKADL::addedge(i + n, j, INF);
				}
			} 
		}
		AliasEKADL::addedge(s, s + n, INF);
		AliasEKADL::addedge(t, t + n, INF);
		if(mat[s][t] == 1){
			printf("NO ANSWER!\n");
			continue;
		}
		AliasEKADL::EKADLInit();
		int ans = AliasEKADL::solve(s, t + n);
		printf("%d\n", ans);
		if(ans == 0){
			continue;
		}
		for(int i = 0; i < n; i++){
			if(i == s || i == t){
				continue;
			}
			if(edges[eid[i]].flow != 0)
				continue;
			bool flag = AliasEKADL::EKADLBfs(i, s);
			if(flag == false)
				continue;
			else
				AliasEKADL::update(i, s, 1);
			flag = AliasEKADL::EKADLBfs(t + n, i + n);
			if(flag == false)
				continue;
			else
				AliasEKADL::update(t + n, i + n, 1);
			edges[eid[i]].flow = 0;
			edges[edges[eid[i]].ope].flow = 0;
			if(!AliasEKADL::EKADLBfs(s, t + n)){
				ans--;
				if(ans == 0){
					printf("%d\n", i + 1);
					break;
				}else{
					printf("%d ", i + 1);
				}	
			}else{
				edges[eid[i]].flow = 1;
				AliasEKADL::EKADLBfs(s, t + n);
				AliasEKADL::update(s, t + n, 1);
			}
		}
		
	}
	return 0;
}
