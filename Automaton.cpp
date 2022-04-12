#include"Automaton.h"
Automaton::Automaton()
{
	memset(tr, 0, sizeof(tr));
	memset(fail, 0, sizeof(fail));
	memset(e, 0, sizeof(e));
	memset(value, 0, sizeof(value));
	cnt = 0;
}


 void Automaton::insert(const string& str,int val)
{
	int len = str.size();
	int u = 0;
	for (int i = 0; i < len; i++) {
		int index = (str[i] == 'X' ? 2 : str[i] - '0');
		if (!tr[u][index]) {
			tr[u][index] = ++cnt;
		}
		u = tr[u][index];
	}
	e[u]++;
	value[u] =(value[u]==0?val:value[u]);
}

int Automaton::search(const string& str)
{
	int u = 0, res = 0;
	for (int i = 0; i < str.length(); i++) {
		int index = (str[i] == 'X' ? 2 : str[i] - '0');
		u = tr[u][index];
		for (int j = u; j != 0 && e[j] != -1; j = fail[j]) {
			res += value[j];
			e[j] = -1;
		}
	}
	return res;
}
 void Automaton::build()
{
	for (int i = 0; i < 3; i++)
		if (tr[0][i])
			q.push(tr[0][i]);
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		for (int i = 0; i < 3; i++) {
			if (tr[u][i]) {
				fail[tr[u][i]] = tr[fail[u]][i];
				q.push(tr[u][i]);
			}
			else
				tr[u][i] = tr[fail[u]][i];
		}
	}
}