#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;


namespace AliasKM{
//HINT: M must >= N	
#include <math.h>

const int inf = -0x3fffffff;
const int INF = 0x3fffffff;
const int MaxV = 250;

	int *plx, *ply, *pmatch, *pslack;
	int (*pw)[MaxV];
	bool *pvx, *pvy;
	int N, M;
	
	void KMInit(){
		int NM = max(N, M);
		for(int i = 0; i < NM; i++){
			pmatch[i] = -1;
			ply[i] = 0;
			plx[i] = 0;
		}
		for(int i = 0; i < N; i++){
			for(int j = 0; j < M; j++){
				if(pw[i][j] > plx[i])
					plx[i] = pw[i][j]; 
			}
		}
	}
	bool KMDfs(int u){
		pvx[u] = true;
		for(int v = 0; v < M; v++){
			if(pvy[v])
				continue;
			int t = plx[u] + ply[v] - pw[u][v];
			if(t == 0){
				pvy[v] = true;
				if(pmatch[v] == -1 || KMDfs(pmatch[v])){
					pmatch[v] = u;
					return true;
				}
			}else if(pslack[v] > t){
				pslack[v] = t;
			}
		}
		return false;
	}
	void KM(int n ,int m, int lx[], int ly[], int w[][MaxV],
		int match[], int slack[], bool vx[], bool vy[]){
		N = n, M = m;
		plx = lx, ply = ly, pw = w;
		pmatch = match, pslack = slack;
		pvx = vx, pvy = vy;
	}
	int solve(){
		for(int x = 0; x < N; x++){
			for(int y = 0; y < M; y++)
				pslack[y] = INF;
			while(1){
				for(int i = 0; i < M; i++)
					pvx[i] = false;
				for(int j = 0; j < M; j++)
					pvy[j] = false;
				if(KMDfs(x))
					break;
				int d = INF;
				for(int j = 0; j < M; j++)
					if(!pvy[j] && d > pslack[j])
						d = pslack[j];
				for(int i = 0; i < N; i++)
					if(pvx[i]) 
						plx[i] -= d;
						
				for(int j = 0; j < M; j++)
					if(pvy[j]) ply[j] += d;
					else pslack[j] -= d;
			}
		}
		
		int ret = 0;
		for(int j = 0; j < M; j++){
			if(pmatch[j] != -1){
				ret += pw[pmatch[j]][j];	
			}
		}
		return ret;
	}
	
}


const int MaxV = AliasKM::MaxV;
int n, m, lx[MaxV], ly[MaxV], w[MaxV][MaxV];
int match[MaxV], slack[MaxV]; 
bool vx[MaxV], vy[MaxV];

int supSo[MaxV][MaxV], canSo[MaxV][MaxV];
int ans, sum = 0;

bool vis[MaxV];

void backTrack(int pos, int value){
	
	if(value < ans)	return ;
	if(pos >= n){
		if(value == ans){
			sum++;
			printf("Best Pairing %d\n", sum);
			for(int i = 0; i < n; i++){
				printf("Supervisor %d with Employee %d\n", i + 1, match[i] + 1);
			}
			return;
		}
		return;
	}
	
	for(int i = 0; i < n; i++){
		if(vis[i] == false){
			match[pos] = i;
			vis[i] = true;
			value += w[pos][i];
			backTrack(pos + 1, value);
			value -= w[pos][i];
			match[pos] = -1;
			vis[i] = false;
		}
	}
	return;
}
//题目输入两个矩阵反了
//还有  要输出说有的可能匹配 题目没说清楚 
int main(){
	
	int tcas = 0;
	scanf("%d", &tcas);
	for(int ks = 1; ks <= tcas; ks++){
		scanf("%d", &n);
		
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				scanf("%d", &canSo[i][j]);
			}
		}
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				scanf("%d", &supSo[i][j]);
			}
		}
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				w[i][j] = 0;
			}
		}
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				w[i][supSo[i][j] - 1] += -j;
				w[canSo[i][j] - 1][i] += -j;
			}
		}
		AliasKM::KM(n, n, lx, ly, w, match, slack, vx, vy);
		AliasKM::KMInit();
		ans = AliasKM::solve();
		
		for(int i = 0; i < n; i++){
			match[i] = -1;
			vis[i] = false;
		}
		printf("Data Set %d, Best average difference: %lf\n", ks, -ans / (2.0 * n));
		backTrack(0, 0);
		sum = 0;
		if(ks != tcas)
			putchar(10);
	}
	
	return 0;
}
