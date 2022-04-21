#pragma once
#include "AI.h"
#include<iostream>
#include<map>
#include<queue>
#include<qdebug.h>
using namespace std;

/*
全部以MAX层的角度来考虑
alpha 是当前MAX层的下限
beta min 上一个MIN层的上限
*/

int AI::MinMaxSearch(bool player,int depth, int alpha, int beta)
{
	if (depth == 0 || gameOver(player) || gameOver(!player)) {
		return evaluate(player);
	}

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
			if (chessBoard[i][j]!=EMPTY || !hasNeighbor(i, j))
				continue;

			char target_ch = (player == COMPUTER ? '1' : '0');
			chessBoard[i][j] = target_ch;
			int val = this->SingleScore(i, j, player)-this->SingleScore(i,j,!player);
			q.push({ i,j,val });		
			chessBoard[i][j] = EMPTY;
		}
	}
	int cnt = 5	;
	while (!q.empty()) {
		PointVal tmp = q.top();
		q.pop();
		cnt--;
		if (SearchDepth == 6 && cnt < 0)
			break;
		makeMove(tmp.rowIndex, tmp.colIndex, player);
		//Zobrist 计算
		ZobristMove(player, tmp.rowIndex, tmp.colIndex);

		int scoreList = -MinMaxSearch(!player, depth - 1, -beta, -alpha);

			//Zobrist 取消
		ZobristMove(player, tmp.rowIndex, tmp.colIndex);

		unmakeMove(tmp.rowIndex, tmp.colIndex, player);

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
	
	char target_ch = (player == COMPUTER ? '1' : '0');
	priority_queue<PointVal> q;
	for (int i = 0; i < ROW_RANGE; i++) {
		for (int j = 0; j < COL_RANGE; j++) {
			if (chessBoard[i][j]!=EMPTY || !hasNeighbor(i, j))
				continue;

			chessBoard[i][j] = target_ch;

			if (!isKillChess(player, i, j)) {
				chessBoard[i][j] =EMPTY;
				continue;
			}
			
			int val = this->SingleScore(i, j, player);
			if(val>=1000)	//剪去一些低分
				q.push({ i,j,val });
			chessBoard[i][j] = EMPTY;
		}
	}
	while (!q.empty()) {
		PointVal tmp = q.top();
		q.pop();
		
		makeMove(tmp.rowIndex, tmp.colIndex, player);

		int scoreList = -killSearch(!player, depth - 1, -beta, -alpha);
		
		unmakeMove(tmp.rowIndex, tmp.colIndex, player);

		if (scoreList > alpha) {
			alpha = scoreList;
			if (depth == 4)
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

	char target_ch = (player == COMPUTER ? '1' : '0');

	for (int k = 0; k < 4; k++) {
		string str;
		for (int i = -3; i <= 3; i++) {
			int tmp_rol = rowIndex + i * direct[k].dy;
			int tmp_col = colIndex + i * direct[k].dx;
			if (isInBoard(tmp_rol, tmp_col)) {
				if (chessBoard[tmp_rol][tmp_col] != EMPTY)
					str += (chessBoard[tmp_rol][tmp_col] == target_ch ? '1' : '0');
				else
					str += EMPTY;	//空
			}
		}
		for (int i = 1; i <= 8; i++)
			if (str.find(chessStr[i]) != str.npos)
				return 1;
	}
	return 0;
}

bool AI::gameOver(bool player)
{
	char target_ch = (player == COMPUTER ? '1' : '0');
	const vector<Direction>direct = { {0,1},{1,0},{1,1},{1,-1} };
	for(int i=0;i<ROW_RANGE;i++)
		for (int j = 0; j < COL_RANGE; j++)
			for (int m = 0; m < direct.size(); m++) {
				if (!isInBoard(i + 4 * direct[m].dy, j + 4 * direct[m].dx))
					continue;
				int n;
				for (n = 0; n < 5; n++) {
					if ((!isInBoard(i + n * direct[m].dy, j + n * direct[m].dx)) || chessBoard[i + n * direct[m].dy][j + n * direct[m].dx]!=target_ch)
						break;
				}
				if (n == 5)
					return 1;
			}
	return false;
}
int AI::countVal(const vector<int>&res) {
	int val=0;
	for (int i = 0; i < chessStr.size(); i++)
		val += res[i] * scoreList[i];
	return val;
}
int AI::evaluate(bool player)
{
#if SEARCH_WITH_AC
	/*auto herizon = boardRecord.getRecord(RECORD_DIR::herizon);
	auto vertical = boardRecord.getRecord(RECORD_DIR::vertical);
	auto leftBt_rightTop = boardRecord.getRecord(RECORD_DIR::leftBt_rightTop);
	auto leftTop_rightBt = boardRecord.getRecord(RECORD_DIR::leftTop_rightBt);

	int selfScore = 0, enemyScore = 0;
	for (auto item : herizon[player]) {
		auto res = AC.search(item);
		selfScore += countVal(res);
	}
	for (auto item : herizon[!player]) {
		auto res = AC.search(item);
		enemyScore += countVal(res);
	}
	for (auto item : vertical[player]) {
		auto res = AC.search(item);
		selfScore += countVal(res);
	}
	for (auto item : vertical[!player]) {
		auto res = AC.search(item);
		enemyScore += countVal(res);
	}
	for (auto item : leftBt_rightTop[player]) {
		auto res = AC.search(item);
		selfScore += countVal(res);
	}
	for (auto item : leftBt_rightTop[!player]) {
		auto res = AC.search(item);
		enemyScore += countVal(res);
	}
	for (auto item : leftTop_rightBt[player]) {
		auto res = AC.search(item);
		selfScore += countVal(res);
	}
	for (auto item : leftTop_rightBt[!player]) {
		auto res = AC.search(item);
		enemyScore += countVal(res);
	}*/
	return boardRecord.getScore(player);
#endif

#if !SEARCH_WITH_AC
	char target_ch = player == COMPUTER ? '1' : '0';
	char enemy_ch = player == COMPUTER ? '0' : '1';
	int selfScore = 0, enemyScore = 0;
	for (int i = 0; i < ROW_RANGE; i++) {
		for (int j = 0; j < COL_RANGE; j++) {
			if (chessBoard[i][j] == target_ch||hasNeighbor(i,j))
				selfScore += SingleScore(i, j, player);
			if (chessBoard[i][j] == enemy_ch||hasNeighbor(i,j))
				enemyScore += SingleScore(i, j, !player);
		}
	}
	return selfScore - enemyScore;
#endif
}

int AI::SingleScore(int rowIndex, int colIndex, bool player)
{
	//4个方向，通过+-即可获得8个方向
	const vector < Direction > direct = { {0,1},{1,0}, {1,1},{1,-1} };
	vector <string>pointList(4);
	int score=0;
	for (int i = 0; i < 4; i++)
		pointList[i]=getPointLink(rowIndex, colIndex, player, direct[i]);
	
#if SINGLE_EVALUATE_USE_AC==0
	for (int k = 0; k < 4; k++) {	
		int dirScore = 0;
		for (int i = 0; i < chessStr.size(); i++) {
			if (pointList[k].find(chessStr[i]) != pointList[k].npos)
				dirScore += scoreList[i];
		}
		score += dirScore;
	}
#endif

#if SINGLE_EVALUATE_USE_AC==1
	for (int k = 0; k < 4; k++) {
		score += countVal(AC.search(pointList[k]));
	}
#endif

	return score;
}

bool AI::isInBoard(int rowIndex, int colIndex)
{
	return rowIndex >= 0 && rowIndex < ROW_RANGE&& colIndex >= 0 && colIndex < COL_RANGE;
}

void AI::clear()
{
	for (int i = 0; i < ROW_RANGE; i++)
		for (int j = 0; j < COL_RANGE; j++)
			chessBoard[i][j] = EMPTY;
	SearchDepth = 2;
}

void AI::makeMove(int row, int col, int player)
{
	char target_ch = (player == COMPUTER ? '1' : '0');
	chessBoard[row][col] = target_ch;
	boardRecord.makeChange(row, col, player,AC);
}

void AI::unmakeMove(int row, int col, int player)
{
	chessBoard[row][col] = EMPTY;
	boardRecord.unmakeChange(row, col, player);
}

bool AI::hasNeighbor(int rowIndex, int colIndex)
{
	for(int i=-1;i<=1;i++)
		for (int j = -1; j <=1; j++){
			if (i == 0 && j == 0)
				continue;
			if (isInBoard(rowIndex + i, colIndex + j) && (chessBoard[rowIndex+i][colIndex+j]!=EMPTY))
				return true;
		}
	return false;
}

string  AI::getPointLink(int rowIndex, int colIndex, bool player,const Direction& direct)
{
	string pointLink;

	char self_ch =(player == COMPUTER ? '1' : '0');
	char enemy_ch = (player == COMPUTER ? '0' : '1');

	for (int i =-3; i <=3; i++) {
		int tmp_rol = rowIndex + i * direct.dy;
		int tmp_col = colIndex + i * direct.dx;
		if (isInBoard(tmp_rol, tmp_col)) {
			if (chessBoard[tmp_rol][tmp_col] == self_ch)
				pointLink += '1';
			else if (chessBoard[tmp_rol][tmp_col] == enemy_ch)
				pointLink += '0';
			else
				pointLink += EMPTY;
		}
	}
	
	return pointLink;
}

AI::AI(int depth)
{

	memset(chessBoard,EMPTY,sizeof(chessBoard));
	this->SearchDepth =depth;

	cutNodes = 0;
	validNodes = 0;

	chessStr.push_back("11111");	scoreList.push_back(1000000);//连五
	
	chessStr.push_back("X1111X");	scoreList.push_back(10000);//活四
	

	chessStr.push_back("X1111");	scoreList.push_back(1000);//眠四
	chessStr.push_back("1X111");	scoreList.push_back(1000);
	chessStr.push_back("11X11");	scoreList.push_back(1000);
	chessStr.push_back("111X1");	scoreList.push_back(1000);
	chessStr.push_back("1111X");	scoreList.push_back(1000);

	chessStr.push_back("X111X");	scoreList.push_back(1000);//活三
	chessStr.push_back("X1X11X");	scoreList.push_back(300);
	chessStr.push_back("X11X1X");	scoreList.push_back(300);

	chessStr.push_back("111XX");	scoreList.push_back(100);//眠三
	chessStr.push_back("XX111");	scoreList.push_back(100);

	chessStr.push_back("XX11XX");	scoreList.push_back(100);//活二
	chessStr.push_back("X1X1X");	scoreList.push_back(100);

	chessStr.push_back("11XXX");	scoreList.push_back(10);//眠二
	chessStr.push_back("XXX11");	scoreList.push_back(10);

	chessStr.push_back("XX1XX");	scoreList.push_back(10);//活一
	
	boardRecord.init(scoreList);

	AC.setStrNum(chessStr.size());


	for (int i=0;i<chessStr.size();i++)
		AC.insert(chessStr[i],i);
	AC.build();
}

void AI::search()
{
	clock_t start, end;
	start = clock();
	cout << "search start" << endl;
	int val=MinMaxSearch(COMPUTER, SearchDepth, -0x3fffffff,0x3fffffff);
	cout << "search end" << endl<<"val= "<<val<<endl;
	cout << boardRecord.getRecord(RECORD_DIR::leftBt_rightTop)[COMPUTER][15] << endl;
	end = clock();
	SEARCH_INFO res{ this->nextPos.row,this->nextPos.col,end - start,cutNodes,validNodes };
	cutNodes = validNodes = 0;
	searchRes = res;
	emit isDone();
}


