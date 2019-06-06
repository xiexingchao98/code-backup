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

enum GAME_OVER_OPTION { NEW_GAME = 1, EXIT_GAME };
enum LEVEL {EASY = 1, MEDIUM, HARD};
enum LEVEL_TIME_SPAN { EASY_TIME_SPAN = 12, MEDIUM_TIME_SPAN = 9, HARD_TIME_SPAN = 7 };
char **map;
int score = 0;

int main() {
  int isGameOver          = 0;
  int isCurTetrisStopMove = 0;
  char keydown            = '@';
  int isHighlightDone     = 0;
  int fullRowIndex        = 0;
  int fullRowStartIndex   = 0;
  int fullRowEndIndex     = 0;
  enum GAME_OVER_OPTION gameOverOption;
  enum LEVEL level;
  int defaultTimeSpan, timeSpan;
  TETRIS oldTetris;
  TETRIS curTetris;
  TETRIS nxtTetris;  
  // 隐藏光标
  printf("\033[?25l");
newGame:
  /*
   * 难度选择
   */
  printf("Level selection: [1. Easy] [2. Medium] [3. Hard]\n");
  scanf("%d", &level);
  switch(level) {
    case EASY:
      defaultTimeSpan = EASY_TIME_SPAN;
      break;
    case MEDIUM:
      defaultTimeSpan = MEDIUM_TIME_SPAN;
      break;
    case HARD:
      defaultTimeSpan = HARD_TIME_SPAN;
      break;
  }
  timeSpan = defaultTimeSpan;
  system("clear");
  map = initMap();
  initRandomTetris(&nxtTetris);
  while (!isGameOver) {
    tetris_copy(&curTetris, &nxtTetris);
    initRandomTetris(&nxtTetris);
    /*
     * 方块移动
     */
    while (!isCurTetrisStopMove) {
      tetris_copy(&oldTetris, &curTetris);
      // 移动光标至（0，0）处
      printf("\033[%d;%dH", 0, 0);
      printMap(map);
      printGameState(&nxtTetris);
      clearTetris(&oldTetris);
      /*
       * 按键检测
       */
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
          timeSpan = 0;
          break;
        default:
          break;
        }
      }
      /*
       * 一定间隔后，方块自由下落一个单位，并重置间隔
       */
      if (!timeSpan--) {
        timeSpan = defaultTimeSpan;
        curTetris.y += 1;
        if (isObjectAside(&curTetris)) {
          curTetris.y -= 1;
          // 与物体发生重叠，且未完全出现在顶部墙壁之下，游戏结束
          if (!isFullAppear(&curTetris)) {
            writeTetris(&curTetris);
            resetCursor();
            printMap(map);
            isGameOver = 1;
            break;
          }
          writeTetris(&curTetris);
          /*
           * 检测并清除满行
           */
          for (int row = MAP_ROW - 2; row > 0;) {
            if (isFullRow(row)) {
              highlightFullRow(row);
              resetCursor();
              printMap(map);
              usleep(200000);
              resetCursor();
              clearFullRow(row);
              printMap(map);
              score += 100;
              row = MAP_ROW - 2;
              continue;
            }
            row--;
          }
          break;
        }
      }
      writeTetris(&curTetris);
      usleep(33333);      
    }
  }
  // 游戏结束画面
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
 * 函数定义区域
 */

/*
 * 初始化地图数据
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

/*
 * 打印地图
 */
void printMap(char **map) {
  for (int row = 0; row < MAP_ROW; ++row) {
    for (int col = 0; col < MAP_COL; ++col) {
      // 输出墙壁
      if (*(*(map + row) + col) == WALL)
        printf("\033[34m%c", *(*(map + row) + col));
      // 输出满行
      else if (*(*(map + row) + col) == '/')
        printf("\033[35m%c", *(*(map + row) + col));
      // 输出方块
      else
        printf("\033[36m%c", *(*(map + row) + col));
    }
    printf(" \n");
  }
}

/*
 * 打印当前游戏状态
 */
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

/*
 * 复制方块数据
 */
void tetris_copy(PTETRIS desti, PTETRIS src) {
  desti->kind = src->kind;
  desti->type = src->type;
  desti->x    = src->x;
  desti->y    = src->y;
}

/*
 * 在地图上写入方块数据
 */
void writeTetris(PTETRIS pTetris) {
  for (int row = 0; row < 4; ++row) {
    if (pTetris->y + row <= 0)
      continue;
    for (int col = 0; col < 4; ++col) {
      if (TETRIS_DATA[pTetris->kind][pTetris->type][row][col])
        *(*(map + pTetris->y + row) + pTetris->x + col) = C_TETRIS;
    }
  }
}

/*
 * 从地图上清除方块数据
 */
void clearTetris(PTETRIS pTetris) {
  for (int row = 0; row < 4; ++row) {
    if (pTetris->y + row <= 0)
      continue;
    for (int col = 0; col < 4; ++col) {
      if (TETRIS_DATA[pTetris->kind][pTetris->type][row][col])
        *(*(map + pTetris->y + row) + pTetris->x + col) = BLANK_SPACE;
    }
  }
}

/*
 * 随即初始化方块数据
 */
void initRandomTetris(PTETRIS pTetris) {
  srand(time(NULL));
  pTetris->x    = MAP_COL / 2;
  pTetris->y    = 0 - 3;
  pTetris->kind = rand() % 7;
  pTetris->type = rand() % TYPES_PER_KIND[pTetris->kind];
  /* debug */
  // pTetris->kind = 1;
  // pTetris->type = 0;
}

/*
 * 判断方块移动后是否会与物体重叠
 */
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

/*
 * 判断行是否满了
 */
int isFullRow(int rowIndex) {
  for (int col = 1; col < MAP_COL - 1; ++col) {
    if (*(*(map + rowIndex) + col) == BLANK_SPACE)
      return 0;
  }
  return 1;
}

/*
 * 清除已满的行
 */
void clearFullRow(int rowIndex) {
  for (int row = rowIndex; row > 1; --row) {
    for (int col = 1; col < MAP_COL - 1; ++col) {
      *(*(map + row) + col) = *(*(map + row - 1) + col);
    }
  }
  for (int col = 1; col < MAP_COL - 1; ++col) {
    *(*(map + 1) + col) = BLANK_SPACE;
  }
}

/*
 * 高亮已满的行，构建消除时的动态效果
 */
void highlightFullRow(int rowIndex) {
  for (int col = 1; col < MAP_COL - 1; ++col) {
    *(*(map + rowIndex) + col) = '/';
  }
}

/*
 * 将光标移动到 0，0 位置
 */
void resetCursor() { printf("\033[%d;%dH", 0, 0); }

/*
 * 判断方块是否完全出现
 */
int isFullAppear(PTETRIS pTetris) {
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      if (TETRIS_DATA[pTetris->kind][pTetris->type][row][col]) {
        if (pTetris->y + row > 0)
          return 1;
        return 0;
      }
    }
  }
  return 0;
}

/*
 * 填充指定数量的空格字符
 */
void fillBlankSpace(int len) {
  while (len--) {
    printf(" ");
  }
}