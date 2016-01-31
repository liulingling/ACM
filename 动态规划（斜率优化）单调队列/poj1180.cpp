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
#define maxn 10010
#define mod 1000000007

int tim[maxn], cost[maxn];
int q[maxn];
ll dp[maxn];
ll G(int x, int y){
	return dp[x] - dp[y];
}
ll S(int x, int y){
	return (ll)(tim[x] - tim[y]);
}

int main(){
	
	int n, s;
	while(scanf("%d %d", &n, &s) != EOF){
		for(int i = 0; i < n; i++){
			scanf("%d %d", &tim[i], &cost[i]);
		}
		tim[n] = 0, cost[n] = 0;
		for(int i = n - 1; i >= 0; i--) tim[i] += tim[i + 1], cost[i] += cost[i + 1];
		int front = 0, rear = 0, k;
		q[0] = n + 1;
		for(int i = n - 1; i >= 0; i--){
			while(front < rear && G(q[front + 1], q[front]) <= cost[i] * S(q[front + 1], q[front]))front++;
			k = q[front];	dp[i] = dp[k] + (s + tim[i] - tim[k]) * cost[i];
			
			while(front < rear && G(i, q[rear]) * S(q[rear], q[rear - 1]) <= G(q[rear], q[rear - 1]) * S(i, q[rear])) rear--;
			q[++rear] = i;
		}
		printf("%lld\n", dp[0]); 	
	}	
	return 0;
}
