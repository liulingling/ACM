/*Author: Lingling Liu 
 *Date  : 2012/10/04
 *Name  : Apriori.cpp 
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <time.h>

using namespace std;

//打印中间数据信息for debug，
//将其注销可以不打印调试信息，否者打印 
//#define DEBUG_PRINT_INFO 1
 
#define INF 9999999
//Record类 用于存储每条记录 
class Record{	
	private:
	//itemList 为存储记录列表
	//列表中的元素为整数，其中每个整数表示
	//该记录中的一个商品 
	vector<int> itemList;
	int recordID;	
	public:	
	Record(const int arecordID){
		recordID = arecordID;
		itemList.clear();
	}
	void init(){
		itemList.clear();
	}
	int getRecordID()const{
		return recordID;
	}
	//向记录中添加一个商品 
	void addItem(const int item){
		itemList.push_back(item); 
	}
	//对该记录中商品进行一个排序
	//这样排序可以使得后面判集合
	//包含关系达到O(max(n1,n2)) 
	//n1, n2为两个集合的大小 
	void sortMyself(){
		sort(itemList.begin(), itemList.end());
	}
	//返回该记录所有的商品 
	vector<int>* getList(){
		return &itemList;
	}
	void clear(){
		itemList.clear();
	}
	~Record(){
		clear(); 
	}
};
/*
 *Set类为一个集合，其中包含一些
 *集合的包含和被包含的操作 
 *以及合计的减法操作（在生成规则中会用到） 
 */
class Set{
	//set为用于存放集合元素的容器 
	vector<int> *set;
	int nums;
	//appeartimes为该集合在记录中出现的次数 
	int appeartimes;
	public:
	Set(){
		set = new vector<int>;
		nums = 0;
		appeartimes = 0;
	}
	void setAppearTimes(int times){
		appeartimes = times;
	}
	int getAppearTimes(){
		return appeartimes;
	}
	int getNums(){
		return nums;
	}
	vector<int> * getSet(){
		return set;
	}
	void add(int element){
		(*set).push_back(element);
		nums++;
	}
	//修改集合中指定位置的元素值 
	void setIndex(int index, int value){
		if(index >= nums){
			cout<<"set Index out of nums"<<endl;
			cout<<"index == "<<index<<"nums == "<<nums<<endl;
			return;
		}
		(*set)[index] = value;
	}
	//将set指向容器中的元素复制到p所指向的容器中 
	void fill(vector<int> *p){
		vector<int>::size_type iter;
		if(NULL == p){
			return ;
		}
		for(iter = 0; iter < (*set).size(); iter++){
			(*p).push_back((*set)[iter]);			
		}
	}
	//克隆自己给pset, pset原来的元素将被覆盖 
	void clone(Set *pset){
		if(NULL == pset){
			return;
		}
		vector<int>::size_type iter;
		(*pset).clear();
		
		for(iter = 0; iter < (*set).size(); iter++){
			(*pset).add((*set)[iter]);
		}
	}
	//判断该集合是否包含p中所有的元素 
	bool contain(vector<int> *p){
		if(NULL == p || (*p).size() == 0){
			return true;
		}
		if(NULL == set || (*set).size() < (*p).size()){
			return false;
		}
		vector<int>::size_type i = 0, j = 0;
		while(i < (*set).size() && j < (*p).size()){
			if((*set)[i] < (*p)[j])
				i++;
			else if((*set)[i] == (*p)[j]){
				i++;
				j++;
			}else{
				break;
			}
		}
		if(j >= (*p).size()){
			return true;
		}
		return false;
	}
	//判断该集合是否包含p所指向的集合 
	bool contain(Set *p){
		return contain((*p).getSet());
	}
	//判断该集合是否被包含在p所指向的容器中 
	bool contained(vector<int> *p){
		if(NULL == set || (*set).size() == 0){
			return true;
		}
		if(NULL == p || (*p).size() == 0){
			return false;
		}
		vector<int>::size_type i = 0, j = 0;
		while(i < (*set).size() && j < (*p).size()){
			if((*set)[i] < (*p)[j]){
				break;
			}else if((*set)[i] == (*p)[j]){
				i++;
				j++;
			}else{
				j++;
			}
		}
		if(i >= (*set).size()){
			return true;
		}
		return false;	
	}
	//判断该集合是否被包含在p所指向的集合中 
	bool contained(Set *p){
		return contained((*p).getSet());
	}
	//集合减法set2 = this - set1
	void subtract(Set *set1, Set *set2){
		
		vector<int>::size_type i = 0, j = 0;
		vector<int> * p1 = (*set1).getSet();
		while(i < (*set).size() && j < (*p1).size()){
			if((*set)[i] < (*p1)[j]){
				(*set2).add((*set)[i]);
				i++;
			}else if((*set)[i] = (*p1)[j]){
				i++;
				j++;
			}else{
				cout<<"There exit an error in Subtract"
					<<endl;
				return;
			}
		}
		while(i < (*set).size()){
			(*set2).add((*set)[i]);
			i++;
		}	
	}
	//打印自己的信息 
	void printMyself(){
		cout<<"A set: nums = "<<nums<<endl;
		vector<int>::size_type i;
		for(i = 0; i < (*set).size(); i++){
			cout<<(*set)[i]<<",";
		}
		cout<<" appeartimes = "<<appeartimes;
		cout<<endl;
	}
	void clear(){
		if(set != NULL){
			(*set).clear();
		}
		nums = 0;
	}
	~Set(){
		clear();
	} 
};
/*
 *SetList是包含相同setSize集合的列表
 *即Lk,(k = setSize) 
 *其中，包含生成setSize + 1列表的操作 
 */
class SetList{
	//list为存储集合的容器 
	vector<Set *> * list;
	int setsSize;
	public:
	SetList(){
		list = new vector<Set *>;
		setsSize = 0;		
	}
	SetList(int size){
		setsSize = size;
		list = new vector<Set *>;
	}
	
	int getSize(){
		return setsSize;
	}
	vector<Set *> *getList(){
		return list;
	}
	//向容器Lk中添加一个集合 
	bool add(Set * element){
		if(NULL == element || (*element).getNums() == 0){
			return false;
		}
		if(setsSize == 0){
			
			setsSize = (*element).getNums();
			(*list).push_back(element);
			return true;
		}
		if(setsSize != (*element).getNums()){
			cout<<"SetList's setSize is:"<<setsSize<<endl
				<<"But the Set's setSize is:"
				<<(*element).getNums()<<endl;
			return false;
		}
		(*list).push_back(element);
		return true;
	}
	//Lk -> Ck -> Lk+1
	//f1 为最大一频繁项集
	//records为所有的记录 
	SetList * getNextGenerarion(vector<int> * f1,
					vector<Record *> * records, int sub){
						
		Set * pset = NULL;
		Set * tmpset = NULL; 
		SetList *nextlist = NULL;
		vector<int> *p = NULL;
		
		vector<Set *>::size_type iter;
		vector<int>::size_type i;
		
		p = new vector<int>;
		tmpset = new Set();
		 
		for(iter = 0; iter < (*list).size(); iter++){
			(*p).clear();
			pset = (*list)[iter];
			
			(*pset).fill(p);
			(*pset).clone(tmpset);
			
			int maxelement = (*p)[setsSize - 1];
			(*tmpset).add(maxelement);
			for(i = 0; i < (*f1).size(); i++){
				if((*f1)[i] > maxelement)
					break;
			}
			for(; i < (*f1).size(); i++){
				(*tmpset).setIndex(setsSize, (*f1)[i]);
				bool ok = check(tmpset, records, sub);
				if(ok == true){
					if(NULL == nextlist){
						nextlist = new SetList(setsSize + 1);
					}
					(*nextlist).add(tmpset);
					tmpset = new Set();
					(*pset).clone(tmpset);
					(*tmpset).add(maxelement);
				}
			} 
		}
		delete p;
		delete tmpset;	
		return nextlist;
	}
	//检查pset指向的集合在记录records中出现的次数 
	bool check(Set * pset, vector<Record *> * records, int sub){
		int count = 0;
		vector<Set *>::size_type iter;
		for(iter = 0; iter < (*list).size(); iter++){
			if((*pset).contain((*list)[iter])){
				count++;
				if(count > setsSize){
					break;
				}
			}
		}
		if(count <= setsSize){
			return false;
		}
		count = 0;
		vector<Record *>::size_type i;
		Record *p;
		for(i = 0; i < (*records).size(); i++){
			p = (*records)[i];
			if((*pset).contained((*p).getList())){
				count++;
			}
		}
		(*pset).setAppearTimes(count);
		if(count >= sub)
			return true;
		return false;
	}
	void clear(){
		if(list == NULL)
		{
			setsSize = 0;
			return;
		}
		vector<Set *>::size_type iter;
		for(iter = 0; iter < (*list).size(); iter++){
			(*(*list)[iter]).clear();
			delete (*list)[iter];
		}
		(*list).clear();
		setsSize = 0;
	}
	void printMyself(){
		cout<<"A SetList: setSize = "<<setsSize<<endl;
		vector<Set *>::size_type i;
		for(i = 0; i < (*list).size(); i++){
			cout<<endl;
			(*(*list)[i]).printMyself();
			cout<<endl;
		}
	}
	~SetList(){
		clear();
		delete list;
		list = NULL; 
	}	
};
/*
 *Rules 为生成的规则集（A -> B） 
 *可用于推荐系统 
 */
class Rules{
	Set *a;
	Set *b;
	double confidence;
	int aTimes;
	int abTimes;
	public:
	Rules(){
		a = NULL;
		b = NULL;
	} 
	Rules(Set *pa, Set *pb,int at, int abt, double cf){
		a = pa;
		b = pb;
		confidence = cf;
		aTimes = at;
		abTimes = abt;
	}
	void setA(Set *pa){
		a = pa;
	}
	Set * getA(){
		return a;
	}
	void setB(Set *pb){
		b = pb;
	}
	Set * getB(){
		return b;
	}
	void setAt(int at){
		aTimes = at;
	}
	int getAt(){
		return aTimes;
	}
	void setAbt(int abt){
		abTimes = abt;
	}
	int getAbT(){
		return abTimes;
	}
	void setConfidence(double cf){
		confidence = cf;
	} 
	double getConfidence(){
		return confidence;
	}
	void printMyself(){
		cout<<"A rule begin"<<endl;
		(*a).printMyself();
		cout<<"=>>>>>>>>"<<endl;
		(*b).printMyself();
		cout<<"A rule end"<<endl; 
	}
	~Rules(){
		if(a != NULL){
			delete a;
			a = NULL;
		}
		if(b != NULL){
			delete b;
			b = NULL;
		}
	}	 
};
/*
 *InputNode 定义一个数据源，其从trainFileName中读取数据
 *并将每条记录存入容器recordList中 
 *这里每条记录由该记录中商品名所对应的商品ID组成
 *利用map将商品名映射成一个整数 
 */ 
class InputNode{
	private:
	string trainFileName;
	int CountItem;
	//用于存储所有的记录 
	vector<Record *> recordList;
	//vtimes用于记录每个item出现的次数，用于生成最大项 
	vector<int> vtimes;
	//vitermName用于记录每个item的名称 
	vector<string> vitemName;
	//用map将一个字符串映射成一个整数 
	map<string, int> items;
	/*
	 *读取一条记录，将记录加入recordList 
	 */ 
	bool readRecord(int recordID, string oneLine){
		Record *p = new Record(recordID);
		istringstream wordin(oneLine);
		string oneItem;
		string blank = "";
		
		while(getline(wordin, oneItem, '\t')){
			
			if(oneItem.compare(blank) == 0)
				continue;
			
			int itemID = map_insert(oneItem);
			
			if(itemID == -1){
				cout<<"Error founded at readRecord"
					<<"while insert:"<<oneItem<<endl; 
			}else{
				p->addItem(itemID);
			}
		}
		p->sortMyself();
		recordList.push_back(p);
		
		return true;
	}
	int map_insert(string item){
		
		map<string,int>::iterator iter;
		iter = items.find(item);
		
		#ifdef DEBUG_PRINT_INFO
		cout<<item<<endl;
		#endif
		if(iter != items.end()){
			#ifdef DEBUG_PRINT_INFO
			cout<<"exit"<<endl;
			#endif
			vtimes[iter->second - 1]++;
			return iter->second;
		}else{
			#ifdef DEBUG_PRINT_INFO
			cout<<"not exit"<<endl;
			#endif
			CountItem++;
			items.insert(map<string,int>::value_type(item, CountItem));
			vitemName.push_back(item);
			vtimes.push_back(1);
			return CountItem;
		}
	}
	
	public:
		
	InputNode(const string name){
		trainFileName = name;
		recordList.clear();
		items.clear();
		vtimes.clear();
		CountItem = 0;
	}
	//返回商品种类数 
	int getCountItem()const{
		return CountItem;
	}
	//返回指定商品ID的商品名 
	string getItemName(int index){
		return vitemName[index - 1];
	}
	//返回指定商品ID,出现的次数
	int getItemTimes(int index) const{
		return vtimes[index - 1];
	} 
	//返回记录表 
	vector<Record *> * getRecordList(){
		return &recordList;
	}
	//返回所有商品出现的次数
	vector<int> * getVtimes(){
		return &vtimes;
	}
	//返回商品名列表
	vector<string> * getVitemName(){
		return &vitemName;
	}
	/*
	 *从文件中读取全部记录 
	 */
	bool readIn(){	
		fstream  fin;
		fin.open(trainFileName.c_str());
		string oneLine;
		
		if(!fin){
			cout<<"can't open file: "<<trainFileName<<endl;
			return false;
		}	
		int recordID = 0;
		while(getline(fin, oneLine, '\n')){
			#ifdef DEBUG_PRINT_INFO
				cout<<"line: "<<oneLine<<endl;
			#endif
			//if(oneLine.compare("") == 0)
			//	continue;
			recordID++;
			if(!readRecord(recordID, oneLine)){
				cout<<"The file does't meet the format"<<endl;
				return false;
			} 
		}
		return true;
	}
	void clear(){
		items.clear();
		vtimes.clear();
		vitemName.clear();
		Record * pr = NULL;
		for(vector<Record *>::size_type iter = 0;
				iter < recordList.size(); iter++){
			pr = recordList[iter];
			(*pr).clear();
			delete pr;
		}
		recordList.clear(); 
	}
	/*
	 *printMap用于调试，打印items中的信息
	 */
	void printMap(){
		map<string, int>::iterator iter;
		for(iter = items.begin(); iter != items.end(); iter++){
			cout<<"one <item, key>"<<endl;
			cout<<iter->first<<"  Mapping  ";
			cout<<iter->second<<endl;
		}
	}	
	/*
	 *printVtimes用于调试，打印vtimes中的信息
	 */
	void printVtimes(){
		vector<int>::size_type iter;
		vector<string>::size_type i;
		for(iter = 0, i = 0; iter != vtimes.size(); 
					iter++, i++){
			cout<<vitemName[i]<<" appeared "
				<<vtimes[iter]<<" times"<<endl;
		
		}
	}
	
	/*
	 *printVitemName用于调试，打印vitemName中的信息 
	 */
	void printVitemName(){
		Record * p = NULL;
		int tmpid;
		for(vector<Record *>::size_type iter = 0; iter != recordList.size(); iter++){
			p = recordList[iter];
			vector<int> * list = (*p).getList();
			for(vector<int>::size_type iter1 = 0; iter1 != (*list).size(); iter1++){
				tmpid = (*list)[iter1];
				cout<<getItemName(tmpid)<<"	";
			}	
			printf("\n");
		}
	}
	~InputNode(){
		clear();
	}
};
/*
 *Apriori类实现Apriori算法
 *所有的频繁项集存在ans中
 *产生的规则存储在rulesList中 
 */
class Apriori{
	//定义一个枚举类型
	//SUB_INT 表示此时最小支持砝值为整数
	//SUB_DOUBLE 表示此时最小支持砝值为小数 
	enum Apriori_State{SUB_INT, SUB_DOUBLE};
	double min_sub;
	int min_sub_int;
	//该变量为最小置信度 
	double min_confidence; 
	//产生最大项的模
	//缺省值为INF 
	int Max_Set_Size;
	
	//指向input中所有的记录 
	vector<Record *> * records;
	Apriori_State state;
	//指向输入节点 
	InputNode * input;
	//为1频繁项集 
	vector<int> frequent_1_sets;
	//ans存储所有的频繁项集 
	vector<SetList *> ans;
	//rulesList存储产生的所有规则 
	vector<Rules *> rulesList;
	
	public:
	Apriori(double sub = 0.3, double confidence = 0.85){
		state = SUB_DOUBLE;
		min_sub = sub;
		min_confidence = confidence;
		records = NULL;
		input = NULL;
		frequent_1_sets.clear();
		ans.clear();
		rulesList.clear(); 
		Max_Set_Size = INF;	
	}
	
	Apriori(int sub, double confidence = 0.85){
		state = SUB_INT;
		min_sub_int = sub;
		min_confidence = confidence;
		records = NULL;
		input = NULL;
		frequent_1_sets.clear();
		ans.clear();
		rulesList.clear();
		Max_Set_Size = INF;
	}
	bool setInputNode(InputNode *pin){
		input = pin;
		records = (*input).getRecordList();
		int totalRecord = (*records).size();
		if(state == SUB_DOUBLE){
			min_sub_int = (int)(min_sub * totalRecord);
		}
		return true;
	}
	//找出所有的1频繁项集 
	void find_Frequent_1_Itemsets(){
		if(NULL == input){
			cout<<"No input Node"<<endl;
			return;
		}else{
			vector<int> *vtimes = (*input).getVtimes();
			for(vector<int>::size_type iter = 0; iter <(*vtimes).size(); iter++){
				if((*vtimes)[iter] >= min_sub_int){
					//如果该商品出现的次数大于min_sub_int那么
					//将该商品的ID加入频繁一项集frequent_1_sets 
					frequent_1_sets.push_back(iter + 1);
				}
			} 
		} 
	}
	void setMin_Sub(double sub){
		min_sub = sub;
		state = SUB_DOUBLE;
	}
	double getMin_Sub()const{
		if(state == SUB_INT){
			if((*records).size() == 0){
				return 0.0;
			}else{
				return (min_sub_int + 0.0) / (*records).size();
			}
		}
		return min_sub;
	}
	void setMin_Sub_Int(int sub){
		min_sub_int = sub;
		state = SUB_INT; 
	}
	int getMin_Sub_Int()const{
		if(state == SUB_DOUBLE){
			return (int)(min_sub * (*records).size());
		}
		return min_sub_int;
	}
	void setMin_Confidence(double confidence){
		min_confidence = confidence;
	}
	double getMin_Confidence()const{
		return min_confidence;
	}
	void setMax_Set_Size(int size){
		Max_Set_Size = size;
	}
	void work(){
		
		//生成1频繁项集 
		find_Frequent_1_Itemsets();
		
		//一下代码将1频繁项集加入到ans容器中 
		SetList *psetlist = NULL;
		psetlist = new SetList();
		vector<int>::size_type i;
		vector<int> *vtimes = (*input).getVtimes();
		for(i = 0; i < frequent_1_sets.size(); i++){
			Set *pset = new Set();
			(*pset).add(frequent_1_sets[i]);
			(*pset).setAppearTimes((*vtimes)[i]);
			(*psetlist).add(pset); 
		}
		ans.push_back(psetlist);
		if(frequent_1_sets.size() < 2)
			return;
		int size;
		//一下for询问生成大小为size的频繁项集 
		for(size = 2; size < Max_Set_Size; size++){
			
			//由 Ck 生成 Ck+1 
			SetList * tmplist = (*psetlist).getNextGenerarion(&frequent_1_sets,
						records, min_sub_int);
			if(tmplist == NULL)
			{
				//如果tmplist为空那么之后的所有更大的项集都 
				//肯定不是频繁项集
				//所以可以直接跳出循环 
				break;
			}else{
				//将生成的Ck+1 加入 ans中 
				ans.push_back(tmplist);
			}
			psetlist = tmplist;	
		}
		//根据频繁项集产生规则 
		generateRules();
	}
	//生成规则 
	void generateRules(){
		vector<SetList *>::size_type i, j;
		SetList *l1, *l2;
		vector<Set *>::size_type is, js;
		vector<Set *> *s1, *s2;
		Set *e1, *e2;
		int ta, tab;
		double cf;
		for(i = 0; i < ans.size(); i++){
			l1 = ans[i];
			s1 = (*l1).getList();
			for(j = i + 1; j < ans.size(); j++){
				l2 = ans[j]; 
				s2 = (*l2).getList();			
				for(is = 0; is < (*s1).size(); is++){
					e1 = (*s1)[is];
					ta = (*e1).getAppearTimes();
					
					for(js = 0; js < (*s2).size(); js++){
						e2 = (*s2)[js];
						tab = (*e2).getAppearTimes();
						//剪枝 
						if(tab < ta * min_confidence)
							break;	
						if(!(*e2).contain(e1))
							break;
						Set *tmpa = new Set();
						Set *tmpb = new Set();
						(*e1).clone(tmpa);
						(*e2).subtract(e1, tmpb);
						Rules *r = new Rules(tmpa, tmpb, ta, tab, (tab + 0.0)/ta);
						rulesList.push_back(r);	
					}	
				}
			}
		}
	}
	//打印ans容器中的内容 
	void printAns(){
		cout<<"*********begin*********"<<endl;
		vector<SetList *>::size_type i;
		vector<Set *>::size_type j;
		vector<int>::size_type k;
		
		vector<Set *> *pl;
		Set *pset;
		vector<int> *pvt;
		vector<string> *names = (*input).getVitemName();
		
		for(i = 0; i < ans.size(); i++){
			
			cout<<"***************"<<endl;
			//(*ans[i]).printMyself();
			pl = (*ans[i]).getList();
			for(j = 0; j < (*pl).size(); j++){
				pset = (*pl)[j];
				pvt = (*pset).getSet();
				cout<<"(";
				for(k = 0; k < (*pvt).size(); k++){
					if(k != 0)
						cout<<",";
					cout<<(*names)[(*pvt)[k] - 1];
				}
				cout<<") appeared ";
				cout<<(*pset).getAppearTimes()<<endl;
			}
			cout<<"***************"<<endl;
		}
		cout<<"*********end**********"<<endl;
	}
	void clear(){
		min_sub = 0.3;
		min_confidence = 0.85;
		records = NULL;
		input = NULL;
		frequent_1_sets.clear();
		ans.clear();
		rulesList.clear();
	}
	//打印频繁项集中的内容 
	void printFrequent_1_sets(){
		vector<int>::size_type iter;
		for(iter = 0; iter < frequent_1_sets.size(); iter++){
			cout<<(*input).getItemName(frequent_1_sets[iter])
				<<" "<<(*input).getItemTimes(frequent_1_sets[iter])
				<<"times"<<endl;
		}
	}
    /*
	 *printRules用于打印生成的规则 
	 */
	void printRules(){
		Rules *p;
		vector<Rules *>::size_type i;
		vector<string> * ps = (*input).getVitemName();
		int total = (*(*input).getRecordList()).size();

		for(i = 0; i < rulesList.size(); i++){
			p = rulesList[i];
			vector<int> *s1 , *s2;
			s1 = (*(*p).getA()).getSet();
			s2 = (*(*p).getB()).getSet();
			vector<int>::size_type i;
			cout<<"(";
			for(i = 0; i < (*s1).size(); i++){
				cout<<" "<<(*ps)[(*s1)[i] - 1];
			}
			cout<<") ==> ";
			
			cout<<"(";
			for(i = 0; i < (*s2).size(); i++){
				cout<<" "<<(*ps)[(*s2)[i] - 1];
			}
			cout<<")"<<" "
				<<"sub(A=>B):"<<((*p).getAbT() + 0.0) / total<<" "
				<<"sub(A):"<<((*p).getAt() + 0.0) / total<<" "
				<<"conf:"<<(*p).getConfidence()<<endl;
		} 
	}
	~Apriori(){
		clear();
	} 
};
int main(){
	cout<<"输入文件格式的正则表达式形式"<<endl; 
	cout<<"Input File Format:"<<endl;
	cout<<"InputFile -> Record \'\\n\' InputFile | empty"<<endl;
	cout<<"Record -> Good \'\\t\' Record | empty"<<endl;
	cout<<"Good -> [a-zA-Z1-9]*"<<endl;
	
	cout<<"Please Input Inputfile Name: (in1.txt, in.txt)";
	string name = "in1.txt";
	cin>>name;
	cout<<"Please Input Sub(range in [0.0-1.0])" 
		<<"and Confidence(range in[0.0-1.0]):";
	double min_sub;
	double confidence;
	cin>>min_sub;
	cin>>confidence; 
	//设置输出文件 
	//freopen("out1.txt", "w", stdout);
	int time_start = clock();
	InputNode input(name);
	bool ok = input.readIn();
	int time_endinput = clock();
	int time_endApriori;
	
	if(!ok){
		cout<<"There is something wrong with file:"
			<<name<<endl;
	}else{
		/*cout<<"***********printMap begin***********"<<endl;
		input.printMap();//打印商品名,商品ID映射表 
		cout<<"***********printMap end*************"<<endl;*/
		
		/*cout<<"*********printVtimes begin**********"<<endl;
		input.printVtimes();//打印每个商品出现的次数 
		cout<<"**********printVtimes end***********"<<endl;*/
		
		/*cout<<"********printVitemName begin********"<<endl;
		input.printVitemName();//打印所有的记录 
		cout<<"*********printVitemName end*********"<<endl;*/
		
		//设置最小支持度，最小置信度 
		Apriori apriori(min_sub, confidence);
		//设置生成频繁项的上界 
		//apriori.setMax_Set_Size(5);//缺省值为INF 
		//将输入节点与apriori连接 
		apriori.setInputNode(&input);
		//执行apriori算法 
		apriori.work();
		time_endApriori = clock();
		cout<<"**********frequent Itemsets begin************"<<endl;
		apriori.printAns();//打印所有的频繁项 
		cout<<"**********frequent Itemsets end**************"<<endl;
		cout<<"***********printRules Begin******************"<<endl;
		apriori.printRules();//打印生成的规则 
		cout<<"***********printRules end*******************"<<endl;
		
		cout<<"Load File Cost: "<<time_endinput - time_start
			<<"ms"<<endl;
		cout<<"Apriori Cost:"<<time_endApriori - time_endinput
			<<"ms"<<endl;
		cout<<"total Cost:"<<time_endApriori - time_start
			<<"ms"<<endl;  
	}
	 
	system("pause");
	return 0;
}
