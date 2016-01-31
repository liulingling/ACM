#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;

namespace AliasGarsiaWachs{
	
	int N;
	int *pstone;
	int ans, t;
	void garsiaWachs(int n, int stone[]){
		N = n, pstone = stone;
	}
	void garsiaWachsInit(){
		ans = 0, t = 1;
	}
	void combine(int k){
		int tmp = pstone[k] + pstone[k - 1];
		ans += tmp;
		for(int i = k; i < t - 1; i++){
			pstone[i] = pstone[i + 1];
		}
		t--;
		int j;
		for (j = k - 1; j > 0 && pstone[j - 1] < tmp ; j--){
            pstone[j] = pstone[j-1] ;
      	}
      	pstone[j] = tmp;
    	while (j >= 2 && pstone[j] >= pstone[j - 2]){
        	int d = t - j;
            combine(j - 1) ;
            j = t - d;
       }
	}
	int solve(){
		for(int i = 1; i < N; i++){
			pstone[t++] = pstone[i];
			while (t >= 3 && pstone[t - 3] <= pstone[t - 1]){
				combine(t - 2);
			}
		}
		while(t > 1) combine(t - 1);
		return ans;
	}
}

const int MaxN = 100010;
int stone[MaxN];
int n;
int main(){
	
	while(scanf("%d", &n), n){
		
		for(int i = 0; i < n; i++){
			scanf("%d", &stone[i]);
		}
		AliasGarsiaWachs::garsiaWachs(n, stone);
		AliasGarsiaWachs::garsiaWachsInit();
		int ans = AliasGarsiaWachs::solve();
		printf("%d\n", ans);
	}
	return 0;
}
