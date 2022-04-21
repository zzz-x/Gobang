#include "chessRecord.h"


int chessRecord::countVal(const vector<int>& res)
{
	int val = 0;
	for (int i = 0; i <scoreList.size(); i++)
		val += res[i] * scoreList[i];
	return val;
}

void chessRecord::init(vector<int>s)
{
	scoreList = s;
	memset(herizon_score, 0, sizeof(herizon_score));
	memset(vertical_score, 0, sizeof(vertical_score));
	memset(leftBt_rightTop_score, 0, sizeof(leftBt_rightTop_score));
	memset(leftTop_rightBt_score, 0, sizeof(leftTop_rightBt_score));
	for(int i=0;i<ROW_RANGE;i++)
		for (int j = 0; j < COL_RANGE; j++) {
			herizon[0][i] += EMPTY;
			vertical[0][j]+= EMPTY;
			herizon[1][i] += EMPTY;
			vertical[1][j] += EMPTY;
		}

	for(int i=0;i<15;i++)
		for (int j = 0; j < i + 1; j++) {
			leftBt_rightTop[0][i]+=EMPTY;
			leftBt_rightTop[1][i] += EMPTY;
			leftTop_rightBt[0][i] += EMPTY;
			leftTop_rightBt[1][i] += EMPTY;
		}
	for(int i=15;i<29;i++)
		for (int j = i - 1; j >= 0; j--) {
			leftBt_rightTop[0][i] += EMPTY;
			leftBt_rightTop[1][i] += EMPTY;
			leftTop_rightBt[0][i] += EMPTY;
			leftTop_rightBt[1][i] += EMPTY;
		}
}

int chessRecord::getScore(bool player)
{
	int self_score=0, enemy_score=0;
	for (int i = 0; i < 15; i++) {
		self_score += herizon_score[player][i][1];
		self_score += vertical_score[player][i][1];
		enemy_score += herizon_score[!player][i][1];
		enemy_score += vertical_score[!player][i][1];
	}
	for (int i = 0; i < 29; i++) {
		self_score += leftBt_rightTop_score[player][i][1];
		self_score += leftTop_rightBt_score[player][i][1];
		enemy_score += leftBt_rightTop_score[!player][i][1];
		enemy_score += leftTop_rightBt_score[!player][i][1];
	}

	return self_score-enemy_score;
}

void chessRecord::makeChange(int row, int col, bool player, Automaton& AC)
{
	herizon[player][row][col] = '1';
	herizon[!player][row][col] = '0';
	
	herizon_score[player][row][0] = herizon_score[player][row][1];  //[0] 保存操作前，[1]存储最新
	herizon_score[!player][row][0] = herizon_score[!player][row][1];
	herizon_score[player][row][1] = countVal(AC.search(herizon[player][row]));
	herizon_score[!player][row][1] = countVal(AC.search(herizon[!player][row]));


	vertical[player][col][row] = '1';
	vertical[!player][col][row] = '0';

	vertical_score[player][col][0] = vertical_score[player][col][1];
	vertical_score[!player][col][0] = vertical_score[!player][col][1];
	vertical_score[player][col][1] = countVal(AC.search(vertical[player][col]));
	vertical_score[!player][col][1] = countVal(AC.search(vertical[!player][col]));

	

	int index_vector_tb = 14-(row - col);// leftTop_rightBt
	int index_str_tb = min(row, col);
	leftTop_rightBt[player][index_vector_tb][index_str_tb] = '1';
	leftTop_rightBt[!player][index_vector_tb][index_str_tb] = '0';

	leftTop_rightBt_score[player][index_vector_tb][0] = leftTop_rightBt_score[player][index_vector_tb][1];
	leftTop_rightBt_score[!player][index_vector_tb][0] = leftTop_rightBt_score[!player][index_vector_tb][1];
	leftTop_rightBt_score[player][index_vector_tb][1] = countVal(AC.search(leftTop_rightBt[player][index_vector_tb]));
	leftTop_rightBt_score[!player][index_vector_tb][1] = countVal(AC.search(leftTop_rightBt[!player][index_vector_tb]));
	
	int index_vector_bt = col + row;
	int index_str_bt = min(14-row, col);
	leftBt_rightTop[player][index_vector_bt][index_str_bt] = '1';
	leftBt_rightTop[!player][index_vector_bt][index_str_bt] = '0';

	leftBt_rightTop_score[player][index_vector_bt][0] = leftBt_rightTop_score[player][index_vector_bt][1];
	leftBt_rightTop_score[!player][index_vector_bt][0] = leftBt_rightTop_score[!player][index_vector_bt][1];
	leftBt_rightTop_score[player][index_vector_bt][1] = countVal(AC.search(leftBt_rightTop[player][index_vector_bt]));
	leftBt_rightTop_score[!player][index_vector_bt][1] = countVal(AC.search(leftBt_rightTop[!player][index_vector_bt]));

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

	herizon_score[player][row][1] = herizon_score[player][row][0];  //[0] 保存操作前，[1]存储最新
	herizon_score[!player][row][1] = herizon_score[!player][row][0];
	vertical_score[player][col][1] = vertical_score[player][col][0];
	vertical_score[!player][col][1] = vertical_score[!player][col][0];



	int index_vector_tb = 14 - (row - col);// leftTop_rightBt
	int index_str_tb = min(row, col);
	leftTop_rightBt[player][index_vector_tb][index_str_tb] = EMPTY;
	leftTop_rightBt[!player][index_vector_tb][index_str_tb] = EMPTY;
	leftTop_rightBt_score[player][index_vector_tb][1] = leftTop_rightBt_score[player][index_vector_tb][0];
	leftTop_rightBt_score[!player][index_vector_tb][1] = leftTop_rightBt_score[!player][index_vector_tb][0];



	int index_vector_bt = col + row;
	int index_str_bt = min(14 - row, col);
	leftBt_rightTop[player][index_vector_bt][index_str_bt] = EMPTY;
	leftBt_rightTop[!player][index_vector_bt][index_str_bt] = EMPTY;

	leftBt_rightTop_score[player][index_vector_bt][1] = leftBt_rightTop_score[player][index_vector_bt][0];
	leftBt_rightTop_score[!player][index_vector_bt][1] = leftBt_rightTop_score[!player][index_vector_bt][0];

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
