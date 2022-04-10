#pragma once
#include<qthread.h>
#include<qdebug.h>
#include"AI.h"

class chessThread :public QThread
{
	Q_OBJECT
public:
	void init(int depth = 2);
	void setDepth(int n = 2) { ai.setDepth(n); }
	void setChess(int row, int col,int player=HUMAN);
	bool check(int row,int col,bool player);
	SEARCH_INFO getResult(){ return res; }
	bool gameOver(bool player) { return ai.gameOver(player); }
protected:
	AI ai;
	SEARCH_INFO res;
	void run();
signals:
	void isDone();
};