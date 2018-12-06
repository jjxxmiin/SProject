#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>


typedef enum _DIRECT {
	TOP, 
	LEFT_TOP, 
	LEFT_DOWN, 
	DOWN, 
	RIGHT_DOWN, 
	RIGHT_TOP
}DIRECT;

typedef struct _BAR {
	int nX[3];
	int nY;
	clock_t OldTime;
	clock_t MoveTime;
}BAR;

typedef struct _BALL {	//공의 속성
	int nReady;	//준비상태
	int nHP;	//생명
	int nX,nY;	//좌표
	DIRECT nDirect;	//이동 방향
	clock_t MoveTime;	//이동 시간 간격
	clock_t OldTime;	//이전 이동 시간
}BALL;

BAR g_Bar;
BALL g_Ball;

void init() {
	g_Bar.nY = 13;
	g_Bar.nX[0] = 30;
	g_Bar.nX[1] = 32;
	g_Bar.nX[2] = 34;
	g_Bar.MoveTime = 80;
	g_Bar.OldTime = clock();

	g_Ball.nX = g_Bar.nX[1];
	g_Ball.nY = g_Bar.nY - 1;
	g_Ball.nReady = 1;
	g_Ball.nDirect = TOP;
	g_Ball.OldTime = clock();
	g_Ball.MoveTime = 100;

	initscr();
	noecho();
	keypad(stdscr,TRUE);
	curs_set(FALSE);
}

void UpDate() {
	clock_t CurTime = clock();

	if(g_Ball.nReady == 0) {
		if(CurTime - g_Ball.OldTime > g_Ball.MoveTime) {
			g_Ball = CurTime;

			switch(g_Ball.nDirect){
				case TOP :
				case LEFT_TOP :
				case LEFT_DOWN :
				case DOWN :
				case RIGHT_DOWN :
				case RIGHT_TOP :	
			}

			if(g_Ball.nX < 0 || g_Ball)
		}
	}
}

int main(int argc,char* argv[]) {
	int key;


	while(1) {
		key = getch();
		switch(key) {
			case 's':

		}
	}
