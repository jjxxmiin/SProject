#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>

int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

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
	g_Bar.nX[0] = 30; //바의 위치
	g_Bar.nX[1] = 32;
	g_Bar.nX[2] = 34;
	g_Bar.MoveTime = 80;
	g_Bar.OldTime = clock();

	g_Ball.nX = g_Bar.nX[1];
	g_Ball.nY = g_Bar.nY - 1;
	g_Ball.nReady = 1; //공의 위치
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
			g_Ball.OldTime = CurTime;

			switch(g_Ball.nDirect){
				case TOP :
					g_Ball.nY--;
					break;
				case LEFT_TOP :
					g_Ball.nX++;
					g_Ball.nY--;
					break;
				case LEFT_DOWN :
					g_Ball.nX++;
					g_Ball.nY++;
					break;
				case DOWN :
					//g_Ball.nX++;
					g_Ball.nY++;
					break;
				case RIGHT_DOWN :
					g_Ball.nX--;
					g_Ball.nY++;
					break;
				case RIGHT_TOP :
					g_Ball.nX--;
					g_Ball.nY--;
					break;	
			}

		}
		if(g_Ball.nX < 0 || g_Ball.nX > 78 || g_Ball.nY > 24 || g_Ball.nY < 0){
			g_Ball.nReady = 1;
			g_Ball.nX = g_Bar.nX[1];
			g_Ball.nY = g_Bar.nY-1;
			g_Ball.nDirect = TOP;
			printf("asdasd");
		}
		else {
			g_Ball.nX = g_Bar.nX[1];
			g_Ball.nY = g_Bar.nY-1;
		}

	}
}
void Render() {
	int i;
	clear();
	mvprintw(g_Ball.nY,g_Ball.nX,"o");
	for(i=0;i<3;i++) {
		mvprintw(g_Bar.nY,g_Bar.nX[i],"==");
	}
	//clear();
}

int main(int argc,char* argv[]) {
	int key,nDirect;
	clock_t CurTime;

	init();
	while(1) {
		if(kbhit()) { //키를 입력 받을때
			key = getch();
			switch(key) {
				case 'k':
					g_Ball.nReady = 0;
					g_Ball.OldTime = clock();
					break;
				case 'j':
					CurTime = clock();
					if(CurTime - g_Bar.OldTime > g_Bar.MoveTime) {
						g_Bar.OldTime = CurTime;
						if(g_Bar.nX[0] > 0) {
							g_Bar.nX[0]--;
							g_Bar.nX[1]--;
							g_Bar.nX[2]--;
						}
					}
					break;
				case 'l':
					CurTime = clock();
					if(CurTime - g_Bar.OldTime > g_Bar.MoveTime) {
						g_Bar.OldTime = CurTime;
						if(g_Bar.nX[2] <= 77) {
							g_Bar.nX[0]++;
							g_Bar.nX[1]++;
							g_Bar.nX[2]++;
						}
					}
					break;
				case '5':
					nDirect = key - '0';
					g_Ball.nReady =1;
					g_Ball.nX=g_Bar.nX[1];
					g_Ball.nY=g_Bar.nY-1;
					g_Ball.nDirect = nDirect;
					g_Ball.OldTime = clock();
					break;
			}
		}
		UpDate();
		Render();
	}
	return 0;
}
