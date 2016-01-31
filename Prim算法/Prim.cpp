#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;
#define typec int

const typec inf = 0x3f3f3f3f;
const int MaxV = 210;
int vis[MaxV];
typec lowc[MaxV];

typec prim(typec cost[][MaxV], int n){
	int i, j, p;
	typec res = 0, minc;
	memset(vis, 0, sizeof(vis));
	vis[0] = 1;
	for(i = 1; i < n; i++) lowc[i] =  cost[0][i];
	for(i = 1; i < n; i++){
		minc = inf; p = -1;
		for(j = 0; j < n; j++){
			if(0 == vis[j] && minc > lowc[j]){
				minc = lowc[j]; p = j;
			}
		}
		if(inf == minc)	return -1;
		res += minc; vis[p] = 1;
		for(j = 0; j < n; j++){
			if(0 == vis[j] && lowc[j] > cost[p][j]){
				lowc[j] = cost[p][j];
			}
		}
	}
	return res;
}

int main(){
	
	return 0;
}
