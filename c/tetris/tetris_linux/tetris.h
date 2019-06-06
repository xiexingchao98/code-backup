#pragma once
#ifndef _TETRIS_H_
#define TETRIS_H

#define MAP_ROW 20
#define MAP_COL 20

const char *GAME_OVER_INFO     = "GG, Man~";
const int GAME_STATE_ROW_START = MAP_ROW / 3;
const int GAME_STATE_COL_START = MAP_COL + 5;

// ■
const char WALL        = '.';
const char BLANK_SPACE = ' ';
const char C_TETRIS    = '@';

// 所有方块类型的数组
const int TETRIS_DATA[7][4][4][4] = {
  // 第1种
  { { {1,1,0,0}, {1,1,0,0}, {0} ,{0} } },
  // 第2种
  { { {1,1,1,1}, {0}, {0}, {0} }, { {1}, {1}, {1}, {1} } },
  // 第3种
  { { {0,1,1,0}, {1,1,0,0}, {0}, {0} }, { {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0} } },
  // 第4种
  { { {1,1,0,0,}, {0,1,1,0}, {0}, {0} }, { {0,1,0,0}, {1,1,0,0,}, {1,0,0,0}, {0} } },
  // 第5种
  { { {1,1,0,0}, {1,0,0,0}, {1,0,0,0}, {0} }, { {1,1,1,0}, {0,0,1,0}, {0}, {0} }, { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0} }, { {1,0,0,0}, {1,1,1,0}, {0}, {0} } },
  // 第6种
  { { {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0} }, { {1,0,0,0}, {1,0,0,0}, {1,1,0,0} }, { {1,1,1,0}, {1,0,0,0}, {0}, {0} }, { {0,0,1,0}, {1,1,1,0}, {0}, {0}} },
  // 第7种
  { { {0,1,0,0}, {1,1,1,0}, {0}, {0} }, { {1,1,1,0}, {0,1,0,0}, {0}, {0} }, { {1,0,0,0}, {1,1,0,0}, {1,0,0,0}, {0} }, { {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0} } }
};
// 每种类型所含有的方块数
const int TYPES_PER_KIND[] = {1, 2, 2, 2, 4, 4, 4};

typedef struct _TETRIS {
  int x;
  int y;
  int kind;
  int type;
} TETRIS, *PTETRIS;

char **initMap();
void printMap(char **map);
void printGameState(PTETRIS pTetris);
void initRandomTetris(PTETRIS pTetris);
void tetris_copy(PTETRIS desti, PTETRIS src);
void clearTetris(PTETRIS pTetris);
void writeTetris(PTETRIS pTetris);
int isObjectAside(PTETRIS pTetris);
int isFullRow(int row);
void clearFullRow(int rowIndex);
void highlightFullRow(int rowIndex);
void resetCursor();
int isFullAppear(PTETRIS pTetris);
void fillBlankSpace(int len);
#endif