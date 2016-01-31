#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

const int MaxV = 40010;
int pnt[MaxV * 2], nxt[MaxV * 2], head[MaxV * 2];
int value[MaxV * 2];
int sum[MaxV];

int nowE;
bool visit[MaxV];
int id;
int dep[2 * MaxV + 1], E[2 * MaxV + 1], R[MaxV];

void dfs(int u, int d, int nowc);
int Query(int x, int y);

int d[20], st[2 * MaxV + 1][20];

void Answer(void){
	int i, Q;
	scanf("%d", &Q);
	for(i = 0; i < Q; i++){
		int x, y;
		int px, py;
		scanf("%d %d", &x, &y);
		x--, y--;
		px = R[x], py = R[y];
		if(px > py){
			int tmp = px; px = py; py = tmp;
		}
		int lca = E[Query(px, py)];
		printf("%d\n", sum[x] + sum[y] - 2 * sum[lca]);
	}
}
void dfs(int u, int d, int nowc){
	visit[u] = true;
	sum[u] = nowc;
	R[u] = id; E[id] = u; dep[id++] = d;
	for(int i = head[u]; i != -1; i = nxt[i]){
		if(visit[pnt[i]] == 0){
			dfs(pnt[i], d + 1, nowc + value[i]);
			E[id] = u; dep[id++] = d;
		}
	}
}
int Query(int x, int y){
	int k;
	k = int(log(double(y - x + 1)) / log(2.0));
	return dep[st[x][k]] > dep[st[y - d[k] + 1][k]] ? st[y - d[k] + 1][k]:st[x][k];	
}

void initrmp(){
	int i, j;
	for(d[0] = 1, i = 1; i < 20; i++) d[i] = d[i - 1] * 2;
	for(i = 0; i < id; i++)	st[i][0] = i;
	int k = int(log(id + 0.0) / log(2.0)) + 1;
	for(j = 1; j < k; j++){
		for(i = 0; i < id; i++){
			if(i + d[j] - 1 >= id)
				break;
			st[i][j] = dep[st[i][j - 1]] > 
				dep[st[i + d[j - 1]][j - 1]] ? st[i + d[j - 1]][j - 1] : st[i][j - 1];
		}
	}
}

void addedge(int u, int v, int c){
	pnt[nowE] = v, nxt[nowE] = head[u];
	value[nowE] = c, head[u] = nowE++;
}
int main(){
	
	int n, m;
	while(scanf("%d %d", &n, &m) != EOF){
		for(int i = 0; i < n; i++){
			head[i] = -1;
			visit[i] = false;
		}
		nowE = 0;
		int u, v, c;
		char dc;
		for(int i = 0; i < m; i++){
			scanf("%d %d %d %c", &u, &v, &c, &dc);
			u--, v--;
			addedge(u, v, c);
			addedge(v, u, c);
		}
		id = 0;
		dfs(0, 0, 0);
		
		/*for(int i = 0; i < n; i++){
			cout<<sum[i]<<endl;
		}*/
		initrmp();
		Answer();
	}
	return 0;
}
