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
const int MaxV = 2550;

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
int a[MaxV][MaxV], match[MaxV], slack[MaxV]; 
bool vx[MaxV], vy[MaxV];

void init()  
{  
    scanf("%d%d",&n,&m);  
    for(int i = 0; i < n; i++)  
    	for(int j = 0; j < m; j++) 
			scanf("%d",&a[i][j]);  
  
    for (int i=0; i<n; i++)  
      for (int j=0; j<m; j++)  
        for (int k=1; k<=n; k++)  
        {  
            w[i][j+(k-1)*m]= -a[i][j]*k;  
        }
		  
    m = n*m;
}  
int main(){
	//这个题构图很巧妙
	//拆点很值得推敲 
	int cas;  
    scanf("%d",&cas);  
    while (cas--)  
    {  
        init();
        AliasKM::KM(n, m, lx, ly, w, match, slack, vx, vy);
        AliasKM::KMInit();
        int ans = AliasKM::solve();
		printf("%.6f\n",-ans/(n*1.0));  
    } 
	return 0;
}
