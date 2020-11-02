#include <bits/stdc++.h>
//#include <windows.h>
using namespace std;
unordered_map<string,double>P1;// 1元模型 正log概率 
unordered_map<string,double>P2;// 2元模型 正log概率 
unordered_map<string,double>P3;// 3元模型 正log概率 
unordered_map<string,double>PT1;// 1元词模型 正log概率 
unordered_map<string,double>PT2;// 2元词模型 正log概率 
bool enable2Word=true;// 2元模型开关
bool enable3Word=false;// 3元模型开关
bool enable2Term=false;// 2元词模型开关
//bool enable2Term=false;// 2元词模型开关
unordered_map<string,vector<string>>py2zh;// 拼音，汉字表
unordered_map<string,vector<string>>zh2py;// 汉字，拼音表(可能有多音字)
unordered_map<string,vector<string>>zhend;// 以该音结尾的，所有词

double getP1(string s){
	if(P1.find(s)==P1.end())return 1e9+7;
	return P1[s];
}
double getP2(string s){
	if(P2.find(s)==P2.end())return 1e9+7;
	return P2[s];
}
double getP3(string s){
	if(P3.find(s)==P3.end())return 1e9+7;
	return P3[s];
}
double getPT1(string s){
	if(PT1.find(s)==PT1.end())return 1e9+7;
	return PT1[s];
}
double getPT2(string s){
	if(PT2.find(s)==PT2.end())return 1e9+7;
	return PT2[s];
}
void init(){
	ifstream py("拼音汉字表.txt");
	string s;double d;
	while(getline(py,s)){
		stringstream ss;ss<<s;
		string py;ss>>py;
		vector<string>zh;
		while(ss>>s)zh.push_back(s);
		py2zh[py]=zh;
		for(int i=0;i<zh.size();i++)
			zh2py[zh[i]].push_back(py);
	}
	if(enable2Word){
		ifstream ifs1("data1.txt");
	//	ifstream ifs2("data2.txt");
		FILE *ifs2=fopen("data2.txt","r");
		while(ifs1>>s>>d){
			P1[s]=-d;
			if(enable2Term){
				PT1[s]=-d;
				for(int i=0;i<zh2py[s.substr(0,2)].size();i++)
					zhend[zh2py[s.substr(0,2)][i]].push_back(s);
			}
		}
	//	while(ifs2>>s>>d){
		char sc[1024];
		int tot=5896921;
		int now=0;
		cout<<"正在读入二元字模型数据......";
		while(fscanf(ifs2,"%s%lf",sc,&d)>0){
			now++;
			if(now%(tot/100)==0){
				cout.width(3);
				cout<<now/(tot/100)<<"%";
				cout<<"\b\b\b\b";
			}
			s="";s+=sc;
			P2[s]=-d;
			if(enable2Term){
				PT2[s]=-d;
			}
	//		zhend[s.substr(s.size()-2,2)].push_back(s);
		}
		fclose(ifs2);
		cout<<"\n";
	}
	if(enable3Word){
		FILE *ifs3=fopen("data3.txt","r");
	//	while(ifs2>>s>>d){
		char sc[1024];
		int tot=51042220;
		int now=0;
		cout<<"正在读入三元字模型数据......";
		while(fscanf(ifs3,"%s%lf",sc,&d)>0){
			now++;
			if(now%(tot/100)==0){
				cout.width(3);
				cout<<now/(tot/100)<<"%";
				cout<<"\b\b\b\b";
			}
			s="";s+=sc;
			P3[s]=-d;
	//		zhend[s.substr(s.size()-2,2)].push_back(s);
		}
		fclose(ifs3);
		cout<<"\n";
	}
	if(enable2Term){
		FILE *ifsc1=fopen("dataci1.txt","r");
		cout<<"正在读入单词模型数据......";
		int now=0;
		int tot=673515;
		char sc[1024];
		while(fscanf(ifsc1,"%s%lf",sc,&d)>0){
			now++;
			if(now%(tot/100)==0){
				cout.width(3);
				cout<<now/(tot/100)<<"%";
				cout<<"\b\b\b\b";
			}
			s="";s+=sc;
			PT1[s]=-d;
			for(int i=0;i<zh2py[s.substr(s.size()-2,2)].size();i++)
				zhend[zh2py[s.substr(s.size()-2,2)][i]].push_back(s);
		}
		fclose(ifsc1);
		cout<<"\n";
		cout<<"正在读入二元词模型数据......";
		FILE *ifsc2=fopen("dataci2.txt","r");
		now=0;
		tot=33326635;
		while(fscanf(ifsc2,"%s%lf",sc,&d)>0){
			now++;
			if(now%(tot/100)==0){
				cout.width(3);
				cout<<now/(tot/100)<<"%";
				cout<<"\b\b\b\b";
			}
			s="";s+=sc;
			PT2[s]=-d;
		}
		fclose(ifsc2);
		cout<<"\n";
	}
}
bool pyvalid(vector<string>&py,int endpos,string& s){
	int len=s.length()/2;
	if(len>endpos+1)return false;
	int pos=0;
	for(int i=endpos+1-len;i<=endpos;i++,pos+=2){
		bool wrong=1;
		for(int j=0;j<zh2py[s.substr(pos,2)].size();j++)
			if(zh2py[s.substr(pos,2)][j]==py[i])wrong=0;
		if(wrong)return false;
	}
	return true;
}
vector<string> transTerm(vector<string>py){
	/*
	dp[i][j]表示前i个字，最后一个词的最后一个字是j的最小log和
	word[i] = 以i字音结尾的，符合拼音的单词集合
	for i
		for j to word[i].size()
			dp[i][j]=min(dp[i][j],dp[i-word[i][j].len]+P[word[i][j]])
	*/
	vector<vector<double>>dp;
	vector<vector<int>>front;// 最小值链祖先，记录i,j位置最优解时的单词和指向的上一个位置 
	double maxv=1e9+7;
	maxv*=maxv;
	for(int i=0;i<py.size();i++){
		dp.push_back(vector<double>(zhend[py[i]].size(),maxv));
		front.push_back(vector<int>(zhend[py[i]].size(),-1));
	}
	for(int i=0;i<py.size();i++){
		for(int j=0;j<zhend[py[i]].size();j++){
			string str=zhend[py[i]][j];
			int len=str.length()/2;
			if(!pyvalid(py,i,str))continue;
			if(len>i+1)continue;
//			if(i==6&&str=="计算机")cout<<"j:"<<str<<endl;
			if(len==i+1){
				double v=getPT1(str);
//				if(str=="两会"){
//					printf("here\n");
//					cout<<str<<endl;
//					cout<<v<<endl;
//					cout<<dp[i][j]<<endl;
//					cout<<i<<" "<<j<<endl;
//				}
				if(dp[i][j]>v){
					dp[i][j]=v;
					front[i][j]=-1;
				}
				continue;
			}
			int ii=i-len;
			for(int k=0;k<zhend[py[ii]].size();k++){
				double v;
				string str2=zhend[py[ii]][k];
				if(!pyvalid(py,ii,str2))continue;
//				if(i==6&&str=="计算机"){
//					cout<<"jk:"<<str<<" "<<str2<<" "<<ii<<" "<<j<<endl;
//					if(str2=="清华大学"){
//						cout<<getPT2(str2+str)<<endl;
//						cout<<dp[ii][k]<<endl;
//					}
//				}
//				if(i==7&&str=="系"){
//					cout<<"jk:"<<str<<" "<<str2<<" "<<ii<<" "<<k<<endl;
//					if(str2=="计算机"){
//						cout<<getPT2(str2+str)<<endl;
//						cout<<dp[ii][k]<<endl;
//					}
//				}
				int len2=str2.length()/2;
				if(len2>ii+1)continue;
				v=dp[ii][k]+getPT2(str2+str);
//				if(i==6&&k==7187){
//					cout<<"debug:"<<v<<endl;
//					cout<<str2<<str<<endl;
//				}
//				if(str=="会"&&(str2=="量"||str2=="两")){
//					printf("here\n");
//					cout<<str2<<" "<<str<<endl;
//					cout<<v<<endl;
//					cout<<dp[i][j]<<endl;
//					cout<<i<<" "<<j<<endl;
//				}
				if(dp[i][j]>v){
					dp[i][j]=v;
					front[i][j]=k;
//					if(front[i][j]==)
				}
			}
		}
	}
	int resj=-1;
	double res=maxv;
	for(int j=0;j<zhend[py[py.size()-1]].size();j++){
		if(res>dp[py.size()-1][j]){
			res=dp[py.size()-1][j];
			resj=j;
		}
	}
//	cout<<res<<endl;
//	cout<<resj<<endl;
//	cout<<dp[6][7187]<<endl;
	vector<string>zh;
	for(int i=py.size()-1;i>=0&&resj!=-1;){
//		cout<<zhend[py[i]][resj]<<" "<<dp[i][resj]<<endl;
		zh.push_back(zhend[py[i]][resj]);
		int step=zhend[py[i]][resj].length()/2;
		resj=front[i][resj];
		i-=step;
	}
	reverse(zh.begin(),zh.end());
	return zh;
}
vector<string> trans(vector<string>py){
//	if(py.size()<1)return null;
	// use viterbi
	/*
	先把每个音的可能字弄出来
	对于每个位置就有很多字
	对这些字做dp
	dp[i][j]表示前i个字，最后一个字选中j的最小log和 
	dp[i][j]=min(dp[i-1][x]+P[w[i-1][x]+w[i][j])
	*/
	double maxv=1e9+7;
	vector<vector<string>>w;
	vector<vector<double>>dp;
	vector<vector<int>>front;// 最小值链祖先
	for(int i=0;i<py.size();i++){
		w.push_back(py2zh[py[i]]);
		dp.push_back(vector<double>(py2zh[py[i]].size(),maxv));
		front.push_back(vector<int>(py2zh[py[i]].size(),-1));
	}
	for(int j=0;j<w[0].size();j++){
		dp[0][j]=getP1(w[0][j]);
	}
	for(int i=1;i<py.size();i++){
		for(int j=0;j<w[i].size();j++){
			for(int k=0;k<w[i-1].size();k++){
				double v=dp[i-1][k]+getP2(w[i-1][k]+w[i][j]);
				if(dp[i][j]>v){
					dp[i][j]=v;
					front[i][j]=k;
				}
				if(enable3Word){
					if(i-2<0)continue;
					for(int kk=0;kk<w[i-2].size();kk++){
						double v=dp[i-2][kk]+getP2(w[i-2][kk]+w[i-1][k])+getP3(w[i-2][kk]+w[i-1][k]+w[i][j]);
						if(dp[i][j]>v){
							dp[i][j]=v;
							front[i][j]=k;
							front[i-1][k]=kk;
						}
					}
				}
			}
		}
	}
	int resj=-1;
	double res=maxv;
	for(int j=0;j<w[py.size()-1].size();j++){
		if(res>dp[py.size()-1][j]){
			res=dp[py.size()-1][j];
			resj=j;
		}
	}
//	cout<<res<<endl;
	vector<string>zh;
	for(int i=py.size()-1;i>=0;i--){
		zh.push_back(w[i][resj]);
//		cout<<w[i][resj]<<" "<<dp[i][resj]<<endl;
		resj=front[i][resj];
	}
	reverse(zh.begin(),zh.end());
	return zh;
}
void process(string infile,string outfile){
	ifstream ifs(infile);
	ofstream ofs(outfile);
	/*
	按行输入，每个拼音按空格分割 
	*/
	string s;
	while(getline(ifs,s)){
		stringstream ss;
		ss<<s;
		vector<string>py;
		while(ss>>s)py.push_back(s);
		for(int i=0;i<py.size();i++){
			transform(py[i].begin(),py[i].end(),py[i].begin(),::tolower);
		}
		vector<string>res;
		if(enable2Term)
			res=transTerm(py);
		else
			res=trans(py);
		for(int i=0;i<res.size();i++)ofs<<res[i];
		ofs<<endl;
	}
}
void check(string s){
	vector<double>vv;
	double v=getP1(s.substr(0,2));
	cout<<s<<":";
	cout<<v<<" ";
	for(int i=2;i<s.length();i+=2){
		double tv=getP2(s.substr(i-2,4));
		v+=tv;
		cout<<s.substr(i-2,4)<<" "<<tv<<" ";
	}
	cout<<v<<endl;
}
int main(int argc,char **argv){
//	check("清华大学计算机系");
//	check("我上学去了");
//	check("今天回家比较晚");
//	check("今天回家比较完");
//	check("两会在北京召开");
//	init();
//	process("input_test.txt","output_test1.txt");
	if(argc==3){
		time_t t1=time(NULL);
		init();
		time_t t2=time(NULL);
		cout<<"初始化时间："<<t2-t1<<"秒"<<endl;
		process(argv[1],argv[2]);
		time_t t3=time(NULL);
		cout<<"拼音转换时间："<<t3-t2<<"秒"<<endl;
	}else{
		printf("调用格式：app.exe input.txt output.txt\n");
	}
	return 0;
}
