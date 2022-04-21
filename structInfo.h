#pragma once
#define COL_RANGE 15
#define ROW_RANGE 15
#define COMPUTER 1
#define HUMAN 0

const char EMPTY = 'X';

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


struct ZobristInfo
{
	int depth;
	int val;
};
