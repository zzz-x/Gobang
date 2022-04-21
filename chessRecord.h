#pragma once
#include"structInfo.h"
#include"Automaton.h"
#include<string>
#include<vector>
using namespace std;

const enum RECORD_DIR{herizon,vertical, leftBt_rightTop,leftTop_rightBt};

class chessRecord
{
private:
	vector<vector<string>> leftBt_rightTop{ 2,vector<string>(29) };
	vector<vector<string>> leftTop_rightBt{ 2,vector<string>(29) };
	vector<vector<string>> herizon{ 2,vector<string>(15) };
	vector<vector<string>> vertical{2,vector<string>(15)};

	vector<int>scoreList;


	int herizon_score[2][15][2];
	int vertical_score[2][15][2]; 
	int leftBt_rightTop_score[2][29][2];
	int leftTop_rightBt_score[2][29][2];
public:
	int countVal(const vector<int>& res);
	void init(vector<int>s);
	int getScore(bool player);
	void makeChange(int row, int col, bool player,Automaton& AC);
	void clear();
	void unmakeChange(int row, int col, bool player);
	const vector<vector<string>>& getRecord(int direction);
	
};