#include <bits/stdc++.h>
//#include <windows.h>
using namespace std;
#define ll long long
/*
数据文件：
	fenci2,4,5,6,7,8,9,10,11.txt
数据处理：
	1.输出每个字的log(概率)
	2.输出每 2个字组合的log(概率)
	3.输出每 3个字组合的log(概率)
	
处理步骤：
	读入汉字表，
	对于每个数据文件：
		统计单字次数
		统计2字次数
		统计3字次数
		(分词暂不统计，要用分词库单独处理数据)
输出： 
	log(每个字的出现概率)
	log(P(字2|字1))
	log(P(字3|字1字2))
		P(字2|字1)=P(字1字2)/P(字1)
		P(字3|字1字2)=P(字1字2字3)/P(字1字2)
*/
set<string>word;
unordered_map<string,ll>P1;// 单词次数
unordered_map<string,ll>P2;// 词1词2次数 
unordered_map<string,double>PP1;// log单词频率
unordered_map<string,double>PP2;// log词1词2频率
// if (access(fname.data(), 0) == 0)
// vector<string> fnames={"2.txt","4.txt","5.txt","6.txt","7.txt","8.txt","9.txt","10.txt","11.txt","2008.txt","2009.txt","2010.txt","2011.txt","2012.txt","2013.txt","2014.txt","2015.txt","2017.txt","2018.txt"};
vector<string> fnames={"2.txt","4.txt","5.txt","6.txt","7.txt","8.txt","9.txt","10.txt","11.txt"};

//void test(){
//	for(int i=0;i<fnames.size();i++){
//		ifstream ifs("o_"+fnames[i]);
//		string s;
//		getline(ifs,s);
//		for(int j=0;j<8;j++)cout<<s[j];
//		cout<<endl;
//	}
//}
//void old(){
//	ifstream ifs1("一二级汉字表.txt");
////	ifstream ifs2("拼音汉字表.txt");
//	string s;
//	int k=0;
//	while(getline(ifs2,s)){
//		stringstream ss;
//		ss<<s;
//		ss>>s;
//		while(ss>>s)k++,se.insert(s);
//	}
//	cout<<k<<endl;	
//	ifs1>>s;
//	for(int i=0;i<s.length();i+=2){
//		if(se.find(s.substr(i,2))==se.end()){
//			cout<<s.substr(i,2)<<" ";
//		}
//	}
//} 
void init(){
	// 初始化字典 
	ifstream ifs("一二级汉字表.txt");
	string s;ifs>>s;
	for(int i=0;i<s.length();i+=2)
		word.insert(s.substr(i,2));
	ifs.close();
}
void addP1(string s){P1[s]++;}
void addP2(string s){P2[s]++;}
//void addP3(string s){P3[s]++;}
bool isword(string s){
	if(s.length()%2)return 0;
	for(int i=0;i<s.length();i+=2)
		if(word.find(s.substr(i,2))==word.end())return 0;
	return 1;
}
//void train(string s){
//	int n=s.length();
//	for(int i=0;i<n;i++){
//		if(i+2<=n&&isword(s.substr(i,2)))addP1(s.substr(i,2));
//		if(i+4<=n&&isword(s.substr(i,4)))addP2(s.substr(i,4));
//		if(i+6<=n&&isword(s.substr(i,6)))addP3(s.substr(i,6));
//	}
//}
/*
求出了P1,P2,P3
输出 
	字 log(每个字的出现概率)
	字1字2 log(P(字2|字1))
		其中log(P(字2|字1))=log(P(字1字2)/P(字1))=log(P(字1字2))-log(P(字1))
	字1字2字3 log(P(字3|字1字2))
		其中log(P(字3|字1字2))=log(P(字1字2字3)/P(字1字2))=log(P(字1字2字3))-log(P(字1字2))
*/
void process(){
	ll sum=0;
	for(unordered_map<string,ll>::iterator i=P1.begin();i!=P1.end();i++)
		sum+=i->second;
	for(unordered_map<string,ll>::iterator i=P1.begin();i!=P1.end();i++)
		PP1[i->first]=log(i->second*1.0/sum);
	sum=0;
	for(unordered_map<string,ll>::iterator i=P2.begin();i!=P2.end();i++)
		sum+=i->second;
	for(unordered_map<string,ll>::iterator i=P2.begin();i!=P2.end();i++)
		PP2[i->first]=log(i->second*1.0/sum);
	{
		ofstream ofs("dataci1.txt");
		for(unordered_map<string,double>::iterator i=PP1.begin();i!=PP1.end();i++)
			ofs<<i->first<<" "<<i->second<<endl;
	}
	{
		ofstream ofs("dataci2.txt");
		for(unordered_map<string,double>::iterator i=PP2.begin();i!=PP2.end();i++)
			ofs<<i->first<<" "<<i->second-PP1[i->first.substr(0,2)]<<endl;
	}
}
void readData(){
	// 读训练数据 
	for(int t=0;t<fnames.size();t++){
		ifstream ifs("fenci"+fnames[t]);
//		ifstream ifs(fnames[t]);
		string s;
		vector<string>v;
		while(ifs>>s){
			if(isword(s)){
				addP1(s);
				v.push_back(s);
			}
		}
		for(int i=1;i<v.size();i++)
			addP2(v[i-1]+v[i]);
	}
	process();// 处理训练数据 
}
int main(){
//	test();
	init();
	readData();
	
	return 0;
}
