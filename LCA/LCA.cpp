#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

const int MaxV = 100010;
int pnt[MaxV], nxt[MaxV], head[MaxV];
int e;
bool visit[MaxV];
int id;
int dep[2 * MaxV + 1], E[2 * MaxV + 1], R[MaxV];

void dfs(int u, int d);
int Query(int x, int y);

int d[20], st[2 * MaxV + 1][20];

void Answer(void){
	int i, Q;
	scanf("%d", &Q);
	for(i = 0; i < Q; i++){
		int x, y;
		scanf("%d %d", &x, &y);
		x = R[x], y = R[y];
		if(x > y){
			int tmp = x; x = y; y = tmp;
		}
		printf("%d\n", E[Query(x, y)]);
	}
}
void dfs(int u, int d){
	visit[u] = true;
	R[u] = id; E[id] = u; dep[id++] = d;
	for(int i = head[u]; i != -1; i = nxt[i]){
		if(!visit[pnt[i]]){
			dfs(pnt[i], d + 1);
			E[id] = u; dep[id++] = d;
		}
	}
}
int Query(int x, int y){
	int k;
	k = int(log(double(y - x + 1)) / log(2.0));
	return dep[st[x][k]] > dep[st[y - d[k] + 1][k]] ? st[y - d[k] + 1][k]:st[x][k];	
}

void initrmp(const int &id){
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
int main(){
	
	return 0;
}
