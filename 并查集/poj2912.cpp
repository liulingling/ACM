#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;
#define maxn 510
#define maxm 2010

int a[maxm], b[maxm], c[maxm];
int d[maxn], pre[maxn], dis[maxn], nums[maxn];
int n, m;

void init(){
    for(int i = 0; i < n; i++){
	pre[i] = i, dis[i] = 0, nums[i] = 1;
    }
}
int find(int x){
   if(x == pre[x]) return x;
   int tmpfa = pre[x];
   int fa = find(tmpfa);
   dis[x] = (dis[x] + dis[tmpfa] + 3) % 3;
   pre[x] = fa;
   return fa;
}
bool merge(int u, int v, int uvd){
    int fu = find(u), fv = find(v);
    uvd = (uvd + 3) % 3;
    if(fu == fv){
	if((dis[v] - dis[u] + 3) % 3 != uvd) return false;
	return true; 
    }
    if(nums[fu] > nums[fv]){
	nums[fu] += nums[fv];
	pre[fv] = fu;
	dis[fv] = (9 + dis[u] + uvd - dis[v]) % 3;
	return true;
    }else{
	nums[fv] += nums[fu];
	pre[fu] = fv;
	dis[fu] = (9 - dis[u] - uvd + dis[v]) % 3; 
    	return true;
    }
}
void isJudge(int i){
    init();
    for(int j = 0; j < m; j++){
	if(a[j] == i || b[j] == i) continue;
	if(!merge(a[j], b[j], c[j])){
	    d[i] = j + 1;
	    return;
	}
    }
}
int main(){
 
   while(scanf("%d %d", &n, &m) != EOF){
   	for(int i = 0; i < m; i++){
	    int id1, id2;
	    char cop;
	    scanf("%d", &id1);
	    while((cop = getchar()) == ' ');
	    scanf("%d", &id2);
	    a[i] = id1, b[i] = id2;
	    //printf("%d %c %d\n", id1, cop, id2);
	    if(cop == '=') c[i] = 0;
	    else if(cop == '>') c[i] = 2;
	    else c[i] = 1; 
	}
	for(int i = 0; i < n; i++) d[i] = -1;
	for(int i = 0; i < n; i++){
	    isJudge(i);
	}
	int cnt = 0, steps = 0, ans = 0;
	for(int i = 0; i < n; i++){
	    if(d[i] == -1){
		cnt++;
		ans = i;
	    }else{
		steps = max(steps, d[i]);
	    }
	}
	if(cnt == 0){
	    puts("Impossible");
	}else if(cnt == 1){
	    printf("Player %d can be determined to be the judge after %d lines\n", ans, steps);
	}else{
	    printf("Can not determine\n");
	}
   }	
   
   return 0;
}