#include <map>
#include <set>
#include <list>
#include <queue>
#include <stack>
#include <cmath>
#include <ctime>
#include <vector>
#include <bitset>
#include <cstdio>
#include <string>
#include <numeric>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;
typedef long long ll;
#define Exp 1e-8
#define INF 0x7fffffff
#define read freopen("in.txt","r",stdin)
#define write freopen("out.txt","w",stdout)
#define maxn 610
#define mod 1000000007

struct Node{
	int pre;
	int ope;
	int nums;	
};

Node node[maxn];
int n, p1, p2;

int find(int x){
	if(x == node[x].pre)
		return x;
	int fa = find(node[x].pre);
	node[x].pre = fa;
	return fa;
}

void merge(int u, int v, int sign){
	int fu = find(u);
	int fv = find(v);
	int f, opf;
	int opu = node[fu].ope, opv = node[fv].ope;
	if(opu != -1)
		opu = find(opu);
	if(opv != -1)
		opv = find(opv);
	node[fu].ope = opu, node[fv].ope = opv;
	if(sign == 0){//u, v belong the same set
		if(fu != fv){
			if(node[fu].nums > node[fv].nums){
				node[fu].nums += node[fv].nums;
				node[fv].pre = fu; f = fu;
			}else{
				node[fv].nums += node[fu].nums;
				node[fu].pre = fv; f = fv;
			}
			if(opu == -1 && opv == -1){
				opf = max(opu, opv);
			}else{
				if(opv != opu){
					if(node[opu].nums > node[opv].nums){
						node[opu].nums += node[opv].nums;
						node[opv].pre = opu; opf = opu;
					}else{
						node[opv].nums += node[opu].nums;
						node[opu].pre = opv; opf = opv; 
					}
				}else{
					opf = opv;
				}
			}
			if(opf != -1){
				node[f].ope = opf;
				node[opf].ope = f;
			}
		}
	}else{// fu fv belong different set 
		int f1 = fu, f2 = fv;
		if(opv != -1 && fu != opv){
			if(node[fu].nums > node[opv].nums){
				node[fu].nums += node[opv].nums;
				node[opv].pre = fu; f1 = fu;
			}else{
				node[opv].nums += node[fu].nums;
				node[fu].pre = opv; f1 = opv;
			}
		}
		if(opu != -1 && fv != opu){
			if(node[fv].nums > node[opu].nums){
				node[fv].nums += node[opu].nums;
				node[opu].pre = fv; f2 = fv;
			}else{
				node[opu].nums += node[fv].nums;
				node[fv].pre = opu; f2 = opu;
			} 
		}
		node[f1].ope = f2;
		node[f2].ope = f1;
	}
}

int a[maxn], b[maxn], c[maxn];
int cntab = 0, cntc = 0;
int dp[maxn][maxn];

bool flag[maxn];

int main(){
	while(scanf("%d %d %d", &n, &p1, &p2) != EOF){
		if(n == 0 && p1 == 0 && p2 == 0) break;
		for(int i = 1; i <= p1 + p2; i++){
			node[i].pre = i; node[i].ope = -1;
			node[i].nums = 1;
		}
		int id1, id2;
		char buffer[32];
		for(int i = 1; i <= n; i++){
			scanf("%d %d %s", &id1, &id2, buffer);
			if(id1 == id2)	continue;
			if(buffer[0] == 'n'){
				merge(id1, id2, 1);
			}else{
				merge(id1, id2, 0);
			}
		}
		for(int i = 1; i <= p1 + p2; i++) find(i);
		for(int i = 1; i <= p1 + p2; i++)
			if(node[i].pre == i && node[i].ope != -1) node[i].ope = node[node[i].ope].pre; 
		cntab = 0, cntc = 0;
		for(int i = 1; i <= p1 + p2; i++){
			if(node[i].pre == i){
				if(node[i].ope == -1){
					c[cntc++] = i;
				}else{
					if(node[i].ope > i){
						a[cntab] = i;
						b[cntab++] = node[i].ope;
					}
				}
			} 
		}
		for(int i = 0; i <= p1 + p2; i++)
			dp[0][i] = -1;
		dp[0][0] = 0;
		int *dpx, *dpy;
		dpx = dp[0];
		
		int d1, d2, cd1, cd2;
		for(int i = 0; i < cntab; i++){
			d1 = a[i], d2 = b[i];
			cd1 = node[d1].nums, cd2 = node[d2].nums;
			dpy = dp[i + 1];
			for(int j = 0; j <= p1 + p2; j++) dpy[j] = -1;
			
			for(int j = 0; j <= p1 + p2; j++){
				if(dpx[j] == -1) continue;
				if(j + cd1 <= p1 + p2){
					if(dpy[j + cd1] == -1){
						dpy[j + cd1] = d1;
					}else{
						dpy[j + cd1] = -2;
					}
					if(dpx[j] == -2)
						dpy[j + cd1] = -2; 
				}
				if(j + cd2 <= p1 + p2){
					if(dpy[j + cd2] == -1){
						dpy[j + cd2] = d2;
					}else{
						dpy[j + cd2] = -2;
					}
					if(dpx[j] == -2)
						dpy[j + cd2] = -2;
				}
			}
			dpx = dpy;
		}
		if(dpx[p1] == -2 || dpx[p2] == -2){
			printf("no\n");
			continue;
		}
		if(dpx[p1] != -1 || dpx[p2] != -1){
			if(dpx[p1] != -1){
				for(int i = 0; i <= p1 + p2; i++) flag[i] = false;
				int nowpos = p1, i = cntab;
				while(nowpos > 0){
					flag[dp[i][nowpos]] = true;
					nowpos -= node[dp[i][nowpos]].nums;
					i--; 
				}
			}else{
				for(int i = 0; i <= p1 + p2; i++) flag[i] = true;
				int nowpos = p2, i = cntab;
				while(nowpos > 0){
					flag[dp[i][nowpos]] = false;
					nowpos -= node[dp[i][nowpos]].nums;
					i--; 
				}
			}
			for(int i = 1; i <= p1 + p2; i++)
				if(flag[node[i].pre])
					printf("%d\n", i);
			printf("end\n");
			continue;
		}
		for(int i = cntab + 1; i <= cntab + cntc; i++){
			dpy = dp[i];
			d1 = c[i - cntab - 1];
			cd1 = node[d1].nums;
			for(int j = 0; j <= p1 + p2; j++)dpy[j] = -1;
			for(int j = 0; j <= p1 + p2; j++){
				if(dpx[j] == -1) continue;
				if(j + cd1 <= p1 + p2){
					if(dpy[j + cd1] == -1){
						dpy[j + cd1] = d1;
					}else{
						dpy[j + cd1] = -2;
					}
					if(dpx[j] == -2)
						dpy[j + cd1] = -2; 
				}
			}
			for(int j = 0; j <= p1 + p2; j++){
				if(dpy[j] == -2) continue;
				if(dpx[j] == -1) continue;
				if(dpx[j] == -2){
					dpy[j] = -2; continue;	
				}
				if(dpy[j] == -1)
					dpy[j] = -3;
				else
					dpy[j] = -2;
			}
			dpx = dpy;
		}
		if(dpx[p1] == -2 || dpx[p2] == -2){
			printf("no\n");
			continue;
		}
		if(dpx[p1] != -1 || dpx[p2] != -1){
			if(dpx[p1] != -1){
				for(int i = 0; i <= p1 + p2; i++) flag[i] = false;
				int nowpos = p1, i = cntab + cntc;
				while(nowpos > 0){
					if(dp[i][nowpos] != -3)
						flag[dp[i][nowpos]] = true;
					if(dp[i][nowpos] != -3) 
						nowpos -= node[dp[i][nowpos]].nums;
					i--; 
				}
			}else{
				for(int i = 0; i <= p1 + p2; i++) flag[i] = true;
				int nowpos = p2, i = cntab + cntc;
				while(nowpos > 0){
					if(dp[i][nowpos] != -3)
						flag[dp[i][nowpos]] = false;
					if(dp[i][nowpos] != -3) 
						nowpos -= node[dp[i][nowpos]].nums;
					i--; 
				}
			}
			for(int i = 1; i <= p1 + p2; i++)
				if(flag[node[i].pre])
					printf("%d\n", i);
			printf("end\n");
		}else{
			printf("no\n");
		}
	}
	return 0;
}
