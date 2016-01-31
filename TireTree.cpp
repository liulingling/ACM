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
private:
	Term* tm_list_p; 
	int cntp;
	int root;
	
	int* acQ_p;
	string* termstr_p;
	int N;

private:
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
public:
	Automaton(Term tm_list[], string termstr[]){
		cntp = 0;
		root = 0;
		N = 0;
		tm_list_p = tm_list;
		termstr_p = termstr; 
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
		}
		fin.close();
	}
	void segMent(vector<string>& sentence, vector<string>& segs){
		int length = sentence.size();
		int pre = root;
		segs.clear();
		int i = 0, j, index, pos = 0;
		string atoken;
		for(i = 0; i < length; ){
			pos = i, j = i;
			pre = root;
			do{
				atoken = sentence[j];
				index = inChilds(*(tm_list_p + pre), atoken);
				if(index != -1){
					pre = (tm_list_p + pre)->cs[index];
					if((tm_list_p + pre)->isWords == true){
						pos = j;
					}
					j++;
				}else{
					break;
				}
			}while(j < length);
			string tmp = "";
			for(j = i; j <= pos; j++){
				tmp += sentence[j];
			}
			segs.push_back(tmp);
			i = pos + 1;
		}
	}
};

Term tm_list[MaxN];
string termstr[MaxN];

int main(){
	string vtf = "dicnary.txt";
	Automaton myActo(tm_list, termstr);
	myActo.readVocabulary(vtf);
	
	string testStr = "你 好 我 是 计 算 机 系 的 ， 我 的 名 字 是 刘 岭 岭";
	
	string testStr1 = "hello, I 是刘岭岭";
	string testStr2;
	char buffer[1024];
	strcpy(buffer, testStr1.c_str());
	int i;
	string word;
	int len = strlen(buffer);
	for(i = 0; i < len; i++){
		if(buffer[i] >= 0 && buffer[i] <= 127 ){	
			word = buffer[i];
			cout<<word<<endl;
		}else{
			word = buffer[i];
			word += buffer[i + 1];
			i++;
			cout<<word<<endl;
		}	 
	}
	 
	/*vector<string> vt;
	string word;
	
	stringstream words(testStr);
	while(getline(words, word, ' ')){
		vt.push_back(word);
	}
	
	vector<string> ans;
	myActo.segMent(vt, ans);
	
	for(int i = 0; i < ans.size(); i++){
		cout<<ans[i]<<endl;
	}*/
	
	system("pause");
	return 0;
}
