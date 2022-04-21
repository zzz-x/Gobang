#pragma once
#pragma execution_character_set("utf-8") 
#include<vector>
#include<map>
#include<string>
#include<qobject.h>
#include"Automaton.h"
#include<time.h>
#include"structInfo.h"
#include"chessRecord.h"

using namespace std;




class AI:public QObject
{
	Q_OBJECT
protected:
	int SearchDepth;
	Point nextPos;
	Automaton AC;	//AC自动机
	char chessBoard[15][15];//记录棋盘

	chessRecord boardRecord;//棋盘四个方向的字符串
	long long Zobrist_Man[15][15];
	long long Zobrist_Ai[15][15];
	long long ZobristCode;

	map<long long,ZobristInfo>ZobristMap;
	vector<string>chessStr;
	vector<int> scoreList;

	int cutNodes;
	int validNodes;

	//Zobrist部分
	void ZobristMove(bool player, int rowIndex, int colIndex);
	void ZobristCache(int depth, int score);
	const ZobristInfo ZobristFind(long long code);
	int MinMaxSearch(bool player, int depth, int alpha, int beta);
	int killSearch(bool player, int depth, int alpha, int beta);
	bool isKillChess(bool player, int rowIndex, int colIndex);
	int evaluate(bool isAI);
	int SingleScore(int rowIndex, int colIndex, bool player);
	bool hasNeighbor(int rowIndex, int colIndex);
	string getPointLink(int rowIndex, int colIndex, bool player, const Direction& direct);
	int countVal(const vector<int>&res);

public:
	AI(int depth = 2);
	SEARCH_INFO searchRes;
	bool gameOver(bool player);
	void ZobristInit();
	void setChess(int row, int col, int player);
	bool checkChess(int row, int col, int player);
	void setDepth(int n) { SearchDepth = n; }
	bool isInBoard(int rowIndex, int colIndex);
	void clear();
	void makeMove(int row, int col, int player);
	void unmakeMove(int row, int col, int player);
	
public slots:
	void search();
signals:
	void isDone();
};