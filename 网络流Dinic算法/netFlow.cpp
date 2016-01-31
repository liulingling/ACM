#include <cstdio>
#include <cstring>
using namespace std;

const int MAXN = 210;
const int INF = 0x3fffffff;
int n, m, s, t, g[MAXN][MAXN];
int f[MAXN][MAXN], pnt[MAXN], open[MAXN], d[MAXN], mk[MAXN];
int flow;

int maxflow(int n, int s, int t)
{
     int cur, tail, i, j, u, v;  
     int ret = flow;
     do{ memset(mk,0,sizeof(mk)); memset(d,0,sizeof(d));
         open[0]=s; mk[s]=1; d[s]=INF;
         for(pnt[s]=cur=tail=0;cur<=tail&&!mk[t];cur++)
              for(u=open[cur],v=0;v<n;v++)if(!mk[v]&&f[u][v]<g[u][v]){
                   mk[v]=1; open[++tail]=v; pnt[v]=u;
                   if(d[u]<g[u][v]-f[u][v])d[v]=d[u];
                       else d[v]=g[u][v]-f[u][v];
              }
         if(!mk[t])break; 
         ret+=d[t];
         for(u=t;u!=s;){v=u;u=pnt[v];f[u][v]+=d[t]; f[v][u]=-f[u][v];}
     }while(d[t]>0); 
     return ret;
}

int main(){
    int cases; scanf("%d", &cases);
    for (int tc = 1; tc <= cases; tc++){
        printf("Case #%d:\n", tc);
        scanf("%d %d", &n, &m);
        int u, v, c, nf;
        memset(f, 0, sizeof(f));
        memset(g, 0, sizeof(g));
        flow = 0;
        for (int i = 0; i < m; i++){
            scanf("%d %d %d", &u, &v, &c);
            u--;
            v--;
            g[u][v] += c;
            g[v][u] += c;
            nf = maxflow(n, 0, n-1);
            //printf("%d %d\n", flow, nf);
            if (nf > flow) printf("%d %d\n", i+1, nf-flow);
            flow = nf;
        }
        
    }
}  
