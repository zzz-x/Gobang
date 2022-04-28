#include "chessRecord.h"


long long chessRecord::countVal(const vector<int>& res)
{
	long long val = 0;
	for (int i = 0; i <scoreList.size(); i++)
		val += long long(res[i]) * scoreList[i];
	return val;
}

void chessRecord::init(vector<long long>s)
{
	scoreList = s;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 15; j++) {
			while (!herizon_score[i][j].empty())
				herizon_score[i][j].pop();
			herizon_score[i][j].push(0);

			while (!vertical_score[i][j].empty())
				vertical_score[i][j].pop();
			vertical_score[i][j].push(0);
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 29; j++) {
			while (!leftBt_rightTop_score[i][j].empty())
				leftBt_rightTop_score[i][j].pop();
			leftBt_rightTop_score[i][j].push(0);

			while (!leftTop_rightBt_score[i][j].empty())
				leftTop_rightBt_score[i][j].pop();
			leftTop_rightBt_score[i][j].push(0);
		}
	}
	for (int i = 0; i < ROW_RANGE; i++)
		for (int j = 0; j < COL_RANGE; j++) {
			herizon[0][i] += EMPTY;
			vertical[0][j] += EMPTY;
			herizon[1][i] += EMPTY;
			vertical[1][j] += EMPTY;
		}

	for (int i = 0; i < 15; i++)
		for (int j = 0; j < i + 1; j++) {
			leftBt_rightTop[0][i] += EMPTY;
			leftBt_rightTop[1][i] += EMPTY;
			leftTop_rightBt[0][i] += EMPTY;
			leftTop_rightBt[1][i] += EMPTY;
		}
	for (int i = 15; i < 29; i++)
		for (int j = i - 1; j >= 0; j--) {
			leftBt_rightTop[0][i] += EMPTY;
			leftBt_rightTop[1][i] += EMPTY;
			leftTop_rightBt[0][i] += EMPTY;
			leftTop_rightBt[1][i] += EMPTY;
		}
}

long long chessRecord::getScore(bool player)
{
	long long self_score=0, enemy_score=0;
	for (int i = 0; i < 15; i++) {
		self_score += herizon_score[player][i].top();
		self_score += vertical_score[player][i].top();
		enemy_score += herizon_score[!player][i].top();
		enemy_score += vertical_score[!player][i].top();
	}
	for (int i = 0; i < 29; i++) {
		self_score += leftBt_rightTop_score[player][i].top();
		self_score += leftTop_rightBt_score[player][i].top();
		enemy_score += leftBt_rightTop_score[!player][i].top();
		enemy_score += leftTop_rightBt_score[!player][i].top();
	}

	return self_score-enemy_score;
}

void chessRecord::makeChange(int row, int col, bool player, Automaton& AC)
{
	herizon[player][row][col] = '1';
	herizon[!player][row][col] = '0';
	
	herizon_score[player][row].push(countVal(AC.search(herizon[player][row])));
	herizon_score[!player][row].push(countVal(AC.search(herizon[!player][row])));
	

	vertical[player][col][row] = '1';
	vertical[!player][col][row] = '0';

	vertical_score[player][col].push(countVal(AC.search(vertical[player][col])));
	vertical_score[!player][col].push(countVal(AC.search(vertical[!player][col])));
	

	int index_vector_tb = 14-(row - col);// leftTop_rightBt
	int index_str_tb = min(row, col);
	leftTop_rightBt[player][index_vector_tb][index_str_tb] = '1';
	leftTop_rightBt[!player][index_vector_tb][index_str_tb] = '0';

	leftTop_rightBt_score[player][index_vector_tb].push(countVal(AC.search(leftTop_rightBt[player][index_vector_tb])));;
	leftTop_rightBt_score[!player][index_vector_tb].push(countVal(AC.search(leftTop_rightBt[!player][index_vector_tb])));;
		
	int index_vector_bt = col + row;
	int index_str_bt = min(14-row, col);
	leftBt_rightTop[player][index_vector_bt][index_str_bt] = '1';
	leftBt_rightTop[!player][index_vector_bt][index_str_bt] = '0';

	leftBt_rightTop_score[player][index_vector_bt].push(countVal(AC.search(leftBt_rightTop[player][index_vector_bt])));
	leftBt_rightTop_score[!player][index_vector_bt].push(countVal(AC.search(leftBt_rightTop[!player][index_vector_bt])));

}

void chessRecord::clear()
{
	init(scoreList);
}

void chessRecord::unmakeChange(int row, int col, bool player)
{
	herizon[player][row][col] = EMPTY;
	herizon[!player][row][col] = EMPTY;
	vertical[player][col][row] = EMPTY;
	vertical[!player][col][row] =EMPTY;

	herizon_score[player][row].pop();  //[0] 保存操作前，[1]存储最新
	herizon_score[!player][row].pop();
	vertical_score[player][col].pop();
	vertical_score[!player][col].pop();


	int index_vector_tb = 14 - (row - col);// leftTop_rightBt
	int index_str_tb = min(row, col);
	leftTop_rightBt[player][index_vector_tb][index_str_tb] = EMPTY;
	leftTop_rightBt[!player][index_vector_tb][index_str_tb] = EMPTY;
	leftTop_rightBt_score[player][index_vector_tb].pop(); 	//重复
	leftTop_rightBt_score[!player][index_vector_tb].pop();

	int index_vector_bt = col + row;
	int index_str_bt = min(14 - row, col);
	leftBt_rightTop[player][index_vector_bt][index_str_bt] = EMPTY;
	leftBt_rightTop[!player][index_vector_bt][index_str_bt] = EMPTY;

	leftBt_rightTop_score[player][index_vector_bt].pop();
	leftBt_rightTop_score[!player][index_vector_bt].pop();
}


const vector<vector<string>>& chessRecord::getRecord(int direction)
{
	if (direction == RECORD_DIR::herizon)
		return herizon;
	else if (direction ==RECORD_DIR::vertical)
		return vertical;
	else if (direction == RECORD_DIR::leftBt_rightTop)
		return leftBt_rightTop;
	else if (direction == RECORD_DIR::leftTop_rightBt)
		return leftTop_rightBt;
	// TODO: 在此处插入 return 语句
}
