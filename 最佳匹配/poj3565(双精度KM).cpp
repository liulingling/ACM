#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;
namespace AliasKM{
//HINT: M must >= N	
#include <math.h>

const int inf = -0x3fffffff;
const int INF = 0x3fffffff;
const int MaxV = 110;
const double eps = 0.000001;

	double *plx, *ply, *pslack;
	int *pmatch;
	double (*pw)[MaxV];
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
			double t = plx[u] + ply[v] - pw[u][v];
			if(fabs(t) <= eps){
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
	void KM(int n ,int m, double lx[], double ly[], double w[][MaxV],
		int match[], double slack[], bool vx[], bool vy[]){
		N = n, M = m;
		plx = lx, ply = ly, pw = w;
		pmatch = match, pslack = slack;
		pvx = vx, pvy = vy;
	}
	double solve(){
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
				double d = INF;
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
		
		double ret = 0;
		for(int j = 0; j < M; j++){
			if(pmatch[j] != -1){
				ret += pw[pmatch[j]][j];	
			}
		}
		return ret;
	}
	
}


const int MaxV = AliasKM::MaxV;
int n, m;
double lx[MaxV], ly[MaxV];
double w[MaxV][MaxV];
int match[MaxV];
double slack[MaxV]; 
bool vx[MaxV], vy[MaxV];

struct Point{
	int x, y;
};

Point pant[MaxV];
Point ptree[MaxV];

double mydistance(const Point &lhs, const Point &rhs){
	return sqrt((lhs.x - rhs.x) * (lhs.x - rhs.x) + 
		(lhs.y - rhs.y) * (lhs.y - rhs.y) + 0.0);
	return 0;
}
int main(){
	
	int n;
	while(scanf("%d", &n) != EOF){
		for(int i = 0; i < n; i++){
			scanf("%d %d", &pant[i].x, &pant[i].y);
		}
		for(int i = 0; i < n; i++){
			scanf("%d %d", &ptree[i].x, &ptree[i].y);
		}
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				w[j][i] = - mydistance(pant[i], ptree[j]);
			}
		}
		AliasKM::KM(n, n, lx, ly, w, match, slack, vx, vy);
		AliasKM::KMInit();
		AliasKM::solve();
		for(int i = 0; i < n; i++){
			printf("%d\n", match[i] + 1);
		}
	}
	return 0;
}
