/*
 * 题目名称：
 * 买股票的最佳时机
 * best time to buy and sell stock
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
// 方法1
// 执行用时：1344ms
int maxProfit(int* prices, int pricesSize){
  int profit = 0;
  int tmpProfit = 0;
  for (int i = 0; i < pricesSize; ++i) {
    for (int j = i + 1; j < pricesSize; ++j) {
      if (prices[j] < prices[i])
        continue;
      tmpProfit = prices[j] - prices[i];
      if (tmpProfit > profit)
        profit = tmpProfit;
    }
  }
  return profit;
}
// 方法2
// 执行用时：4ms
int maxProfit(int* prices, int pricesSize){
  int bestBuyPrice = INT_MAX;
  int bestSellPrice = -1;
  int profit = 0;
  int tmpProfit = 0;
  for (int i = 0; i < pricesSize; ++i) {
    if (prices[i] < bestBuyPrice) {
      bestBuyPrice = prices[i];
      bestSellPrice = -1;
      continue;
    }
    if (prices[i] > bestSellPrice) {
      bestSellPrice = prices[i];
      tmpProfit = bestSellPrice - bestBuyPrice;
      if (tmpProfit > profit) {
        profit = tmpProfit;
      }
      continue;
    }
  }
  return profit;
}

int main() {
    int testCaseCount = 5;
    srand(time(NULL));
    while (testCaseCount--) {
        int len = rand() % 10;
        int * prices = (int *)calloc(len, sizeof(int));
        for (int i = 0; i < len; ++i) {
            prices[i] = rand() % 100;
            printf("%d ", prices[i]);
        }
        printf("\n");
        printf("profit = %d\n", maxProfit(prices, len));
    }
}
