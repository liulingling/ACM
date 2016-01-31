#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

const int MaxW = 110;
const int MaxH = 110;


int mat[MaxH][MaxW];
int D[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0},
				{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
int que[10010][2];
int nowClusterId;

struct TPoint{
#define eps 0.000001
	int x, y;
	TPoint& operator = (const TPoint& ap){
		x = ap.x;
		y = ap.y;
		return *this;
	}
	bool operator != (const TPoint& ap){
		if(x != ap.x || y != ap.y)
			return true;
		return false;
	}
	void pan(int dltx, int dlty){
		x -= dltx, y -= dlty;
	}
	void rotation(double ang){
		double cosang = cos(ang), sinang = sin(ang);
		rotation(cosang, sinang);
	}
	void rotation(double cosang, double sinang){
		double x1 = x * cosang - y * sinang;
		double y1 = y * cosang + sinang * x;
		x = (int) (x1 + eps);
		y = (int) (y1 + eps);
	}
	void rotationVcl(int sign){
		//sign = 1, 2, 3, 0 for 90 180 270
		int x1 = x, y1 = y;
		if(sign == 1){
			x1 = -y, y1 = x;
		}else if(sign == 2){
			x1 = -x, y1 = -y;
		}else if(sign == 3){
			x1 = y, y1 = -x;
		}
		x = x1, y = y1;
	}
	friend bool operator < (const TPoint& lhs, const TPoint& rhs){
		if(lhs.x != rhs.x)
			return lhs.x < rhs.x;
		return lhs.y < rhs.y;
	}	
};
struct TCluster{
	TCluster(){
		cntPoints = 0;
		belong = 1;
	}
	TCluster& operator = (const TCluster& ac){
		cntPoints = ac.cntPoints;
		for(int i = 0; i < cntPoints; i++){
			points[i] = ac.points[i];
		}
		return *this;
	}
	void addPoint(const TPoint& ap){
		points[cntPoints] = ap;
		cntPoints++;
	}
	void addPoint(const int x, const int y){
		points[cntPoints].x = x;
		points[cntPoints].y = y;
		cntPoints++;
	}
	void sortMe(){
		sort(points, points + cntPoints);
	}
	void Pan(int dltx, int dlty){
		for(int i = 0; i < cntPoints; i++){
			points[i].pan(dltx, dlty);
		}
	}
	void rotationVcl(int sign){
		for(int i = 0; i < cntPoints; i++){
			points[i].rotationVcl(sign);
		}
	}
	bool equal(const TCluster& cluster){
		if(cntPoints != cluster.cntPoints)
			return false;
		for(int i = 0; i < cntPoints; i++){
			if(points[i] != cluster.points[i])
				return false;
		}
		return true;	
	}
	void reflect(){
		for(int i = 0; i < cntPoints; i++){
			points[i].x = - points[i].x;
		}
	}
	bool similarTo(const TCluster& cluster){
		TCluster tc = cluster;
		TCluster me;
		me.cntPoints = cntPoints;
		for(int i = 0; i < cntPoints; i++){
			me.points[i] = points[i];
		}
		int dltx, dlty, cntTimes = 0;
		me.sortMe();
		do{
			cntTimes++;
			tc.sortMe();
			dltx = tc.points[0].x - me.points[0].x;
			dlty = tc.points[0].y - me.points[0].y;
			tc.Pan(dltx, dlty);
			if(me.equal(tc))	return true;
			tc.rotationVcl(1);//90¶È
		}while(cntTimes < 4);
		tc.reflect();
		cntTimes = 0;
		do{
			cntTimes++;
			tc.sortMe();
			dltx = tc.points[0].x - me.points[0].x;
			dlty = tc.points[0].y - me.points[0].y;
			tc.Pan(dltx, dlty);
			if(me.equal(tc))	return true;
			tc.rotationVcl(1);//90¶È
		}while(cntTimes < 4);
		return false;		
	}
	void rotation(double ang){
		
	}
	int cntPoints;
	int belong;
	TPoint points[170];		
};
TCluster clusters[510];
int h, w;

void bfs(int stx, int sty){
	nowClusterId++;
	int front = 0, rear = 0;
	int x, y, xx, yy;
	que[0][0] = stx, que[0][1] = sty;
	mat[stx][sty] = -1;
	clusters[nowClusterId].addPoint(stx, sty);
	while(front <= rear){
		x = que[front][0], y = que[front][1];
		front++;
		for(int i = 0; i < 8; i++){
			xx = x + D[i][0];
			yy = y + D[i][1];
			if(xx >= 0 && x < h && yy >= 0 &&
				yy < w && mat[xx][yy] == 1){
				mat[xx][yy] = -1;
				++rear;
				que[rear][0] = xx, que[rear][1] = yy;
				clusters[nowClusterId].addPoint(xx, yy);
			}
		}
	}
}
int main(){
	char buffer[110];
	while(scanf("%d %d", &w, &h) != EOF){
		
		for(int i = 0; i < h; i++){
			scanf("%s", buffer);
			for(int j = 0; j < w; j++){
				if(buffer[j] == '0'){
					mat[i][j] = 0;
				}else{
					mat[i][j] = 1;
				}
			}
		}
		nowClusterId = 0;
		for(int i = 0; i < h; i++){
			for(int  j = 0; j < w; j++){
				if(mat[i][j] == 1){
					bfs(i, j);
				}
			}
		}
		int cntNS = 0;
		bool flag;
		for(int i = 1; i <= nowClusterId; i++){
			flag = false;
			for(int j = 1; j < i; j++){
				if(clusters[j].similarTo(clusters[i])){
					clusters[i].belong = clusters[j].belong;
					flag = true;
					break;
				}
			}
			if(!flag){
				clusters[i].belong = ++cntNS;			
			}
		}
		int x, y;
		for(int i = 1; i <= nowClusterId; i++){
			for(int j = 0; j < clusters[i].cntPoints; j++){
				x = clusters[i].points[j].x;
				y = clusters[i].points[j].y;
				mat[x][y] = clusters[i].belong;
			}
		}
		for(int i = 0; i < h; i++){
			for(int j = 0; j < w; j++){
				if(mat[i][j] == 0){
					printf("0");
				}else{
					printf("%c", mat[i][j] - 1 + 'a');
				}
			}
			putchar(10);
		}
	}
	return 0;
}
