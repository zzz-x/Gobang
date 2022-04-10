#include "AI.h"
#include<iostream>
#include<map>
#include<queue>
using namespace std;





/*
全部以MAX层的角度来考虑
alpha 是当前MAX层的下限
beta min 上一个MIN层的上限
*/

int AI::MinMaxSearch(bool player,int depth, int alpha, int beta)
{
	if (gameOver(player)||gameOver(!player)||depth==0)
		return evaluate(player);

	/*   
	  加入启发式搜索
	  对当前的所有的空点进行打分，排序
	  利用优先队列
	*/

	auto info = ZobristFind(this->ZobristCode);
	if (info.depth >= depth) {
		printf("通过Zobrist缓存 \n");
		return info.val;
	}

	priority_queue<PointVal> q;
	for (int i = 0; i < ROW_RANGE; i++) {
		for (int j = 0; j < COL_RANGE; j++) {
			if (this->aiChess[i][j] || this->manChess[i][j] || !hasNeighbor(i, j))
				continue;
			if (player == COMPUTER)
				aiChess[i][j] = 1;
			else
				manChess[i][j] = 1;

			int val = this->SingleScore(i, j, player);
			q.push({ i,j,val });
			if (player == COMPUTER)
				aiChess[i][j] = 0;
			else
				manChess[i][j] = 0;
		}
	}
	while (!q.empty()) {
		PointVal tmp = q.top();
		q.pop();
		if (player == COMPUTER)
			aiChess[tmp.rowIndex][tmp.colIndex] = 1;
		else
			manChess[tmp.rowIndex][tmp.colIndex] = 1;

		//Zobrist 计算
		ZobristMove(player, tmp.rowIndex, tmp.colIndex);

		int scoreList = -MinMaxSearch(!player, depth - 1, -beta, -alpha);

			//Zobrist 取消
		ZobristMove(player, tmp.rowIndex, tmp.colIndex);

		if (player == COMPUTER)
			aiChess[tmp.rowIndex][tmp.colIndex] = 0;
		else
			manChess[tmp.rowIndex][tmp.colIndex] = 0;

		if (scoreList > alpha) {
			alpha = scoreList;
			if (depth == this->SearchDepth)
				nextPos = { tmp.rowIndex,tmp.colIndex };
			validNodes++;
		}
		if (scoreList >= beta) {
			cutNodes++;
			return beta;
		}
	}
	ZobristCache(depth, alpha);
	return alpha;
}

int AI::killSearch(bool player, int depth, int alpha, int beta)
{
	if (gameOver(player) || gameOver(!player) || depth == 0)
		return evaluate(player);
	/*
	  加入启发式搜索
	  对当前的所有的空点进行打分，排序
	  利用优先队列
	*/
	priority_queue<PointVal> q;
	for (int i = 0; i < ROW_RANGE; i++) {
		for (int j = 0; j < COL_RANGE; j++) {
			if (this->aiChess[i][j] || this->manChess[i][j] || !hasNeighbor(i, j))
				continue;
			if (player == COMPUTER)
				aiChess[i][j] = 1;
			else
				manChess[i][j] = 1;

			if (!isKillChess(player, i, j)) {
				if (player == COMPUTER)
					aiChess[i][j] = 0;
				else
					manChess[i][j] = 0;
				continue;
			}
			
			int val = this->SingleScore(i, j, player);
			if(val>100)	//剪去一些低分
				q.push({ i,j,val });
			if (player == COMPUTER)
				aiChess[i][j] = 0;
			else
				manChess[i][j] = 0;
		}
	}
	while (!q.empty()) {
		PointVal tmp = q.top();
		q.pop();
		if (player == COMPUTER)
			aiChess[tmp.rowIndex][tmp.colIndex] = 1;
		else
			manChess[tmp.rowIndex][tmp.colIndex] = 1;

		int scoreList = -killSearch(!player, depth - 1, -beta, -alpha);

		if (player == COMPUTER)
			aiChess[tmp.rowIndex][tmp.colIndex] = 0;
		else
			manChess[tmp.rowIndex][tmp.colIndex] = 0;

		if (scoreList > alpha) {
			alpha = scoreList;
			if (depth == this->SearchDepth)
				nextPos = { tmp.rowIndex,tmp.colIndex };
		}
		if (scoreList >= beta)
			return beta;
	}
	return alpha;
}

bool AI::isKillChess(bool player, int rowIndex, int colIndex)
{
	const vector < Direction > direct = { {0,1},{1,0}, {1,1},{1,-1} };

	for (int k = 0; k < 4; k++) {
		string str;
		for (int i = -4; i <= 4; i++) {
			int tmp_rol = rowIndex + i * direct[k].dy;
			int tmp_col = colIndex + i * direct[k].dx;
			if (isInBoard(tmp_rol, tmp_col)) {
				if (player == COMPUTER) {
					if (!manChess[tmp_rol][tmp_col])
						str += (aiChess[tmp_rol][tmp_col] == 1 ? "1" : "0");
					else
						str += "X";
				}
				else {
					if (!aiChess[tmp_rol][tmp_col])
						str+= (manChess[tmp_rol][tmp_col] == 1 ? "1" : "0");
					else
						str+= "X";
				}
			}
		}
		for (int i = 1; i <= 9; i++)
			if (str.find(chessStr[i]) != str.npos)
				return 1;
	}
	return 0;
}

bool AI::gameOver(bool player)
{
	const vector<Direction>direct = { {0,1},{1,0},{1,1},{1,-1} };
	for(int i=0;i<ROW_RANGE;i++)
		for (int j = 0; j < COL_RANGE; j++)
			for (int m = 0; m < direct.size(); m++) {
				if (!isInBoard(i + 4 * direct[m].dy, j + 4 * direct[m].dx))
					continue;
				int n;
				for (n = 0; n < 5; n++) {
					if (player == COMPUTER && ((!isInBoard(i + n * direct[m].dy, j + n * direct[m].dx)) || aiChess[i + n * direct[m].dy][j + n * direct[m].dx] == 0))
						break;
					if (player == HUMAN && ((!isInBoard(i + n * direct[m].dy, j + n * direct[m].dx)) || manChess[i + n * direct[m].dy][j + n * direct[m].dx] == 0))
						break;
				}
				if (n == 5)
					return 1;
			}
	return false;
}

int AI::evaluate(bool player)
{
	int selfScore = 0, enemyScore = 0;
	for (int i = 0; i < ROW_RANGE; i++) {
		for (int j = 0; j < COL_RANGE; j++) {
			if (player == COMPUTER) {
				if(aiChess[i][j])
					selfScore += SingleScore(i, j, player);
				if(manChess[i][j])
					enemyScore += SingleScore(i, j, !player);
			}
			else {
				if (manChess[i][j])
					selfScore += SingleScore(i, j, player);
				if (aiChess[i][j])
					enemyScore += SingleScore(i, j, !player);
			}
		}
	}
	return selfScore - enemyScore;
}

int AI::SingleScore(int rowIndex, int colIndex, bool player)
{
	//4个方向，通过+-即可获得8个方向
	const vector < Direction > direct = { {0,1},{1,0}, {1,1},{1,-1} };
	vector <string>pointList;
	int score=0;
	int dirScore = 0;
	for (int i = 0; i < 4; i++)
		pointList.push_back(getPointLink(rowIndex, colIndex, player, direct[i]));
	for (int k= 0; k < 4; k++) {
		for (int i = 0; i < chessStr.size(); i++) {
			if (pointList[k].find(chessStr[i]) != pointList[k].npos) 
				dirScore = max(dirScore, scoreList[i]);
		}
		score += dirScore;
	}
	return score;
}

bool AI::isInBoard(int rowIndex, int colIndex)
{
	return rowIndex >= 0 && rowIndex < ROW_RANGE&& colIndex >= 0 && colIndex < COL_RANGE;
}

bool AI::hasNeighbor(int rowIndex, int colIndex)
{
	for(int i=-1;i<=1;i++)
		for (int j = -1; j <=1; j++){
			if (i == 0 && j == 0)
				continue;
			if (isInBoard(rowIndex + i, colIndex + j) && (aiChess[rowIndex + i][colIndex + j] || manChess[rowIndex + i][colIndex + j]))
				return true;
		}
	return false;
}

string  AI::getPointLink(int rowIndex, int colIndex, bool player,const Direction& direct)
{
	string pointLink;

	for (int i =-4; i <=4; i++) {
		int tmp_rol = rowIndex + i * direct.dy;
		int tmp_col = colIndex + i * direct.dx;
		if (isInBoard(tmp_rol, tmp_col)) {
			if (player == COMPUTER) {
				if (!manChess[tmp_rol][tmp_col])
					pointLink += (aiChess[tmp_rol][tmp_col] == 1 ? "1" : "0");
				else 
					pointLink += "X";
			}
			else {
				if (!aiChess[tmp_rol][tmp_col])
					pointLink += (manChess[tmp_rol][tmp_col] == 1 ? "1" : "0");
				else
					pointLink += "X";
			}
		}
	}
	
	return pointLink;
}

AI::AI(int depth)
{
	this->SearchDepth =depth;

	cutNodes = 0;
	validNodes = 0;

	chessStr.push_back("11111");	scoreList.push_back(100000);//连五

	chessStr.push_back("011110");	scoreList.push_back(10000);//活四

	chessStr.push_back("01111");	scoreList.push_back(1000);//眠四
	chessStr.push_back("10111");	scoreList.push_back(1000);
	chessStr.push_back("11011");	scoreList.push_back(1000);
	chessStr.push_back("11101");	scoreList.push_back(1000);
	chessStr.push_back("11110");	scoreList.push_back(1000);

	chessStr.push_back("01110");	scoreList.push_back(1000);//活三
	chessStr.push_back("010110");	scoreList.push_back(1000);
	chessStr.push_back("011010");	scoreList.push_back(1000);

	chessStr.push_back("11100");	scoreList.push_back(100);//眠三
	chessStr.push_back("00111");	scoreList.push_back(100);

	chessStr.push_back("001100");	scoreList.push_back(100);//活二
	chessStr.push_back("01010");	scoreList.push_back(100);

	chessStr.push_back("11000");	scoreList.push_back(10);//眠二
	chessStr.push_back("00011");	scoreList.push_back(10);

	chessStr.push_back("011010");	scoreList.push_back(10);//活一
}

SEARCH_INFO AI::search(bool player)
{
	clock_t start, end;
	start = clock();
	cout << "search start" << endl;
	MinMaxSearch(player, SearchDepth, -0x3fffffff,0x3fffffff);
	cout << "search end" << endl;
	end = clock();
	SEARCH_INFO res{ this->nextPos.row,this->nextPos.col,end - start,cutNodes,validNodes };
	cutNodes = validNodes = 0;
	return res;
}


