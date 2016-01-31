#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;
#define typec int

const typec inf = 0x3f3f3f3f;
const int MaxV = 210;
int vis[MaxV];
bool inMST[MaxV][MaxV];
typec lowc[MaxV];
typec maxc[MaxV][MaxV];
int pre[MaxV];

typec prim(typec cost[][MaxV], int n){
	int i, j, p;
	typec res = 0, minc;
	memset(vis, 0, sizeof(vis));
	memset(inMST, false, sizeof(inMST));
	memset(maxc, 0, sizeof(maxc));
	
	for(int i = 0; i < n; i++){
		pre[i] = 0; 
	}
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
		
		int anc = pre[p];
		inMST[anc][p] = inMST[p][anc] = true;
		maxc[anc][p] = maxc[p][anc] = minc;
		
		for(j = 0; j < n; j++){
			if(vis[j]){
				maxc[j][p] = maxc[p][j] = max(maxc[j][p], maxc[anc][p]);
			}
		}
		res += minc; vis[p] = 1;
		for(j = 0; j < n; j++){
			if(0 == vis[j] && lowc[j] > cost[p][j]){
				lowc[j] = cost[p][j];
				pre[j] = p;
			}
		}
	}
	return res;
}

void sec_MST(int cost[][MaxV], int n){
	//bool ok = true;
	typec sec_min = inf;
	typec fir_min = prim(cost, n);
	
	for(int i = 0; i < n; i++){
		for(int j = i + 1; j < n; j++){
			if(cost[i][j] != inf && !inMST[i][j]){
				if(sec_min > fir_min + cost[i][j] - maxc[i][j]){
					sec_min = fir_min + cost[i][j] - maxc[i][j];
				}
			}
		}
	}
	if(sec_min == inf){
		
	}else{
		cout<<sec_min<<endl;	
	}
}
int main(){
	
	return 0;
}
