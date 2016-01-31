#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <algorithm>

const int MaxV = 100010;
int pnt[MaxV * 2], nxt[MaxV * 2], head[MaxV * 2];
int value[MaxV * 2], fnt[MaxV * 2];

int nowE;
bool visit[MaxV];
int id;
int dep[2 * MaxV + 10], E[2 * MaxV + 10], R[MaxV];//R 第一次出现的位置, E和R互为逆运算
int pre[MaxV], edges[MaxV], cntEdges;

void dfs(int u, int d);
int Query(int x, int y);

int d[25], st[2 * MaxV + 1][25];

bool cmp(const int &lhs, const int &rhs){
    return lhs < rhs;
}
void Answer(void){
    int i, Q;
	scanf("%d", &Q);
	int prex, prey;
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

        cntEdges = 0;
        prex = pre[x];
        while(x != lca){
            edges[cntEdges++] = value[prex];
            x = fnt[prex];
            prex = pre[x];
        }
        prey = pre[y];
        while(y != lca){
            edges[cntEdges++] = value[prey];
            y = fnt[prey];
            prey = pre[y];
        }
        std::sort(edges, edges + cntEdges, cmp);
        bool ok = false;
        for(int k = 2; k < cntEdges; k++){
            if(edges[k - 1] + edges[k - 2] > edges[k]){
                ok = true;
                break;
            }
        }
        if(ok){
            printf("Yes\n");
        }else{
            printf("No\n");
        }
	}
}
/*void dfs(int u, int d){
	visit[u] = true;
	R[u] = id; E[id] = u; dep[id++] = d;
	for(int i = head[u]; i != -1; i = nxt[i]){
		if(!visit[pnt[i]]){
			dfs(pnt[i], d + 1);
			E[id] = u; dep[id++] = d;
			pre[pnt[i]] = i;//记录是由那一条边过来的
		}
	}
}*/
void dfs(int u, int d){
	visit[u] = true;
	//sum[u] = nowc;
	R[u] = id; E[id] = u; dep[id++] = d;
	for(int i = head[u]; i != -1; i = nxt[i]){
		if(visit[pnt[i]] == false){
			dfs(pnt[i], d + 1);
			E[id] = u; dep[id++] = d;
			pre[pnt[i]] = i;
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
	for(d[0] = 1, i = 1; i < 25; i++) d[i] = d[i - 1] * 2;
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
	pnt[nowE] = v, nxt[nowE] = head[u], fnt[nowE] = u;
	value[nowE] = c, head[u] = nowE++;
}

int main(){
    int T, N;
    scanf("%d", &T);
    for(int tcas = 1; tcas <= T; tcas++){
        scanf("%d", &N);
        for(int i = 0; i <= N; i++){
            head[i] = -1;
            pre[i] = -1;
            visit[i] = false;
        }
        nowE = 0;
        int u, v, c;
        for(int i = 1; i < N; i++){
            scanf("%d %d %d", &u, &v, &c);
            u--, v--;
            addedge(u, v, c);
            addedge(v, u, c);
        }
        id = 0;
		dfs(0, 0);
		initrmp();
		printf("Case #%d:\n", tcas);
		Answer();
    }
    return 0;
}
