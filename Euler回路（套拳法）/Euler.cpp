#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;
//套圈法 
const int MaxV = 110;

int adj[MaxV][MaxV], cnt[MaxV];;//邻接表 init
int idx[MaxV][MaxV];
int stk[MaxV];
int top;

int path(int v){
	for(int w; cnt[v] > 0; v = w){
		stk[top++] = v;
		w = adj[v][--cnt[v]];
		//删除v - w 
		adj[w][idx[w][v]] = adj[w][--cnt[w]];
		//删除 w - v(无向图） 
	}
	return v; 
}
void elpath(int b, int n){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < cnt[i]; j++){
			idx[i][adj[i][j]] = j;
		}
	}
	printf("%d", b);
	for(top = 0; path(b) == b && top != 0; ){
		b = stk[--top];
		printf("-%d", b);
	}
	printf("\n");
} 
int main(){
	
	int n, m;
	while(scanf("%d %d", &n, &m) != EOF){
		for(int i = 0; i < n; i++){
			cnt[i] = 0;
		}
		int u, v;
		for(int i = 0; i < m; i++){
			scanf("%d %d", &u, &v);
			adj[u][cnt[u]++] = v;
			adj[v][cnt[v]++] = u; 
		}
		elpath(0, n);
	}
	return 0;
}
 
