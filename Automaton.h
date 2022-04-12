#pragma once
#include<string>
#include<queue>
using namespace std;

const int AC_STATES = 1e5;
// X����3��ʣ��0,1
class Automaton
{

public:
	Automaton();
	void insert(const string& str,int val);
	int search(const string& str);
	void build();
private:
	int value[AC_STATES];
	int tr[AC_STATES][3];	//״̬���
	int cnt;				//����ؼ�
	int fail[AC_STATES];	//ʧ��ָ��
	int e[AC_STATES];		//��ǰ׺�ĸ���
	queue<int>q;
};

