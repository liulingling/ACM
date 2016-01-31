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

//��ӡ�м�������Ϣfor debug��
//����ע�����Բ���ӡ������Ϣ�����ߴ�ӡ 
//#define DEBUG_PRINT_INFO 1
 
#define INF 9999999
//Record�� ���ڴ洢ÿ����¼ 
class Record{	
	private:
	//itemList Ϊ�洢��¼�б�
	//�б��е�Ԫ��Ϊ����������ÿ��������ʾ
	//�ü�¼�е�һ����Ʒ 
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
	//���¼�����һ����Ʒ 
	void addItem(const int item){
		itemList.push_back(item); 
	}
	//�Ըü�¼����Ʒ����һ������
	//�����������ʹ�ú����м���
	//������ϵ�ﵽO(max(n1,n2)) 
	//n1, n2Ϊ�������ϵĴ�С 
	void sortMyself(){
		sort(itemList.begin(), itemList.end());
	}
	//���ظü�¼���е���Ʒ 
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
 *Set��Ϊһ�����ϣ����а���һЩ
 *���ϵİ����ͱ������Ĳ��� 
 *�Լ��ϼƵļ��������������ɹ����л��õ��� 
 */
class Set{
	//setΪ���ڴ�ż���Ԫ�ص����� 
	vector<int> *set;
	int nums;
	//appeartimesΪ�ü����ڼ�¼�г��ֵĴ��� 
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
	//�޸ļ�����ָ��λ�õ�Ԫ��ֵ 
	void setIndex(int index, int value){
		if(index >= nums){
			cout<<"set Index out of nums"<<endl;
			cout<<"index == "<<index<<"nums == "<<nums<<endl;
			return;
		}
		(*set)[index] = value;
	}
	//��setָ�������е�Ԫ�ظ��Ƶ�p��ָ��������� 
	void fill(vector<int> *p){
		vector<int>::size_type iter;
		if(NULL == p){
			return ;
		}
		for(iter = 0; iter < (*set).size(); iter++){
			(*p).push_back((*set)[iter]);			
		}
	}
	//��¡�Լ���pset, psetԭ����Ԫ�ؽ������� 
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
	//�жϸü����Ƿ����p�����е�Ԫ�� 
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
	//�жϸü����Ƿ����p��ָ��ļ��� 
	bool contain(Set *p){
		return contain((*p).getSet());
	}
	//�жϸü����Ƿ񱻰�����p��ָ��������� 
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
	//�жϸü����Ƿ񱻰�����p��ָ��ļ����� 
	bool contained(Set *p){
		return contained((*p).getSet());
	}
	//���ϼ���set2 = this - set1
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
	//��ӡ�Լ�����Ϣ 
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
 *SetList�ǰ�����ͬsetSize���ϵ��б�
 *��Lk,(k = setSize) 
 *���У���������setSize + 1�б�Ĳ��� 
 */
class SetList{
	//listΪ�洢���ϵ����� 
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
	//������Lk�����һ������ 
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
	//f1 Ϊ���һƵ���
	//recordsΪ���еļ�¼ 
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
	//���psetָ��ļ����ڼ�¼records�г��ֵĴ��� 
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
 *Rules Ϊ���ɵĹ��򼯣�A -> B�� 
 *�������Ƽ�ϵͳ 
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
 *InputNode ����һ������Դ�����trainFileName�ж�ȡ����
 *����ÿ����¼��������recordList�� 
 *����ÿ����¼�ɸü�¼����Ʒ������Ӧ����ƷID���
 *����map����Ʒ��ӳ���һ������ 
 */ 
class InputNode{
	private:
	string trainFileName;
	int CountItem;
	//���ڴ洢���еļ�¼ 
	vector<Record *> recordList;
	//vtimes���ڼ�¼ÿ��item���ֵĴ������������������ 
	vector<int> vtimes;
	//vitermName���ڼ�¼ÿ��item������ 
	vector<string> vitemName;
	//��map��һ���ַ���ӳ���һ������ 
	map<string, int> items;
	/*
	 *��ȡһ����¼������¼����recordList 
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
	//������Ʒ������ 
	int getCountItem()const{
		return CountItem;
	}
	//����ָ����ƷID����Ʒ�� 
	string getItemName(int index){
		return vitemName[index - 1];
	}
	//����ָ����ƷID,���ֵĴ���
	int getItemTimes(int index) const{
		return vtimes[index - 1];
	} 
	//���ؼ�¼�� 
	vector<Record *> * getRecordList(){
		return &recordList;
	}
	//����������Ʒ���ֵĴ���
	vector<int> * getVtimes(){
		return &vtimes;
	}
	//������Ʒ���б�
	vector<string> * getVitemName(){
		return &vitemName;
	}
	/*
	 *���ļ��ж�ȡȫ����¼ 
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
	 *printMap���ڵ��ԣ���ӡitems�е���Ϣ
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
	 *printVtimes���ڵ��ԣ���ӡvtimes�е���Ϣ
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
	 *printVitemName���ڵ��ԣ���ӡvitemName�е���Ϣ 
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
 *Apriori��ʵ��Apriori�㷨
 *���е�Ƶ�������ans��
 *�����Ĺ���洢��rulesList�� 
 */
class Apriori{
	//����һ��ö������
	//SUB_INT ��ʾ��ʱ��С֧����ֵΪ����
	//SUB_DOUBLE ��ʾ��ʱ��С֧����ֵΪС�� 
	enum Apriori_State{SUB_INT, SUB_DOUBLE};
	double min_sub;
	int min_sub_int;
	//�ñ���Ϊ��С���Ŷ� 
	double min_confidence; 
	//����������ģ
	//ȱʡֵΪINF 
	int Max_Set_Size;
	
	//ָ��input�����еļ�¼ 
	vector<Record *> * records;
	Apriori_State state;
	//ָ������ڵ� 
	InputNode * input;
	//Ϊ1Ƶ��� 
	vector<int> frequent_1_sets;
	//ans�洢���е�Ƶ��� 
	vector<SetList *> ans;
	//rulesList�洢���������й��� 
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
	//�ҳ����е�1Ƶ��� 
	void find_Frequent_1_Itemsets(){
		if(NULL == input){
			cout<<"No input Node"<<endl;
			return;
		}else{
			vector<int> *vtimes = (*input).getVtimes();
			for(vector<int>::size_type iter = 0; iter <(*vtimes).size(); iter++){
				if((*vtimes)[iter] >= min_sub_int){
					//�������Ʒ���ֵĴ�������min_sub_int��ô
					//������Ʒ��ID����Ƶ��һ�frequent_1_sets 
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
		
		//����1Ƶ��� 
		find_Frequent_1_Itemsets();
		
		//һ�´��뽫1Ƶ������뵽ans������ 
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
		//һ��forѯ�����ɴ�СΪsize��Ƶ��� 
		for(size = 2; size < Max_Set_Size; size++){
			
			//�� Ck ���� Ck+1 
			SetList * tmplist = (*psetlist).getNextGenerarion(&frequent_1_sets,
						records, min_sub_int);
			if(tmplist == NULL)
			{
				//���tmplistΪ����ô֮������и������� 
				//�϶�����Ƶ���
				//���Կ���ֱ������ѭ�� 
				break;
			}else{
				//�����ɵ�Ck+1 ���� ans�� 
				ans.push_back(tmplist);
			}
			psetlist = tmplist;	
		}
		//����Ƶ����������� 
		generateRules();
	}
	//���ɹ��� 
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
						//��֦ 
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
	//��ӡans�����е����� 
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
	//��ӡƵ����е����� 
	void printFrequent_1_sets(){
		vector<int>::size_type iter;
		for(iter = 0; iter < frequent_1_sets.size(); iter++){
			cout<<(*input).getItemName(frequent_1_sets[iter])
				<<" "<<(*input).getItemTimes(frequent_1_sets[iter])
				<<"times"<<endl;
		}
	}
    /*
	 *printRules���ڴ�ӡ���ɵĹ��� 
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
	cout<<"�����ļ���ʽ��������ʽ��ʽ"<<endl; 
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
	//��������ļ� 
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
		input.printMap();//��ӡ��Ʒ��,��ƷIDӳ��� 
		cout<<"***********printMap end*************"<<endl;*/
		
		/*cout<<"*********printVtimes begin**********"<<endl;
		input.printVtimes();//��ӡÿ����Ʒ���ֵĴ��� 
		cout<<"**********printVtimes end***********"<<endl;*/
		
		/*cout<<"********printVitemName begin********"<<endl;
		input.printVitemName();//��ӡ���еļ�¼ 
		cout<<"*********printVitemName end*********"<<endl;*/
		
		//������С֧�ֶȣ���С���Ŷ� 
		Apriori apriori(min_sub, confidence);
		//��������Ƶ������Ͻ� 
		//apriori.setMax_Set_Size(5);//ȱʡֵΪINF 
		//������ڵ���apriori���� 
		apriori.setInputNode(&input);
		//ִ��apriori�㷨 
		apriori.work();
		time_endApriori = clock();
		cout<<"**********frequent Itemsets begin************"<<endl;
		apriori.printAns();//��ӡ���е�Ƶ���� 
		cout<<"**********frequent Itemsets end**************"<<endl;
		cout<<"***********printRules Begin******************"<<endl;
		apriori.printRules();//��ӡ���ɵĹ��� 
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
