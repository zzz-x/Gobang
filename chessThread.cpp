#include "chessThread.h"

void chessThread::init(int depth)
{
	ai.setDepth(depth);
	ai.ZobristInit();
}

void chessThread::setChess(int row, int col,int player)
{
	if (player == COMPUTER)
		ai.aiChess[row][col] = 1;
	else
		ai.manChess[row][col] = 1;
}

bool chessThread::check(int row, int col, bool player)
{	
	if (player == COMPUTER)
		return ai.aiChess[row][col];
	else
		return ai.manChess[row][col];

}

void chessThread::run()
{
	qDebug() << ai.SearchDepth << endl;
	res = ai.search(COMPUTER);
	ai.aiChess[res.nextPosRow][res.nextPosCol] = 1;
	qDebug() << "ok!" << res.nextPosCol << ' ' << res.nextPosRow << endl;
	emit isDone();
}
