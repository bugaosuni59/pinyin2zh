#include <bits/stdc++.h>
using namespace std;
void solve(string f1,string f2){
	ifstream ifs1(f1);
	ifstream ifs2(f2);
	string s1,s2;
	int tot=0;
	int sum=0;
	while(getline(ifs1,s1)){
		getline(ifs2,s2);
		if(s1.size()!=s2.size()){
			printf("这字数不太一样吧\n");
			exit(0);
		}
		for(int i=1;i<s1.size();i+=2){
			if(s1[i-1]==s2[i-1]&&s1[i]==s2[i])sum++;
			tot++;
		}
	}
	printf("正确率：%f(%d/%d)\n",sum*1.0/tot,sum,tot);
}
int main(int argc,char** argv){
	if(argc!=3){
		printf("用法：checker.exe file1.txt file2.txt\n");
		return 0;
	}
	solve(argv[1],argv[2]);
	return 0;
}
