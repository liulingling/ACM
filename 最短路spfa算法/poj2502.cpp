#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <math.h>
#include <string>

using namespace std;

struct Point{
	int x, y;
	bool operator ==(const Point &p){
		if(x == p.x && y == p.y)
			return true;
		return false;
	}
};

const int MaxV = 700;
const double INF = -1;

double mat[MaxV][MaxV];
double dist[MaxV];
bool inq[MaxV];

vector<Point> stopLists;
Point st, ed;
int stId, edId;
bool first;


double getDistance(Point &s, Point &e){
	return sqrt((s.x - e.x) * (s.x - e.x) + (s.y - e.y) * (s.y - e.y) + 0.0);
}

int getID(const Point &p){
	for(int i = 0; i < stopLists.size(); i++){
		if(stopLists[i] == p){
			return i;
		}
	}
	stopLists.push_back(p);
	return stopLists.size() - 1;
}

bool relax(int u, int v, double c){
	if(c < 0)
		return false;
	if(dist[v] < 0 || dist[v] > dist[u] + c){
		dist[v] = dist[u] + c;
		return true;
	}
	return false;
}
void solve(){
	if(stId == -1 || edId == -1)
		return;
	for(int i = 0; i < stopLists.size(); i++){
		for(int j = i + 1; j < stopLists.size(); j++){
			double walkDis = getDistance(stopLists[i], stopLists[j]);
			walkDis = walkDis * 6 / 1000;
			if(mat[i][j] < 0 || mat[i][j] > walkDis){
				mat[i][j] = mat[j][i] = walkDis;
			}
		}
	}
	int N = stopLists.size();
	for(int i = 0; i <= N; i++){
		inq[i] = false;
		dist[i] = INF;
	} 
	queue<int> que;
	dist[stId] = 0;
	inq[stId] = true;
	que.push(stId);
	int u, v;
	while(!que.empty()){
		u = que.front();
		inq[u] = false;
		que.pop();
		for(v = 0; v < N; v++){
			if(mat[u][v] < 0)
				continue;
			if(relax(u, v, mat[u][v]) && !inq[v]){
				
				inq[v] = true;
				que.push(v);
			}
		}
	}
	int ans = (int)(dist[edId] + 0.5);
	printf("%d\n", ans);
}
bool nextInt(char buffer[], int& pos, int &num){
	int sign = 1;
	num = 0;
	int state = 0;
	while(buffer[pos] != 0){
		if(buffer[pos] >= '0' && buffer[pos] <= '9'){
			state = 1;
			num = num * 10 + buffer[pos] - '0';
			pos++;
		}else if(buffer[pos] == '-'){
			state = 1;
			sign = -1;
			pos++;
		}else if(buffer[pos] == '+'){
			state = 1;
			sign = 1;
			pos++;
		}else{
			if(state == 0){
				pos++;
			}else{
				num = num * sign;
				return true;
			} 
		}
	}
	if(state == 1){
		num = num * sign;
		return true;
	}
	return false;
}

void addSubWay(char buffer[]){
	int tmp, state = 0, pos = 0;
	Point astop, fstop;
	bool firststop = false;
	vector<int> stops;
	
	while(nextInt(buffer, pos, tmp)){
		if(state == 0){
			astop.x = tmp;
			state = 1;
		}else{
			astop.y = tmp;
			if(firststop == false){
				firststop = true;
			}else{
				int stopId = getID(fstop);
				stops.push_back(stopId);
			}
			fstop = astop;
			state = 0;
		}
	}
	vector<double> disLists;
	double nowDis = 0, sumDis = 0;
	for(int i = 0; i < stops.size(); i++){
		nowDis = getDistance(stopLists[stops[i == 0 ? 0: i - 1]], stopLists[stops[i]]);
		sumDis += nowDis;
		disLists.push_back(sumDis);
	}
	int iId, jId;
	for(int i = 0; i < stops.size(); i++){
		iId = stops[i];
		for(int j = i + 1; j < stops.size(); j++){
			jId = stops[j];
			nowDis = disLists[j] - disLists[i];
			nowDis = (nowDis * 3) / 2000; 
			if(mat[iId][jId] < 0 || mat[iId][jId] > nowDis){
				mat[iId][jId] = nowDis;
				mat[jId][iId] = nowDis;
			} 
		}
	}
}
void setStEd(char buffer[]){
	if(first){
		solve();	
	}else{
		first = true;
	}
	int tmp;
	int state = 0, pos = 0;
	while(nextInt(buffer, pos, tmp)){
		if(state == 0){
			st.x = tmp;
			state = 1;
		}else if(state == 1){
			st.y = tmp;
			state = 2;
		}else if(state == 2){
			ed.x = tmp;
			state = 3;			
		}else if(state == 3){
			ed.y = tmp;
			state = 4;
		}else{
			break;
		}
	}
							
	for(int i = 0; i < 210; i++){
		for(int j = 0; j < 210; j++){
			mat[i][j] = INF;
		}
	}
	stopLists.clear();
	stId = getID(st);
	edId = getID(ed);
}
int isWhat(char buffer[]){
	int cntnums = 0;
	int state = 0;//0 initstate 1 nums 2 else
	for(int i = 0; buffer[i] != '\0'; i++){
		if(buffer[i] >= '0' && buffer[i] <= '9' ||
			buffer[i] == '-' || buffer[i] == '+'){
			if(state == 0 || state == 2){
				cntnums++;
				state = 1;	
			}
		}else{
			state = 2;
		}
	}
	if(cntnums == 4)
		return 1;
	if(cntnums >= 6)
		return 2;
	return 0;
}
char buffer[50000];

int main(){
	
	string oneline;
	first = false;
	stId = -1, edId = -1;
	while(getline(cin, oneline)){
		strcpy(buffer, oneline.c_str());
		int sign = isWhat(buffer);
		if(sign == 0)
			continue;
		else if(sign == 1)
			setStEd(buffer);
		else
			addSubWay(buffer);
	}	
	solve();
	return 0;
}
