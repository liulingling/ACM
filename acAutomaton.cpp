#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
const int MaxN = 100010;

//定义一个term结构体，用于存放trie树节点信息 
struct Term{
	string word;
	bool isWords;
	int fail;
	vector<int> cs;
	Term(){
		isWords = false;
		fail = 0;
		cs.clear();
	}
	void init(){
		isWords = false;
		fail = 0;
		cs.clear();
	}
	int compare(const string & wd){
		return word.compare(wd);
	}
};

class Automaton{
	Term* tm_list_p;
	int cntp;
	int root;
	
	int* acQ_p;
	string* termstr_p;
	int N;
	//二分查找一个节点中是否包含tm子节点 
	int inChilds(Term &rt, const string & tm){
		int left = 0, right = rt.cs.size() - 1;
		int index, mid, tmp;
		while(left <= right){
			mid = (left + right) / 2;
			tmp = (tm_list_p + rt.cs[mid])->compare(tm);
			if(tmp == 0)
				return mid;
			if(tmp > 0){
				right = mid - 1;
			}else{
				left = mid + 1;
			}
		}
		return -1;
	}
	//将一个串vs插入到Trie树中 
	void insert(const vector<string> &vs){
		int p = root;
		int tmp;
		int len = vs.size();
		string aword;
		for(int i = 0; i < len; i++){
			aword = vs[i];
			tmp = inChilds(*(tm_list_p + p), aword);
			if(tmp == -1){
				++cntp;
				(tm_list_p + cntp)->word = aword;
				(tm_list_p + p)->cs.push_back(cntp);
				tmp = (tm_list_p + p)->cs.size() - 1;
			}
			p = (tm_list_p + p)->cs[tmp];
		}
		(tm_list_p + p)->isWords = true;
	}
	//直接在tire树上查找是否含有串vs 
	int search(const vector<string> &vs){
		int p = root;
		int tmp;
		int len = vs.size();
		string aword;
		for(int i = 0; i < len; i++){
			aword = vs[i];
			tmp = inChilds(*(tm_list_p + p), aword);
			if(tmp == -1){
				return 0; 
			}
			p = (tm_list_p + p)->cs[tmp];
		}
		if((tm_list_p + p)->isWords == true)
			return 1;
		return 0;
	}
	//定义一个比较算子 
	static bool cmp(const string &lstr, const string &rstr){
		return lstr.compare(rstr) < 0;
	}
	//在Trie树中建立失败指针，形成自动机 
	void buildAcTo(){
		int front = 0, rear = 0;
		string atoken;	
		*(acQ_p + 0) = root;
		(tm_list_p + root)->fail = -1;
		while(front <= rear){
			int pre = *(acQ_p + front);
			front++;
			int ct, nc = (tm_list_p + pre)->cs.size();
			for(int i = 0; i < nc; i++){
				ct = (tm_list_p + pre)->cs[i];
				atoken = (tm_list_p + ct)->word;
				if(pre == root){
					(tm_list_p + ct)->fail = root;
				}
				else{
					int p = (tm_list_p + pre)->fail;
					while(p != -1){
						int index = inChilds(*(tm_list_p + p), atoken);
						if(index != -1){
							(tm_list_p + ct)->fail = (tm_list_p + p)->cs[index];
							break;
						}
						p = (tm_list_p + p)->fail;
					}
					if(p == -1){
						(tm_list_p + ct)->fail = root;
					}
				}
				++rear;
				*(acQ_p + rear) = ct;
			}
		}
	}
	public:
	Automaton(Term tm_list[], int acQ[], string termstr[]){
		cntp = 0;
		root = 0;
		N = 0;
		tm_list_p = tm_list;
		acQ_p = acQ;
		termstr_p = termstr; 
	}
	void init(){
		cntp = 0;
		root = 0;
		for(int i = 0; i < MaxN; i++){
			(tm_list_p + i)->init();
		}
	}
	//查询一个微薄中包含多少个模式串 
	int Query(const vector<string> &vs){
		int length = vs.size();
		int pre = root;
		int cnt = 0, ct, index;
		vector<int> ans;
		
		string atoken;
		for(int i = 0; i < length; i++){
			atoken = vs[i];
			index = -1;
			while(1)
			{
				index = inChilds(*(tm_list_p + pre), atoken);
				if(index != -1 || pre == root)
					break;
				pre = (tm_list_p + pre)->fail;
			}
			if(index != -1){
				pre = (tm_list_p + pre)->cs[index];
				if((tm_list_p + pre)->isWords == true){
					ans.push_back(pre);
				}
			}else{
				pre = root;
			}
		}
		cnt = ans.size();
		ans.clear(); 
		return cnt;
	}
	//读取词汇表，将每一个模式串加入到Trie树中 
	void readVocabulary(string tvf){
		
		ifstream fin(tvf.c_str());
		N = 0;
		if(NULL == fin){
			cout<<"can't find file:"<<tvf<<endl;
		}
		string oneterm;
		while(getline(fin, *(termstr_p + N), '\n')){
			N++;
		}
		sort(termstr_p, termstr_p + N, cmp);
		string  word;
		vector<string> vt;
		
		int index = 0;
		while(index < N){
			oneterm = *(termstr_p + index);
			index++;
			vt.clear();
			stringstream words(oneterm);
			while(getline(words, word, ' ')){
				vt.push_back(word);
			}
			insert(vt);
			/*cout<<"one word"<<endl; 
			for(int i = 0; i < vt.size(); i++){
				cout<<vt[i]<<endl;
			}*/
		}
		fin.close();
		buildAcTo();
	}
};

Term tm_list[MaxN];
int acQ[MaxN];
string termstr[MaxN];

int main(){ 
	string vtf = "dicnary.txt";
	Automaton myActo(tm_list, acQ, termstr);
	myActo.readVocabulary(vtf);
	
	string testStr = "你 好 我 是 计 算 机 系 的， 我 的 名 字 是 刘 岭 岭";
	vector<string> vt;
	string word;
	
	//cout<<testStr.size()<<endl;
	for(int i = 0; i < testStr.size(); i++){
		//int c = testStr[i];
		//cout<<c<<endl;
	}
	stringstream words(testStr);
	while(getline(words, word, ' ')){
		vt.push_back(word);
	}
	int nums = myActo.Query(vt);
	cout<<nums<<endl;
	system("pause"); 
	/*string mbf = "short.txt";
	ifstream mbf_in(mbf.c_str());
	
	string mbfo = "short_out2.txt";
	ofstream fout(mbfo.c_str());
	
	if(NULL == mbf_in){
		cout<<"can't open file:"<<mbf<<endl;
	}else{
		string oneline, oneterm, strID;
		string word;
		vector<string> vt;
		
		while(getline(mbf_in, oneline, '\n')){
			stringstream firstStep(oneline);
			getline(firstStep, strID, '\t');
			getline(firstStep, oneterm, '\t');
						
			vt.clear();
			stringstream words(oneterm);
			while(getline(words, word, ' ')){
				vt.push_back(word);
			}
			int nums = myActo.Query(vt);

			if(nums > 0){
				fout<<strID<<"\t"<<oneterm<<"\t"<<nums<<endl;
			}
		}	
	}*/
	return 0;
}
