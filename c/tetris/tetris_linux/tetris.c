/*
 * Description: Tetris Simple Demo in Linux
 * Date: 2019-5-31
 * By: Xie Xingchao
 */

#include "tetris.h"
#include "kbhit.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int TIME_SPAN_3 = 30;
const int TIME_SPAN_2 = 20;
const int TIME_SPAN_1 = 10;

TETRIS oldTetris;
TETRIS curTetris;
TETRIS nxtTetris;

enum GAME_OVER_OPTION { NEW_GAME = 1, EXIT_GAME };

char **map;
int score = 0;

int main() {
newGame:
  int isGameOver          = 0;
  int isCurTetrisStopMove = 0;
  char keydown            = '@';
  int timespan            = TIME_SPAN_1;
  int isHighlightDone     = 0;
  int fullRowIndex        = 0;
  int fullRowStartIndex   = 0;
  int fullRowEndIndex     = 0;
  enum GAME_OVER_OPTION gameOverOption;
  // 隐藏光标
  printf("\033[?25l");
  map = initMap();
  initRandomTetris(&nxtTetris);
  system("clear");
  while (!isGameOver) {
    tetris_copy(&curTetris, &nxtTetris);
    initRandomTetris(&nxtTetris);
    while (!isCurTetrisStopMove) {
      tetris_copy(&oldTetris, &curTetris);
      // 移动光标至（0，0）处
      printf("\033[%d;%dH", 0, 0);
      printMap(map);
      printGameState(&nxtTetris);
      clearTetris(&oldTetris);
      if (kbhit()) {
        keydown = getchar();
        switch (keydown) {
        case 'a':
        case 'A':
          curTetris.x -= 1;
          //  移动后会与物体重叠，进行回退操作
          if (isObjectAside(&curTetris))
            curTetris.x += 1;
          break;
        case 'd':
        case 'D':
          curTetris.x += 1;
          if (isObjectAside(&curTetris))
            curTetris.x -= 1;
          break;
        case 'w':
        case 'W':
          curTetris.type =
              (curTetris.type + 1) % TYPES_PER_KIND[curTetris.kind];
          // 改变形态后会与物体重叠，进行回退操作
          if (isObjectAside(&curTetris)) {
            curTetris.type = curTetris.type == 0
                                 ? TYPES_PER_KIND[curTetris.kind] - 1
                                 : curTetris.type - 1;
          }
          break;
        case 's':
        case 'S':
          timespan = 0;
          break;
        default:
          break;
        }
      }
      if (!timespan--) {
        timespan = TIME_SPAN_1;
        curTetris.y += 1;
        if (isObjectAside(&curTetris)) {
          // 与物体发生重叠，且未完全出现在顶部墙壁之下，游戏结束
          if (!isFullAppear(&curTetris)) {
            curTetris.y -= 1;
            writeTetris(&curTetris);
            resetCursor();
            printMap(map);
            isGameOver = 1;
            break;
          }
          // 回退操作
          curTetris.y -= 1;
          writeTetris(&curTetris);
          // 检查有无完全填充的行
          fullRowStartIndex = -1;
          fullRowEndIndex   = -1;
          while (1) {
            fullRowIndex = hasFullRow();
            if (!fullRowIndex)
              break;
            if (fullRowIndex > fullRowStartIndex)
              fullRowStartIndex = fullRowIndex;
            else
              fullRowEndIndex = fullRowIndex;
            isHighlightDone = 1;
            score += 100;
            // 对完全填充的行进行样式更改
            highlightFullRow(fullRowIndex);
            resetCursor();
            printMap(map);
          }
          // 对之前找到的完全填充行清除
          if (isHighlightDone == 1) {
            isHighlightDone = 0;
            if (fullRowEndIndex == -1)
              fullRowEndIndex = fullRowStartIndex;
            clearFullRow(fullRowStartIndex, fullRowEndIndex);
            // 消除“动画”过渡时间
            usleep(250000);
          }
          break;
        }
      }
      writeTetris(&curTetris);
      // 预计以 1 秒 30 帧显示
      usleep(33333);
    }
  }
  // 显示游戏结束画面
  system("clear");
  printf("%s\n", GAME_OVER_INFO);
  printf("Your score is : %d\n", score);
  printf("Press [1] to start a new game, and [2] exit game.\n");
  printf("Your choice: ");
  scanf("%d", &gameOverOption);
  switch (gameOverOption) {
  case NEW_GAME:
    isGameOver = 0;
    goto newGame;
    break;
  case EXIT_GAME:
    return 0;
  default:
    break;
  }
}

/*
 * 函数定义
 */

char **initMap() {
  map = (char **)malloc(sizeof(char *) * MAP_ROW);
  for (int row = 0; row < MAP_ROW; ++row) {
    *(map + row) = (char *)malloc(sizeof(char) * MAP_COL);
    if (row == 0 || row == MAP_ROW - 1) {
      for (int col = 0; col < MAP_COL; ++col) {
        *(*(map + row) + col) = WALL;
      }
      continue;
    } else {
      for (int col = 1; col < MAP_COL - 1; ++col) {
        *(*(map + row) + col) = BLANK_SPACE;
      }
    }
    *(*(map + row) + 0)           = WALL;
    *(*(map + row) + MAP_COL - 1) = WALL;
  }
  return map;
}

void printMap(char **map) {
  for (int row = 0; row < MAP_ROW; ++row) {
    for (int col = 0; col < MAP_COL; ++col) {
      if (*(*(map + row) + col) == WALL)
        printf("\033[34m%c", *(*(map + row) + col));
      else if (*(*(map + row) + col) == '/')
        printf("\033[35m%c", *(*(map + row) + col));
      else
        printf("\033[36m%c", *(*(map + row) + col));
    }
    printf(" \n");
  }
}

void printGameState(PTETRIS nxtTetris) {
  printf("\033[%d;%dH", GAME_STATE_ROW_START, GAME_STATE_COL_START);
  printf("[The next]");
  // 清除旧输出
  for (int row = 0; row < 4; ++row) {
    printf("\033[%d;%dH", GAME_STATE_ROW_START + 1 + row,
           GAME_STATE_COL_START + 3);
    fillBlankSpace(4);
  }
  for (int row = 0; row < 4; ++row) {
    printf("\033[%d;%dH", GAME_STATE_ROW_START + 1 + row,
           GAME_STATE_COL_START + 3);
    for (int col = 0; col < 4; ++col) {
      if (TETRIS_DATA[nxtTetris->kind][nxtTetris->type][row][col]) {
        printf("%c", C_TETRIS);
      } else {
        printf(" ");
      }
    }
  }
  // 清除旧输出
  printf("\033[%d;%dH", GAME_STATE_ROW_START + 5, GAME_STATE_COL_START);
  fillBlankSpace(20);
  printf("\033[%d;%dH", GAME_STATE_ROW_START + 5, GAME_STATE_COL_START);
  printf("[Score: %d]", score);
}

void tetris_copy(PTETRIS desti, PTETRIS src) {
  desti->kind = src->kind;
  desti->type = src->type;
  desti->x    = src->x;
  desti->y    = src->y;
}

void writeTetris(PTETRIS pTetris) {
  for (int row = 0; row < 4; ++row) {
    if (pTetris->y + row <= 0)
      continue;
    for (int col = 0; col < 4; ++col) {
      if (TETRIS_DATA[pTetris->kind][pTetris->type][row][col]) {
        *(*(map + pTetris->y + row) + pTetris->x + col) = C_TETRIS;
      }
    }
  }
}

void clearTetris(PTETRIS pTetris) {
  for (int row = 0; row < 4; ++row) {
    if (pTetris->y + row <= 0)
      continue;
    for (int col = 0; col < 4; ++col) {
      if (TETRIS_DATA[pTetris->kind][pTetris->type][row][col]) {
        *(*(map + pTetris->y + row) + pTetris->x + col) = BLANK_SPACE;
      }
    }
  }
}

void initRandomTetris(PTETRIS pTetris) {
  srand(time(NULL));
  pTetris->x    = MAP_COL - 2;
  pTetris->y    = 0 - 3;
  pTetris->kind = rand() % 7;
  pTetris->type = rand() % TYPES_PER_KIND[pTetris->kind];
}

int isObjectAside(PTETRIS pTetris) {
  for (int row = 0; row < 4; ++row) {
    if (pTetris->y + row <= 0)
      continue;
    for (int col = 0; col < 4; ++col) {
      if (TETRIS_DATA[pTetris->kind][pTetris->type][row][col] &&
          (*(*(map + pTetris->y + row) + pTetris->x + col) == WALL ||
           *(*(map + pTetris->y + row) + pTetris->x + col) == C_TETRIS)) {
        return 1;
      }
    }
  }
  return 0;
}

int hasFullRow() {
  int flag = 1;
  for (int row = MAP_ROW - 2; row >= 1; --row) {
    for (int col = 1; col < MAP_COL - 1; ++col) {
      if (*(*(map + row) + col) != C_TETRIS) {
        flag = 0;
        continue;
      }
    }
    if (flag == 1) {
      return row;
    }
  }
  return 0;
}

void clearFullRow(int rowStartIndex, int rowEndIndex) {
  int offset = rowStartIndex - rowEndIndex + 1;
  for (int row = rowEndIndex - 1; row > 1; --row) {
    for (int col = 1; col < MAP_COL - 1; ++col) {
      *(*(map + row + offset) + col) = *(*(map + row) + col);
    }
  }
  for (int col = 1; col < MAP_COL - 1; ++col) {
    *(*(map + 1) + col) = BLANK_SPACE;
  }
}

void highlightFullRow(int rowIndex) {
  for (int col = 1; col < MAP_COL - 1; ++col) {
    *(*(map + rowIndex) + col) = '/';
  }
}

void resetCursor() { printf("\033[%d;%dH", 0, 0); }

int isFullAppear(PTETRIS pTetris) {
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      if (TETRIS_DATA[pTetris->kind][pTetris->type][row][col]) {
        if (pTetris->y + row > 0) {
          return 1;
        }
        return 0;
      }
    }
  }
  return 0;
}

void fillBlankSpace(int len) {
  while (len--) {
    printf(" ");
  }
}