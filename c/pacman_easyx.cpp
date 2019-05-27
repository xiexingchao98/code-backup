#include <graphics.h>
#include <conio.h>

#define PACMAN_YELLOW RGB(252, 207, 0)
#define PACMAN_RADIUS_MAX 100

double getRadian(double degree) {
	return 0.0174532254 * degree;
}

struct unique_pacman {
	int x;
	int y;
	int radius;
} upacman;

struct mouth {
	int left;
	int top;
	int right;
	int bottom;
	double degree_start_1;
	double degree_end_1;
	double degree_start_2;
	double degree_end_2;
} mouthstate;    // 嘴巴初态、终态数据

enum MOUTH_STATE {
	MOUTH_OPEN = 0, MOUTH_CLOSE
};

void init() {
	upacman.x = getmaxx() / 2;
	upacman.y = getmaxy() / 2;
	upacman.radius = 1;

	mouthstate.degree_start_1 = -15;
	mouthstate.degree_end_1 = 15;
	mouthstate.degree_start_2 = -30;
	mouthstate.degree_end_2 = 30;
}

void printPacman(int x, int y, int r) {
	setfillcolor(PACMAN_YELLOW);
	solidcircle(x, y, r);
	clearpie(x - r, y - r, x + r + 1, y + r + 1, getRadian(-30), getRadian(30));
}

void birth() {
	// 动画持续时间
	int period = 2;
	// 每秒帧数
	int frame = 30;
	// 总共需要绘制的帧数
	int total_frame = period * frame;
	int mills_per_frame = (1.0 / frame) * 1000;
	// 总帧数应当小于 PACMAN_RADIUS_MAX，否则增长会为 0
	int radius_increment = PACMAN_RADIUS_MAX / total_frame;

	for (int i = 0; i < total_frame; ++i) {
		setfillcolor(PACMAN_YELLOW);
		solidcircle(upacman.x, upacman.y, upacman.radius);
		upacman.radius += radius_increment;
		Sleep(mills_per_frame);
	}

	mouthstate.left = upacman.x - upacman.radius;
	mouthstate.top = upacman.y - upacman.radius;
	mouthstate.right = upacman.x + upacman.radius + 1;
	mouthstate.bottom = upacman.y + upacman.radius + 1;
}

void setMouth(int state) {
	int period = 1;
	int frame = 30;
	int total_frame = period * frame;
	int mills_per_frame = (1.0 / frame) * 1000;

	double degree_start;
	double degree_end;
	double degree_increment;

	if (state == MOUTH_OPEN) {
		degree_start = 0;
		degree_end = 0;
		degree_increment = (mouthstate.degree_end_2 - 0) / total_frame;
	}
	else if (state == MOUTH_CLOSE) {
		degree_start = mouthstate.degree_start_2;
		degree_end = mouthstate.degree_end_2;
		degree_increment = (0 - mouthstate.degree_end_2) / total_frame;
	}

	for (int i = 0; i < total_frame; ++i) {
		setfillcolor(PACMAN_YELLOW);
		solidcircle(upacman.x, upacman.y, upacman.radius);
		setfillcolor(BLACK);
		solidcircle(upacman.x - upacman.radius / 3, upacman.y - upacman.radius / 2, upacman.radius / 8);
		clearpie(mouthstate.left, mouthstate.top, mouthstate.right, mouthstate.bottom, getRadian(degree_start), getRadian(degree_end));
		degree_start -= degree_increment;
		degree_end += degree_increment;
		Sleep(mills_per_frame);
	}
}

void eat() {
	Sleep(250);
	setMouth(MOUTH_OPEN);
	Sleep(250);
	setMouth(MOUTH_CLOSE);
	Sleep(250);
}

int main()
{
	initgraph(1000, 700);
	init();
	birth();
	while (1) {
		eat();
	}
	_getch();
	return 0;
}
