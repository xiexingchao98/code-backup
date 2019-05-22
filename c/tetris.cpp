#include "pch.h"
#include <stdio.h>
void printSquare(int square[4][4]);
void printType(int type[4][4][4], int count);
void printAllSquare(int types[7][4][4][4], int type_square_count[]);
void printDivider();
int main() {
	// 所有方块类型的数组
	int types[7][4][4][4] = {
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
	int type_square_count[] = { 1, 2, 2, 2, 4, 4, 4 };
	// 打印全部方块
	printAllSquare(types, type_square_count);
}

void printSquare(int square[4][4]) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (square[i][j] == 0)
				printf("  ");
			else
				printf("■");
		}
		printf("\n");
	}
}

void printType(int type[4][4][4], int square_count) {
	for (int i = 0; i < square_count; ++i) {
		printf("第 %d 种\n", i + 1);
		printSquare(type[i]);
	}
}

void printAllSquare(int types[7][4][4][4], int type_square_count[]) {
	for (int i = 0; i < 7; ++i) {
		printf("类型 %d\n", i + 1);
		printType(types[i], type_square_count[i]);
		printDivider();
	}
}

void printDivider() {
	int loop = 50;
	while (loop--) {
		printf("=");
	}
	printf("\n");
}