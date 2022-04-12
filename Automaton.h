#pragma once
#include<string>
#include<queue>
using namespace std;

const int AC_STATES = 1e5;
// X代表3，剩余0,1
class Automaton
{

public:
	Automaton();
	void insert(const string& str,int val);
	int search(const string& str);
	void build();
private:
	int value[AC_STATES];
	int tr[AC_STATES][3];	//状态编号
	int cnt;				//分配控件
	int fail[AC_STATES];	//失配指针
	int e[AC_STATES];		//该前缀的个数
	queue<int>q;
};

