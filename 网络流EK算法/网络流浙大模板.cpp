#include <stdio.h>
#include <string.h>
#include <algorithm>
using  namespace std;

#include <stdio.h>
#include <string.h>
const int MAXN = 210;
const int inf = 100000000;
int N, M;
int mat[MAXN][MAXN], flow[MAXN][MAXN];
//网络流模板来自浙大模板
int max_flow(int n,int source,int sink){
    int pre[MAXN],que[MAXN],d[MAXN],p,q,t,i,j;
    int u, v, w;
    int cur = 0;
	if (source==sink) return inf;
	for (i=0;i<n;i++)
		for (j=0;j<n;flow[i][j++]=0);
	for(int ii = 0; ii < M; ii++){
        scanf("%d %d %d", &u, &v, &w);
        mat[u-1][v-1] = mat[v-1][u-1] = w;
		for (;;){
			for (i=0;i<n;pre[i++]=0);
			pre[t=source]=source+1,d[t]=inf;
			for (p=q=0;p<=q&&!pre[sink];t=que[p++])
				for (i=0;i<n;i++)
					if (!pre[i]&&(j=mat[t][i]-flow[t][i]))
						pre[que[q++]=i]=t+1,d[i]=d[t]<j?d[t]:j;
					else if (!pre[i]&&(j=flow[i][t]))
						pre[que[q++]=i]=-t-1,d[i]=d[t]<j?d[t]:j;
			if (!pre[sink]) break;
			for (i=sink;i!=source;)
				if (pre[i]>0)
					flow[pre[i]-1][i]+=d[sink],i=pre[i]-1;
				else
					flow[i][-pre[i]-1]-=d[sink],i=-pre[i]-1;
		}
		for (j=i=0;i<n;j+=flow[source][i++]);
        if(j != cur){
		    printf("%d %d\n", ii+1, j-cur);
            cur = j;
		}
	}
	return j;
}


int  main()
{
	int  T, CAS = 1;
	//freopen("test.in", "r", stdin);
	scanf("%d", &T);
	while(T--)
	{
        memset(mat, 0, sizeof(mat));
        memset(flow, 0, sizeof(flow));
		scanf("%d %d", &N, &M);
		printf("Case #%d:\n", CAS++);
        max_flow(N, 0, N-1);
	}
	return 0;
}
