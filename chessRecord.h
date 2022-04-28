#pragma once
#include"structInfo.h"
#include"Automaton.h"
#include<string>
#include<vector>
#include<stack>
using namespace std;

const enum RECORD_DIR{herizon,vertical, leftBt_rightTop,leftTop_rightBt};

class chessRecord
{
private:
	vector<vector<string>> leftBt_rightTop{ 2,vector<string>(29) };
	vector<vector<string>> leftTop_rightBt{ 2,vector<string>(29) };
	vector<vector<string>> herizon{ 2,vector<string>(15) };
	vector<vector<string>> vertical{2,vector<string>(15)};

	vector<long long>scoreList;


	stack<long long,vector<long long>> herizon_score[2][15];

	stack<long long,vector<long long>> vertical_score[2][15];
	stack<long long,vector<long long>> leftBt_rightTop_score[2][29];
	stack<long long, vector<long long>> leftTop_rightBt_score[2][29];
public:
	long long countVal(const vector<int>& res);
	void init(vector<long long>s);
	long long getScore(bool player);
	void makeChange(int row, int col, bool player,Automaton& AC);
	void clear();
	void unmakeChange(int row, int col, bool player);
	const vector<vector<string>>& getRecord(int direction);
	
};