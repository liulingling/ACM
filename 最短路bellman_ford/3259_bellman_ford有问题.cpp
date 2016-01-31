#include<iostream>  
using namespace std;  
const int fMax = 505;  
const int eMax = 5205;  
const int wMax = 99999;  
  
  
  
struct{  
    int sta, end, time;  
}edge[eMax];  
int point_num, edge_num, dict[fMax];  
  
  
  
bool bellman_ford()  
{  
    int i, j;  
    for(i = 2; i <= point_num; i ++)  
        dict[i] = wMax;  
    for(i = 1; i < point_num; i ++)  
    {  
        bool finish = true;    //  加个全部完成松弛的判断，优化了50多MS。   
        for(j = 1; j <= edge_num; j ++)  
        {  
            int u = edge[j].sta;  
            int v = edge[j].end;  
            int w = edge[j].time;  
            if(dict[v] > dict[u] + w)  
            {   //  松弛。  
                dict[v] = dict[u] + w;  
                finish = false;  
            }  
        }  
        if(finish)  break;  
    }  
    for(i = 1; i <= edge_num; i ++)  
    {   //  是否存在负环的判断。  
        int u = edge[i].sta;  
        int v = edge[i].end;  
        int w = edge[i].time;  
        if(dict[v] > dict[u] + w)   
              
            return false;  
    }  
    return true;  
}  
  
  
  
int main()  
{  
    int farm;  
    scanf("%d", &farm);  
    while(farm --)  
    {  
        int field, path, hole;  
        scanf("%d %d %d", &field, &path, &hole);  
        int s, e, t, i, k = 0;  
        for(i = 1; i <= path; i ++)  
        {  
            scanf("%d %d %d", &s, &e, &t);  //  用scanf代替了cin，优化了100多MS。  
            k ++;  
            edge[k].sta = s;  
            edge[k].end = e;  
            edge[k].time = t;  
            k ++;  
            edge[k].sta = e;  
            edge[k].end = s;  
            edge[k].time = t;  
        }  
        for(i = 1; i <= hole; i ++)  
        {  
            scanf("%d %d %d", &s, &e, &t);  
            k ++;  
            edge[k].sta = s;  
            edge[k].end = e;  
            edge[k].time = -t;  
        }  
        point_num = field;  
        edge_num = k;  
        if(!bellman_ford())    
            printf("YES\n");  
        else  printf("NO\n");  
    }  
    return 0;  
}  
