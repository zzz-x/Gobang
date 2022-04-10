#pragma once
#pragma execution_character_set("utf-8") 
#include<vector>
#include<map>
#include<string>
#include<time.h>

using namespace std;

#define COL_RANGE 15
#define ROW_RANGE 15
#define COMPUTER 1
#define HUMAN 0



struct SEARCH_INFO {
	int nextPosRow;
	int nextPosCol;
	int time;
	int cutNodes;
	int validNodes;
};


/* row col */
struct Point
{
	int row;
	int col;
};
struct PointVal {
	int rowIndex;
	int colIndex;
	int value;
	friend bool operator <(const PointVal& p1, const PointVal& p2) { return p1.value < p2.value; }

};

/* dx dy */
struct Direction
{
	int dx;
	int dy;
};


struct ZobristInfo {
	int depth;
	int val;
};

class AI {
protected:
	int SearchDepth;
	Point nextPos;
	bool manChess[15][15] = {0};	//记录人
	bool aiChess[15][15] = { 0 };	//记录AI

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
	bool gameOver(bool player);
	int evaluate(bool isAI);
	int SingleScore(int rowIndex, int colIndex, bool player);
	bool hasNeighbor(int rowIndex, int colIndex);
	string getPointLink(int rowIndex, int colIndex, bool player, const Direction& direct);
	
public:
	AI(int depth = 2);
	SEARCH_INFO search(bool player);
	void ZobristInit();

	void setDepth(int n) { SearchDepth = n; }
	bool isInBoard(int rowIndex, int colIndex);

	friend class chessThread;

};