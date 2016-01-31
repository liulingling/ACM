#include <cstdio>
#include <queue>
#include <algorithm>
using namespace std;
 
const int N = 128;
const int INF = 1 << 28;
 
class Graph {
private:
   bool xckd[N], yckd[N];
   int n, edge[N][N], xmate[N], ymate[N];
   int lx[N], ly[N], slack[N], prev[N];
   queue<int> Q;
   bool bfs();
   void agument(int);
public:
   bool make();
   int KMMatch();
};
bool Graph::make() {
   int house[N], child[N], h, w, cn = 0;
   char line[N];
   scanf("%d %d", &h, &w);
   if(w == 0) return false;
   scanf("\n"); n = 0;
   for(int i = 0; i < h; i++) {
      gets(line);
      for(int j = 0; line[j] != 0; j++) {
         if(line[j] == 'H') house[n++] = i * N + j;
         if(line[j] == 'm') child[cn++] = i * N + j;
      }
   }
   for(int i = 0; i < n; i++) {
      int cr = child[i] / N, cc = child[i] % N;
      for(int j = 0; j < n; j++) {
         int hr = house[j] / N, hc = house[j] % N;
         edge[i][j] = -abs(cr-hr) - abs(cc-hc);
      }
   }
   return true;
}
bool Graph::bfs() {
   while(!Q.empty()) {
      int p = Q.front(), u = p>>1; Q.pop();
      if(p&1) {
         if(ymate[u] == -1) { agument(u); return true; }
         else { xckd[ymate[u]] = true; Q.push(ymate[u]<<1); }
      } else {
         for(int i = 0; i < n; i++)
            if(yckd[i]) continue;
            else if(lx[u]+ly[i] != edge[u][i]) {
               int ex = lx[u]+ly[i]-edge[u][i];
               if(slack[i] > ex) { slack[i] = ex; prev[i] = u; }
            } else {
               yckd[i] = true; prev[i] = u;
               Q.push((i<<1)|1);
            }
      }
   }
   return false;
}
void Graph::agument(int u) {
   while(u != -1) {
      int pv = xmate[prev[u]];
      ymate[u] = prev[u]; xmate[prev[u]] = u;
      u = pv;
   }
}
int Graph::KMMatch() {
   memset(ly, 0, sizeof(ly));
   for(int i = 0; i < n; i++) {
      lx[i] = -INF;
      for(int j = 0; j < n; j++) lx[i] >?= edge[i][j];
   }
   memset(xmate, -1, sizeof(xmate)); memset(ymate, -1, sizeof(ymate));
   bool agu = true;
   for(int mn = 0; mn < n; mn++) {
      if(agu) {
         memset(xckd, false, sizeof(xckd));
         memset(yckd, false, sizeof(yckd));
         for(int i = 0; i < n; i++) slack[i] = INF;
         while(!Q.empty()) Q.pop();
         xckd[mn] = true; Q.push(mn<<1);
      }
      if(bfs()) { agu = true; continue; }
      int ex = INF; mn--; agu = false;
      for(int i = 0; i < n; i++)
         if(!yckd[i]) ex <?= slack[i];
      for(int i = 0; i < n; i++) {
         if(xckd[i]) lx[i] -= ex;
         if(yckd[i]) ly[i] += ex;
         slack[i] -= ex;
      }
      for(int i = 0; i < n; i++) 
         if(!yckd[i] && slack[i] == 0) { yckd[i] = true; Q.push((i<<1)|1); }
 
   }
   int cost = 0;
   for(int i = 0; i < n; i++) cost += edge[i][xmate[i]];
   return cost;
}
 
int main()
{
   Graph g;
 
   while(g.make()) printf("%d\n", -g.KMMatch());
 
   return 0;
}
