#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;


namespace AliasBiMatch{

struct Edge{
	int pnt;
	int nxt;
};
	
	int *pvis, *pmatch, *phead;
	Edge *pedges;
	int gN, gM, nowE;
	int nowTimes = 0;
	void initMap(){
		nowE = 0;
		for(int i = 0; i < gN; i++){
			phead[i] = -1;
		}
	}
	void addedge(int u, int v){
		pedges[nowE].pnt = v;
		pedges[nowE].nxt = phead[u];
		phead[u] = nowE;
		nowE++;
	}
	void biMatchInit(){
		nowTimes = 0;
		for(int i = 0; i < gM; i++){
			pmatch[i] = -1;
			pvis[i] = 0;
		}
	}
	void biMatch(int gn, int gm, Edge edges[],
				int head[], int vis[], int match[]){
		gN = gn, gM = gm, phead = head;
		pvis = vis, pmatch = match, pedges = edges;
	}
	bool biMatchDfs(int u){
		int v, ee = phead[u];
		while(ee != -1){
			v = pedges[ee].pnt;
			if(pvis[v] != nowTimes){
				pvis[v] = nowTimes;
				if(pmatch[v] == -1 ||
					biMatchDfs(pmatch[v])){
					pmatch[v] = u;
					return true;	
				}
			}
			ee = pedges[ee].nxt;
		}
		return false;
	}
	int solve(){
		int i, num = 0;
		for(int i = 0; i < gN; i++){
			nowTimes++;
			if(biMatchDfs(i)){
				num++;
			}
		}
		return num; 
	}	
}
const int MaxV = 2010;
const int MaxE = 20010;

int vis[MaxV], match[MaxV], head[MaxV];
AliasBiMatch::Edge edges[MaxE];

bool appear[1200];

int toInt(char buffer[]){
	int len = strlen(buffer);
	int p = 1, sum = 0;
	for(int i = len - 1; i >= 0; i--){
		if(buffer[i] == '1'){
			sum += p;	
		}
		p *= 2;
	}
	return sum;
}
void preProcess(char buffer[]){
	bool exitStar = false;
	char buffer1[20], buffer2[20];
	for(int i = 0; buffer[i] != '\0'; i++){
		if(buffer[i] == '*'){
			strcpy(buffer1, buffer);
			strcpy(buffer2, buffer);
			buffer1[i] = '0';
			buffer2[i] = '1';
			exitStar = true;
			break;
		}
	}
	if(!exitStar){
		int index = toInt(buffer);
		appear[index] = true;
	}else{
		int index = toInt(buffer1);
		appear[index] = true;
		index = toInt(buffer2);
		appear[index] = true;
	}
	
}

int lowbit(int x){
	return x & (-x);
}
bool judge(int a, int b){
	if(a == b)
		return false;
	int c = a ^ b;
	if(lowbit(c) == c){
		return true;
	}
	return false;
}
int main(){
	int n, m;
	while(scanf("%d %d", &n, &m) != EOF){
		if(n == 0 && m == 0){
			break;
		}
		char buffer[16];
		
		for(int i = 0; i < 1024; i++){
			appear[i] = false;
		}
		for(int i = 0; i < m; i++){
			scanf("%s", buffer);
			preProcess(buffer);
		}
		AliasBiMatch::biMatch(1024, 1024, edges, head, vis, match);
		AliasBiMatch::initMap();
		
		int virusNums = 0;
		for(int i = 0; i < 1024; i++){
			if(appear[i]){
				virusNums++;
				for(int j = i + 1; j < 1024; j++){
					if(appear[j] && judge(i, j)){
						AliasBiMatch::addedge(i, j);
						AliasBiMatch::addedge(j, i);
					}
				}
			}
		}
		AliasBiMatch::biMatchInit();
		int matchNums = AliasBiMatch::solve();
		
		printf("%d\n", virusNums - matchNums / 2);
	}
	return 0;
}
