#include"AI.h"
using namespace std;

_int64 rand64(void) {
	return rand() ^ ((_int64)rand() << 15) ^ ((_int64)rand() << 30) ^ ((_int64)rand() << 45) ^ ((_int64)rand() << 60);
}


void AI::ZobristMove(bool player, int rowIndex, int colIndex)
{
	if (player == COMPUTER)
		ZobristCode ^= (this->Zobrist_Ai[rowIndex][colIndex]);
	else
		ZobristCode ^= (this->Zobrist_Man[rowIndex][colIndex]);
}

void AI::ZobristInit() {
	for (int i = 0; i < ROW_RANGE; i++)
		for (int j = 0; j < COL_RANGE; j++) {
				this->Zobrist_Ai[i][j] = rand64();
				this->Zobrist_Man[i][j] = rand64();
			}
	ZobristCode = rand64();
}

void AI::ZobristCache(int depth,int score)
{
	if (ZobristMap.find(this->ZobristCode) != ZobristMap.end())
		ZobristMap.insert(pair<long long,ZobristInfo>( this->ZobristCode,{ depth,score }));
}

const ZobristInfo AI::ZobristFind(long long code)
{
	auto iter = ZobristMap.find(code);
	return iter == ZobristMap.end() ? ZobristInfo{ -1 ,-1} : iter->second;
}

