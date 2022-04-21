#include"Automaton.h"

#if AC_TYPE==1
Automaton::Automaton()
{
	memset(this->in, 0, sizeof(in));
	memset(this->trie, 0, sizeof(trie));
	memset(this->vis, 0, sizeof(vis));
	memset(this->Map, 0, sizeof(Map));
	memset(ans, 0, sizeof(ans));


	cnt =1;
}


 void Automaton::insert(const string& s,int num)
{
	 int u = 1, len = s.size();
	 for (int i = 0; i < len; i++) {
		 int v =(s[i]=='X'?2:s[i]-'0');
		 if (!trie[u].son[v])
			 trie[u].son[v] = ++cnt;
		 u = trie[u].son[v];
	 }
	 if (!trie[u].flag)
		 trie[u].flag = num;
	 Map[num] = trie[u].flag;
}

 void Automaton::query(const string& s)
 {
	 int u = 1, len = s.size();
	 for (int i = 0; i < len; i++) {
		 
		 u = trie[u].son[s[i] == 'X' ? 2 : s[i] - '0'];
		 //trie[u].ans++;
		 ans[u]++;
	 }
 }
 void Automaton::toplogic()
 {
	 for (int i = 1; i <= cnt; i++)
		 if (sub_in[i] == 0)q.push(i);
	 while (!q.empty()) {
		 int u = q.front(); q.pop(); 
		 //vis[trie[u].flag] = trie[u].ans;
		 vis[trie[u].flag] = ans[u];
		 int v = trie[u].fail; 
		 sub_in[v]--;
		 ans[v] += ans[u];
		 //trie[v].ans += trie[u].ans;
		 if (sub_in[v] == 0)
			 q.push(v);
	 }
 }
 void Automaton::build()
{
	 for (int i = 0; i < 3; i++)
		 trie[0].son[i] = 1;
	 q.push(1);
	 while (!q.empty()) {
		 int u = q.front(); q.pop();
		 int Fail = trie[u].fail;
		 for (int i = 0; i < 3; i++) {
			 int v = trie[u].son[i];
			 if (!v) {
				 trie[u].son[i] = trie[Fail].son[i]; 
				 continue; 
			 }
			 trie[v].fail = trie[Fail].son[i]; 
			 in[trie[v].fail]++;
			 sub_in[trie[v].fail]++;
			 q.push(v);
		 }
	 }
}

 void Automaton::clear()
 {
	 memcpy(sub_in, in, sizeof(in));
	 memset(vis, 0, sizeof(vis));
	 memset(ans, 0, sizeof(ans));
 }

 vector<int> Automaton::search(const string& str)
 {
	 query(str);
	 toplogic();
	 vector<int>res(strnum);
	 for (int i = 0; i < strnum; i++)
		 res[i]=vis[Map[i]];
	 clear();
	 return res;
 }

#endif

#if AC_TYPE==2
 
  void Automaton::insert(const string& s, int id) {
     int now = 0;
     for (int i = 0; i < s.size(); i++) {
         int v = s[i] == 'X' ? 2 : s[i] - '0';
         if (!ch[now][v]) ch[now][v] = ++tot;
         now = ch[now][v];
     }
     val[id] = now;
 }
  void Automaton::build() {
     for (Rint i = 0; i < 3; i++)
         if (ch[0][i]) q[rear++] = ch[0][i];
     while (front < rear) {
         int now = q[front++];
         for (Rint i = 0; i < 3; i++)
             if (ch[now][i]) {
                 fail[ch[now][i]] = ch[fail[now]][i];
                 q[rear++] = ch[now][i];
             }
             else
                 ch[now][i] = ch[fail[now]][i];
     }
 }
  void Automaton::query(const string& s) {
     int now = 0;
     for (int i = 0; i < s.size(); i++) {
         int index = s[i] == 'X' ? 2 : s[i] - '0';
         now = ch[now][index];
         ++sum[now];
     }
 }
 void Automaton::add(int a, int b) {
     to[++cnt] = b; nxt[cnt] = head[a]; head[a] = cnt;
 }
  void Automaton:: dfs(int x) {
     for (Rint i = head[x]; i; i = nxt[i]) {
         dfs(to[i]);
         sum[x] += sum[to[i]];
     }
 }
 Automaton::Automaton() {
     memset(ch, 0, sizeof(ch));
     memset(fail, 0, sizeof(fail));
     memset(val, 0, sizeof(val));
     memset(sum, 0, sizeof(sum));
     memset(q, 0, sizeof(q));
     memset(head, 0, sizeof(head));
     memset(to, 0, sizeof(to));
     memset(nxt, 0, sizeof(nxt));
     front = rear = tot = cnt = 0;
 }
 vector<int>Automaton::search(const string& s) {
     query(s);
     for (Rint i = 1; i <= tot; i++) add(fail[i], i);
     dfs(0);
     vector<int>res(n);
     for (int i = 0; i < n; i++)
         res[i] = sum[val[i]];
     memset(sum, 0, sizeof(sum));
     memset(head, 0, sizeof(head));
     memset(to, 0, sizeof(to));
     memset(nxt, 0, sizeof(nxt));
     cnt = 0;
     return res;
 }
#endif