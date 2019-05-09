/*
 * Date: 2019-5-8
 * By: Xie Xingchao
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "default.dat"
#define DATA_FILE_TEMP "tmp.dat"
#define LINE_BUFFER_SIZE 120
#define GLOBAL_OPTION_QUERY 1
#define GLOBAL_OPTION_ADD 2
#define GLOBAL_OPTION_UPDATE 3
#define GLOBAL_OPTION_DELETE 4
#define GLOBAL_OPTION_QUERYALL 5
#define GLOBAL_OPTION_QUERYALL_SORT_BY_SALARY 6
#define GLOBAL_OPTION_EXIT 7
#define FILTER_OPTION_NUMBER 1
#define FILTER_OPTION_NAME 2
#define EMPLOYEE_NUMBER_INDEX 1
#define EMPLOYEE_NAME_INDEX 2
#define EMPLOYEE_SALARY_INDEX 3

/* struct define */
typedef struct filter {
  int key;
  char value[20];
} Filter;

typedef struct employee {
  char number[10];
  char name[20];
  float salary;
} Employee;

/* basic print functions */
void printDivider();
void pressAnyKeyContinue();
void printGlobalMenu();

/* interactive functions */
Filter getFilter();
Filter getNewValueFilter();
Employee getEmployee();
Employee getEmployeeFromString(char *);

/* data access functions */
void checkDataFile();
int isTarget(char *, Filter *);
void execQuery(Filter *filter);
void execQueryAll();
void execQueryAllSortBySalary();
void execAdd(Employee *employee);
void execUpdate(Filter *filter, Filter *newValue);
void execDelete(Filter *filter);

char TAB = '\t';
int globalOption = 0;
int filterOption = 0;

int main() {
  checkDataFile();
  do {
    printGlobalMenu();
    scanf("%d", &globalOption);
    getchar();
    switch (globalOption) {
    case GLOBAL_OPTION_QUERY: {
      Filter filter = getFilter();
      execQuery(&filter);
      break;
    }
    case GLOBAL_OPTION_ADD: {
      Employee emp = getEmployee();
      execAdd(&emp);
      break;
    }
    case GLOBAL_OPTION_UPDATE: {
      Filter filter = getFilter();
      Filter newValueFilter = getNewValueFilter();
      execUpdate(&filter, &newValueFilter);
      break;
    }
    case GLOBAL_OPTION_DELETE: {
      Filter filter = getFilter();
      execDelete(&filter);
      break;
    }
    case GLOBAL_OPTION_QUERYALL: {
      execQueryAll();
      break;
    }
    case GLOBAL_OPTION_QUERYALL_SORT_BY_SALARY: {
      execQueryAllSortBySalary();
      break;
    }
    case GLOBAL_OPTION_EXIT:
      printf("\n再见～\n");
      return 0;
    default:
      break;
    }
  } while (1);
}

void printDivider() {
  int loop = 50;
  printf("\n");
  while (loop--) {
    printf("#");
  }
  printf("\n\n");
}

void pressAnyKeyContinue() {
  printf("请按任意健继续……");
  getchar();
}

void printGlobalMenu() {
  system("clear");
  printf("欢迎使用～\n");
  printDivider();
  printf("1. 查询职工\n");
  printf("2. 添加职工\n");
  printf("3. 修改职工\n");
  printf("4. 删除职工\n");
  printf("5. 查询全体职工（默认排序）\n");
  printf("6. 查询全体职工（工资排序）\n");
  printf("7. *退出\n");
  printDivider();
  printf("输入[编号]选择功能：");
}

Filter getFilter() {
  system("clear");
  printf("通过何种方式操作？\n");
  printDivider();
  printf("1. 编号\n");
  printf("2. 名字\n");
  printDivider();
  printf("输入[编号]选择功能：");

  Filter filter;
  char *tips;

  scanf("%d", &filterOption);
  switch (filterOption) {
  case FILTER_OPTION_NUMBER:
    filter.key = EMPLOYEE_NUMBER_INDEX;
    tips = "请输入编号：";
    break;
  case FILTER_OPTION_NAME:
    filter.key = EMPLOYEE_NAME_INDEX;
    tips = "请输入姓名：";
    break;
  default:
    break;
  }
  system("clear");
  printf("%s", tips);
  scanf("%s", filter.value);
  getchar();
  return filter;
}

Filter getNewValueFilter() {
  Filter newValueFilter;

  system("clear");
  printf("更改哪一列的值？\n");
  printf("1. 编号\n");
  printf("2. 姓名\n");
  printf("3. 薪水\n");

  printf("输入[编号]选择功能：");
  scanf("%d", &newValueFilter.key);

  system("clear");
  printf("请输入[目标值]：");
  scanf("%s", newValueFilter.value);

  getchar();

  return newValueFilter;
}

Employee getEmployee() {
  Employee emp;

  system("clear");
  printf("请输入职工[编号]：");
  scanf("%s", emp.number);

  system("clear");
  printf("请输入职工[姓名]：");
  scanf("%s", emp.name);

  system("clear");
  printf("请输入职工[薪水]：");
  scanf("%f", &emp.salary);

  getchar();

  return emp;
}

Employee getEmployeeFromString(char *string) {
  Employee emp;
  strcpy(emp.number, strtok(string, &TAB));
  strcpy(emp.name, strtok(NULL, &TAB));
  emp.salary = atof(strtok(NULL, &TAB));
  return emp;
}

void checkDataFile() {
  FILE *in = fopen(DATA_FILE, "r");
  if (NULL == in) {
    in = fopen(DATA_FILE, "w");
  }
  fclose(in);
}

void execQuery(Filter *filter) {
  FILE *in = fopen(DATA_FILE, "r");
  char buffer[LINE_BUFFER_SIZE];
  int isEmployeeExist = 0;
  while (fgets(buffer, 120, in) != NULL) {
    if (isTarget(buffer, filter)) {
      isEmployeeExist = 1;
      break;
    }
  }
  if (fclose(in) == 0) {
    if (isEmployeeExist == 1) {
      printf("找到职工信息如下：\n");
      printDivider();
      printf("%s\n", buffer);
      printDivider();
      pressAnyKeyContinue();
      return;
    }
    printf("没有找到相关记录！\n");
    pressAnyKeyContinue();
    return;
  }

  printf("查询失败！\n");
  pressAnyKeyContinue();
}

void execQueryAll() {
  FILE *in = fopen(DATA_FILE, "r");
  char buffer[LINE_BUFFER_SIZE];

  system("clear");
  printf("查询结果如下：\n");
  printDivider();

  while (fgets(buffer, LINE_BUFFER_SIZE, in) != NULL) {
    printf("%s", buffer);
  }

  fclose(in);

  printDivider();
  pressAnyKeyContinue();
}

void execQueryAllSortBySalary() {
  FILE *in = fopen(DATA_FILE, "r");
  char buffer[LINE_BUFFER_SIZE];
  int count = 0;
  int empsLength = 0;
  Employee emps[10];
  Employee *empsPtr = emps;
  while (fgets(buffer, LINE_BUFFER_SIZE, in) != NULL) {
    empsLength = sizeof(emps) / sizeof(Employee);
    if (count > empsLength) {
      empsPtr =
          (Employee *)realloc(empsPtr, (empsLength + 3) * sizeof(Employee));
    }
    emps[count] = getEmployeeFromString(buffer);
    count++;
  }
  fclose(in);

  int empIndex[count];
  int i, j;
  for (i = 0; i < count; ++i) {
    empIndex[i] = i;
  }
  int temp;
  for (i = 0; i < count - 1; ++i) {
    for (j = i + 1; j < count; ++j) {
      if (empsPtr[empIndex[j]].salary > empsPtr[empIndex[i]].salary) {
        temp = empIndex[i];
        empIndex[i] = empIndex[j];
        empIndex[j] = temp;
      }
    }
  }
  system("clear");
  printf("查询结果按工资排序：\n");
  printDivider();
  for (i = 0; i < count; ++i) {
    printf("%s\t%s\t%f\n", empsPtr[empIndex[i]].number,
           empsPtr[empIndex[i]].name, empsPtr[empIndex[i]].salary);
  }
  printDivider();
  pressAnyKeyContinue();
}

void execAdd(Employee *emp) {
  FILE *out = fopen(DATA_FILE, "a");

  fprintf(out, "%s", emp->number);
  fprintf(out, "\t");
  fprintf(out, "%s", emp->name);
  fprintf(out, "\t");
  fprintf(out, "%.2f", emp->salary);
  fprintf(out, "\n");

  system("clear");

  if (fclose(out) == 0) {
    printf("添加职工成功！\n");
    pressAnyKeyContinue();
    return;
  }

  printf("添加职工失败！\n");
  pressAnyKeyContinue();
}

void execUpdate(Filter *filter, Filter *newValueFilter) {
  FILE *in = fopen(DATA_FILE, "r");
  FILE *out = fopen(DATA_FILE_TEMP, "w");
  char buffer[LINE_BUFFER_SIZE];
  int isUpdateComplete = 0;
  while (fgets(buffer, LINE_BUFFER_SIZE, in) != NULL) {
    if (isUpdateComplete == 0 && isTarget(buffer, filter)) {
      isUpdateComplete = 1;
      Employee emp = getEmployeeFromString(buffer);
      switch (newValueFilter->key) {
      case EMPLOYEE_NUMBER_INDEX:
        strcpy(emp.number, newValueFilter->value);
        break;
      case EMPLOYEE_NAME_INDEX:
        strcpy(emp.name, newValueFilter->value);
        break;
      case EMPLOYEE_SALARY_INDEX:
        emp.salary = atof(filter->value);
        break;
      default:
        break;
      }
      fprintf(out, "%s\t%s\t%.2f\n", emp.number, emp.name, emp.salary);
      continue;
    }
    fprintf(out, "%s", buffer);
  }
  fclose(in);
  fclose(out);
  if (isUpdateComplete == 1) {
    remove(DATA_FILE);
    rename(DATA_FILE_TEMP, DATA_FILE);
    printf("修改成功！\n");
  } else
    printf("没有找到对应记录，所以无法修改！\n");
  pressAnyKeyContinue();
}

void execDelete(Filter *filter) {
  FILE *in = fopen(DATA_FILE, "r");
  FILE *out = fopen(DATA_FILE_TEMP, "w");
  char buffer[LINE_BUFFER_SIZE];
  int isDeleteComplete = 0;
  while (fgets(buffer, LINE_BUFFER_SIZE, in) != NULL) {
    if (isDeleteComplete == 0 && isTarget(buffer, filter)) {
      isDeleteComplete = 1;
      continue;
    }
    fprintf(out, "%s", buffer);
  }

  fclose(in);
  fclose(out);
  remove(DATA_FILE);
  rename(DATA_FILE_TEMP, DATA_FILE);

  if (isDeleteComplete)
    printf("删除成功！\n");
  else
    printf("删除失败，该记录不存在！\n");
  pressAnyKeyContinue();
}

int isTarget(char *buffer, Filter *filter) {
  char tmpBuffer[LINE_BUFFER_SIZE];
  strcpy(tmpBuffer, buffer);
  strtok(tmpBuffer, &TAB);
  int i = 1;
  while (i++ < filter->key) {
    strtok(NULL, &TAB);
  }
  if (strcmp(tmpBuffer, filter->value) == 0)
    return 1;
  return 0;
}