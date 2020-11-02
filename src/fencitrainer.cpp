#include <bits/stdc++.h>
//#include <windows.h>
using namespace std;
#define ll long long
/*
�����ļ���
	fenci2,4,5,6,7,8,9,10,11.txt
���ݴ���
	1.���ÿ���ֵ�log(����)
	2.���ÿ 2������ϵ�log(����)
	3.���ÿ 3������ϵ�log(����)
	
�����裺
	���뺺�ֱ�
	����ÿ�������ļ���
		ͳ�Ƶ��ִ���
		ͳ��2�ִ���
		ͳ��3�ִ���
		(�ִ��ݲ�ͳ�ƣ�Ҫ�÷ִʿⵥ����������)
����� 
	log(ÿ���ֵĳ��ָ���)
	log(P(��2|��1))
	log(P(��3|��1��2))
		P(��2|��1)=P(��1��2)/P(��1)
		P(��3|��1��2)=P(��1��2��3)/P(��1��2)
*/
set<string>word;
unordered_map<string,ll>P1;// ���ʴ���
unordered_map<string,ll>P2;// ��1��2���� 
unordered_map<string,double>PP1;// log����Ƶ��
unordered_map<string,double>PP2;// log��1��2Ƶ��
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
//	ifstream ifs1("һ�������ֱ�.txt");
////	ifstream ifs2("ƴ�����ֱ�.txt");
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
	// ��ʼ���ֵ� 
	ifstream ifs("һ�������ֱ�.txt");
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
�����P1,P2,P3
��� 
	�� log(ÿ���ֵĳ��ָ���)
	��1��2 log(P(��2|��1))
		����log(P(��2|��1))=log(P(��1��2)/P(��1))=log(P(��1��2))-log(P(��1))
	��1��2��3 log(P(��3|��1��2))
		����log(P(��3|��1��2))=log(P(��1��2��3)/P(��1��2))=log(P(��1��2��3))-log(P(��1��2))
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
	// ��ѵ������ 
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
	process();// ����ѵ������ 
}
int main(){
//	test();
	init();
	readData();
	
	return 0;
}
