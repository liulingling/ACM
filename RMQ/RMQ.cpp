#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;

const int MaxN = 1010;

int st[20][MaxN], ln[MaxN], val[MaxN];
void initrmp(int n){
	int i, j, k, sk;
	ln[0] = ln[1] = 0;
	for(i = 0; i < n; i++)	st[0][i] = val[i];
	
	for(i = 1, k = 2; k < n; i++, k <<= 1){
		for(j = 0, sk = (k >> 1); j < n; ++j, ++sk){
			st[i][j] = st[i - 1][j];
			if(sk < n && st[i][j] > st[i - 1][sk])
				st[i][j] = st[i - 1][sk];
		}
		for(j = (k>>1) + 1; j <= k; ++j) ln[j] = ln[k>>1] + 1;
	}
	for(j = (k>>1) + 1; j <= k; ++j) ln[j] = ln[k>>1] + 1;
}
int query(int x, int y){
	int bl = ln[y - x + 1];
	return min(st[bl][x], st[bl][y - (1<<bl) + 1]);
}
int main(){
	
	return 0;
}
