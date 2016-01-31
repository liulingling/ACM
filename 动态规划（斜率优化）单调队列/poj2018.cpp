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
#define maxn 100010
#define mod 1000000007


int cows[maxn];
int q[maxn];

bool ok(int i, int j, int t){
	return ((ll)(cows[t] - cows[i])) * (j - i) <
			((ll)(cows[j] - cows[i])) * (t - i);
}
int main(){
	int N, F;
	while(scanf("%d %d", &N, &F) != EOF){
		cows[0] = 0;
		for(int i = 1; i <= N; i++){
			scanf("%d", &cows[i]);
			cows[i] += cows[i - 1];
		}
		int front = 0, rear = 0;
		q[0] = 0;
		
		double ans = 0, tmp, tmp1;
		for(int i = 1; i <= N; i++){
			while(front < rear && ok(q[rear - 1], q[rear], i)) --rear;
			q[++rear] = i;
			if(i < F) continue;
			tmp = (cows[q[rear]] - cows[q[front]] + 0.0) / (q[rear] - q[front]);
			while(front < rear && q[rear] - q[front + 1] >= F){
				tmp1 = (cows[q[rear]] - cows[q[front + 1]] + 0.0) / (q[rear] - q[front + 1]);
				if(tmp1 <= tmp)	break;
				tmp = tmp1;
				front++;
			}
			if(ans < tmp)	ans = tmp;
		}
		printf("%d\n", (int)(ans * 1000));
	}
	return 0;
}
