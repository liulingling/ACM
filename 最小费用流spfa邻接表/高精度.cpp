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

#define INF 0x7fffffff
#define read freopen("in.txt","r",stdin)
#define write freopen("out.txt","w",stdout)
#define maxn 55
#define mod 1000000007

#define sqr(x) ((x) * (x))
#define lowbit(x) ((x) & (-(x)))
#define PB push_back
#define FOR(i, a, b) for(int i = (a); i < (b); ++i)
#define FORD(i, a, b) for(int i = (a) - 1; i >= (b); --i)

template<class T> inline void checkmin(T &a, T b){if(b < a) a = b;}
template<class T> inline void checkmax(T &a, T b){if(b > a) a = b;}

typedef vector<int> VI;
typedef vector<string> VS;
typedef vector<double> VD;
typedef long long ll;

const double PI = acos(-1.0);
const double EPS = 1e-8;

const int K = 10000;
struct Node{
	bool sign;
	vector<int> ls;
	Node(): sign(true){}
	Node(string oneline);
	const Node& operator = (const Node& t){
		sign = t.sign;
		ls.clear();
		FOR(i, 0, t.ls.size()){
			ls.push_back(t.ls[i]);
		}
		return *this;
	}
};
ostream& operator << (ostream& out, const Node& a){
	int index = a.ls.size() - 1;
	while(index >= 0 && a.ls[index] == 0)index--;
	if(index < 0)out<<"0";
	else{
		if(a.sign == false) out<<"-";
		out<<a.ls[index];
		index--;
		while(index >= 0){
			if(a.ls[index] < 1000)
				out<<"0";
			if(a.ls[index] < 100)
				out<<"0";
			if(a.ls[index] < 10)
				out<<"0";
			out<<a.ls[index];
			index--;
		}	
	}
	return out;
}
Node operator + (const Node& lhs, const Node& rhs){
	Node ans;
	int l1 = lhs.ls.size(), l2 = rhs.ls.size();
	int i;
	for(i = 0; i < l1 && i < l2; ++i) ans.ls.push_back(lhs.ls[i] + rhs.ls[i]);
	while(i < l1) ans.ls.push_back(lhs.ls[i++]);
	while(i < l2) ans.ls.push_back(rhs.ls[i++]);
	
	int yu = 0, sum;
	for(int i = 0; i < ans.ls.size(); ++i){
		sum = ans.ls[i] + yu;
		ans.ls[i] = sum % K;
		yu = sum / K;
	}
	while(yu > 0){
		ans.ls.push_back(yu % K);
		yu /= K;
	}
	return ans;
}
Node operator * (const Node& lhs, const Node& rhs){
	Node ans;
	int l1 = lhs.ls.size(), l2 = rhs.ls.size();
	for(int i = 0; i < l1 + l2; ++i){
		ans.ls.push_back(0); 
	}
	if(lhs.sign == rhs.sign){
		ans.sign = true;
	}else{
		ans.sign = false;
	}
	FOR(i, 0, l1)FOR(j, 0, l2)
		ans.ls[i + j] += lhs.ls[i] * rhs.ls[j];
	int yu = 0, sum;
	FOR(i, 0, l1 + l2){
		sum = ans.ls[i] + yu;
		ans.ls[i] = sum % K;
		yu = sum / K; 
	}
	while(yu > 0){
		ans.ls.push_back(yu % K);
		yu /= K;
	}
	return ans;
}
void rev(char src[], int len){
	for(int i = 0; i < len / 2; ++i){
		swap(src[i], src[len - 1 - i]);
	}
}
Node::Node(string oneline){
	char buffer[1024];
	strcpy(buffer, oneline.c_str());
	int i = 0;
	if(buffer[0] == '-'){
		sign = false;
		i = 1;
	}else if(buffer[1] == '+'){
		sign = true;
		i = 1;
	}else{
		sign = true;
	}
	int len = strlen(buffer);
	rev(buffer + i, len - i);
	int cnt = 0, sumvalue = 0;
	for(; buffer[i] != '\0'; ++i){
		cnt++;
		if(cnt == 1)
			sumvalue = buffer[i] - '0';
		else if(cnt == 2)
			sumvalue = sumvalue  + (buffer[i] - '0') * 10;
		else if(cnt == 3)
			sumvalue = sumvalue + (buffer[i] - '0') * 100;
		else if(cnt == 4)
			sumvalue = sumvalue + (buffer[i] - '0') * 1000;
		if(cnt == 4){
			ls.push_back(sumvalue);
			cnt = 0;
			sumvalue = 0;
		}
	}
	if(cnt != 0){
		ls.push_back(sumvalue);
	}
}
int main(){

	string a, b;
	cin>>a;
	cin>>b;
	Node na(a);
	Node nb(b);
	cout<<"a = "<<na<<endl;
	cout<<"b = "<<nb<<endl;
	cout<<"na + nb = "<<nb + na<<endl;
	cout<<"na * nb = "<<na * nb<<endl;
	system("pause");
    return 0;
}
