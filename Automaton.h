#pragma once
#include<string>
using namespace std;
#define SEARCH_WITH_AC 1
#define AC_TYPE 2
#define SINGLE_EVALUATE_USE_AC 1


#if AC_TYPE==1
#include<queue>
const int AC_STATES = 50;

struct AcNode {
	int son[3];
	int fail;
	int flag;
	AcNode() :fail(0),flag(0){};
};
// X代表3，剩余0,1
class Automaton
{
public:
	Automaton();
	void insert(const string& str,int num);
	void build();
	void setStrNum(int n) { strnum = n; }
	vector<int>search(const string& str);
private:
	void toplogic();

	void clear();
	void query(const string& str);
	int vis[AC_STATES];	//出现次数
	int Map[AC_STATES];
	AcNode trie[AC_STATES];
	int strnum;
	int ans[AC_STATES];
	int in[AC_STATES];
	int sub_in[AC_STATES];
	int cnt;
	queue<int>q;
};
#endif

#if  AC_TYPE==2

#define Rint register int
#include<vector>
using namespace std;

const int N = 50;

class Automaton {
private:
    int n, ch[N][3], fail[N], val[N], sum[N], tot;
    int q[N], front, rear;
    int head[N], to[N], nxt[N];
    int cnt;    //static in add
public:
    void insert(const string& s, int id);
    void build();
    void setStrNum(int strnum) { n = strnum; }
    void query(const string& s);
    void add(int a, int b);
    void dfs(int x);
    Automaton();
	vector<int>search(const string& s);
};
#endif

