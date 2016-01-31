#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>

using namespace std;
#define MaxN 10010

#define INC 0
#define DEC 1

//sign = INC 递增 
//sign = DEC 递减

bool compare(int &a, int &b, int &sign);
void Bubble(int nus[], int &N, int &sign);
void Select(int nus[], int &N, int &sign);
void Insert(int nus[], int &N, int &sign);
void QuickS(int nus[], int &N, int &sign);
void Merger(int nus[], int &N, int &sign);
void Heapst(int nus[], int &N, int &sign);

int nus[MaxN];
int main(){
	
	freopen("datain.txt", "r", stdin);
	freopen("dataout.txt", "w", stdout);
	int N, sign;
	//N 表示数的个数 
	cin>>N;
	//sign = 0,递增，1, 递减 
	cin>>sign;
	for(int i = 0; i < N; i++)
		cin>>nus[i];
		
	//Bubble(nus, N, sign);
	//Select(nus, N, sign);
	//Insert(nus, N, sign);
	//QuickS(nus, N, sign);
	//Merger(nus, N, sign);
	Heapst(nus, N, sign);
	
	for(int i = 0; i < N; i++){
		cout<<nus[i]<<endl;
	}
	return 0;	
}
bool compare(int &a, int &b, int &sign){
	if(sign == INC){
		if(a <= b)
			return true;
		else return false;
	}else{
		if(a >= b)
			return true;
		else return false;
	}
} 
void Bubble(int nus[], int &N, int &sign){
	for(int i = 1; i < N; i++){
		for(int j = 0; j < N - i; j++){
			if(compare(nus[j], nus[j+1], sign) == false){
				nus[j] ^= nus[j+1];
				nus[j+1] ^= nus[j];
				nus[j] ^= nus[j+1];
			}
		}
	}	
}
void Select(int nus[], int &N, int &sign){
	int pos;
	for(int i = 0; i < N; i++){
		pos = i;
		for(int j = i + 1; j < N; j++){
			if(compare(nus[j], nus[pos], sign) == true){
				pos = j;
			}
		}
		if(pos != i){
			nus[pos] ^= nus[i];
			nus[i] ^= nus[pos];
			nus[pos] ^= nus[i];
		} 
	}
}
void Insert(int nus[], int &N, int &sign){
	int tmp, pos;
	for(int i = 1; i < N; i++){
		tmp = nus[i];
		int j = i;
		for(; j > 0; j--){
			if(compare(nus[j-1], tmp, sign) == false){
				nus[j] = nus[j-1];
			}else{
				break;
			}
		}
		nus[j] = tmp;
	}
}
void Qs(int nus[], int left, int right, int &sign){
	if(left + 1 > right)
		return;
	int sp = left, s = nus[left];
	int i = left, j = right + 1;
	while(i < j){
		while(--j >= i && compare(s, nus[j], sign) == true);
		if(j <= i)
			break;
		nus[sp] = nus[j];
		sp = j;
		while(++i <= j && compare(nus[i], s, sign) == true);
		if(i >= j)
			break;
		nus[sp] = nus[i];
		sp = i;
	}
	nus[sp] = s;
	//排左边 
	Qs(nus, left, sp - 1, sign);
	//排右边 
	Qs(nus, sp + 1, right, sign);
}
void QuickS(int nus[], int &N, int &sign){
	Qs(nus, 0, N - 1, sign);
}
void Mg(int nus[], int tmp[], int left, int right, int &sign){
	if(left + 1 > right)
		return;
	int mid = (left + right) / 2;
	Mg(nus, tmp, left, mid, sign);
	Mg(nus, tmp, mid + 1, right, sign);
	int i = left, j = mid + 1;
	int k = left;
	while(i <= mid && j <= right){
		if(compare(nus[i], nus[j], sign) == true){
			tmp[k] = nus[i];
			i++;
		}else{
			tmp[k] = nus[j];
			j++;
		}
		k++;
	}
	while(i <= mid){
		tmp[k] = nus[i];
		k++, i++;
	}
	while(j <= right){
		tmp[k] = nus[j];
		k++, j++;
	}
	for(i = left; i <= right; i++){
		nus[i] = tmp[i];
	}
}
void Merger(int nus[], int &N, int &sign){
	int tmp[MaxN];
	Mg(nus, tmp, 0, N - 1, sign);
}
void HeapFix(int nus[], int N, int rt, int &sign){
	int l = rt * 2;
	int r = l + 1;
	int pl = rt;
	if(l <= N && compare(nus[l], nus[pl], sign) == false){
		pl = l;
	}
	if(r <= N && compare(nus[r], nus[pl], sign) == false){
		pl = r;
	}
	if(pl != rt){
		nus[pl] ^= nus[rt];
		nus[rt] ^= nus[pl];
		nus[pl] ^= nus[rt];
		HeapFix(nus, N, pl, sign);
	}
}
void buildHeap(int nus[], int &N, int &sign){
	for(int i = N / 2; i >= 1; i--)
		HeapFix(nus, N, i, sign);
}

void Heapst(int nus[], int &N, int &sign){
	for(int i = N; i > 0; i--)
		nus[i] = nus[i - 1];
	buildHeap(nus, N, sign);
	for(int i = N; i > 1; i--){
		nus[i] ^= nus[1];
		nus[1] ^= nus[i];
		nus[i] ^= nus[1];
		HeapFix(nus, i - 1, 1, sign);
	}
	for(int i = 0; i < N; i++){
		nus[i] = nus[i + 1]; 
	}
}
