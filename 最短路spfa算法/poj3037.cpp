#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

struct Edge{
	int pnt, nxt;
	double value;	
};

const int MaxV = 105;
const int MaxQL = MaxV * MaxV;

int head[MaxV * MaxV], nowE = 0;
Edge edges[MaxV * MaxV * 4];
double speeds[MaxV][MaxV];
double base[110];
int D[4][2] = {{1, 0},{-1, 0},
				{0, 1},{0, -1}};
int E[MaxV][MaxV];
int sv, r, c, n;

double dist[MaxV * MaxV];
bool inq[MaxV * MaxV];
int q[MaxQL];

void addedge(int u, int v, double c){
	edges[nowE].pnt = v, edges[nowE].value = c;
	edges[nowE].nxt = head[u], head[u] = nowE++;
}

bool relax(int u, int v, double c){
	if(dist[v] < 0 || dist[v] > dist[u] + c){
		dist[v] = dist[u] + c;
		return true;
	}
	return false;
}
void spfa(int s){
	bool around = false;
	int front = 0, rear = 0;
	for(int i = 0; i < n; i++){
		dist[i] = - 1;
		inq[i] = false;
	}
	inq[s] = true;
	q[0] = s;
	dist[0] = 0;
	
	int u, v, ee;
	while((around && rear < front)||(!around && front <= rear)){
		u = q[front];
		inq[u] = false;
		front++;
		//TODO::
		if(front >= MaxQL){
			front -= MaxQL;
			around = false;
		}
		ee = head[u];
		while(ee != -1){
			v = edges[ee].pnt;
			if(relax(u, v, edges[ee].value) && !inq[v]){
				inq[v] = true;
				rear++;
				//TODO::
				if(rear >= MaxQL){
					rear -= MaxQL;
					around = true;
				}
				q[rear] = v;
			}
			ee = edges[ee].nxt;
		}
	}
	printf("%.2lf\n", dist[n - 1]);
}
int main(){
	base[50] = 1;
	for(int i = 49; i >= 0; i--){
		base[i] = base[i + 1] * 0.5;
	}
	for(int i = 51; i <= 100; i++){
		base[i] = base[i - 1] * 2;
	}
	
	
	while(scanf("%d %d %d", &sv, &r, &c) != EOF){
		int dlt = 0;
		for(int i = 0; i < r; i++){
			for(int j = 0; j < c; j++){
				scanf("%d", &E[i][j]);
				dlt = E[0][0] - E[i][j];
				speeds[i][j] = sv * base[50 + dlt];
			}
		}
		n = r * c;
		for(int i = 0; i < n; i++){
			head[i] = -1;
		}
		nowE = 0;
		int u, v;
		int x, y;
		for(int i = 0; i < r; i++){
			for(int j = 0; j < c; j++){
				u = i * c + j;
				for(int k = 0; k < 4; k++){
					x = i + D[k][0];
					y = j + D[k][1];
					if(x >= 0 && x < r && y >= 0 && y < c){
						v = x * c + y;
						addedge(u, v, 1 / speeds[i][j]);
					}	
				}
			}
		}
		spfa(0);
	} 
	return 0;
}
