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
int g_StateTable[4][6] = {
	{3,2,-1,-1,-1,4},
	{-1,5,4,-1,-1,-1},
	{-1,-1,1,0,5,-1},
	{-1,-1,-1,-1,2,1}
};

void init() {
	g_Bar.nY = 13;
	g_Bar.nX[0] = 30;
	g_Bar.nX[1] = 32;
	g_Bar.nX[2] = 34;
	g_Bar.MoveTime = 80;
	g_Bar.OldTime = clock();

	g_Ball.nX = g_Bar.nX[1];
	g_Ball.nY = g_Bar.nY-1;
	g_Ball.nReady = 1; 
	g_Ball.nDirect = TOP;
	g_Ball.OldTime = clock();
	g_Ball.MoveTime = 100;

	initscr();
	noecho();
	keypad(stdscr,TRUE);
	curs_set(FALSE);
}

int Collision(int nX,int nY) {
	if(nY < 0) {
		g_Ball.nDirect = g_StateTable[0][g_Ball.nDirect];
		return 1;
	}
	if(nX > 78) {
		g_Ball.nDirect = g_StateTable[1][g_Ball.nDirect];
		return 1;
	}
	if(nY > 24) {
		g_Ball.nDirect = g_StateTable[2][g_Ball.nDirect];
		return 1;
	}
	if(nX < 0) {
		g_Ball.nDirect = g_StateTable[3][g_Ball.nDirect];
		return 1;
	}
	return 0;
}

void UpDate() {
	clock_t CurTime = clock();
	if(g_Ball.nReady == 0) {
		if(CurTime - g_Ball.OldTime > g_Ball.MoveTime) {
			g_Ball.OldTime = CurTime;

			switch(g_Ball.nDirect){
				case TOP :
					if(Collision(g_Ball.nX,g_Ball.nY-1) == 0) {
						g_Ball.nY--;
					}
					break;

				case LEFT_TOP :
					if(Collision(g_Ball.nX+1,g_Ball.nY-1) == 0) {
						g_Ball.nX++;
						g_Ball.nY--;
					}
					break;
				case LEFT_DOWN :
					if(Collision(g_Ball.nX+1,g_Ball.nY+1) == 0) {
						g_Ball.nX++;
						g_Ball.nY++;
					}
					break;
				case DOWN :
					if(Collision(g_Ball.nX,g_Ball.nY+1) == 0) {
						g_Ball.nY++;
					}
					break;
				case RIGHT_DOWN :
					if(Collision(g_Ball.nX-1,g_Ball.nY+1) == 0) {
						g_Ball.nX--;
						g_Ball.nY++;
					}
					break;
				case RIGHT_TOP :
					if(Collision(g_Ball.nX-1,g_Ball.nY-1) == 0) {
						g_Ball.nX--;
						g_Ball.nY--;
					}
					break;	
			}

		}
	}
}
void Render() {
	int i;
	//clear();
	for(i=0;i<3;i++) {
		mvprintw(g_Bar.nY,g_Bar.nX[i],"==");
	}
	mvprintw(g_Ball.nY,g_Ball.nX,"o");
}

int main(int argc,char* argv[]) {
	int key,nDirect;
	clock_t CurTime;
	pid_t pid;

	init();

	while(1) {
		clear();
		if(kbhit()) { //키를 입력 받을때
			key = getchar();
			switch(key) {
				case 'k':
					g_Ball.nReady = 0;
					break;
				case 'l':
					CurTime = clock();
					if(CurTime - g_Bar.OldTime > g_Bar.MoveTime) {
						g_Bar.OldTime = CurTime;
						if(g_Bar.nX[0] > 0) {
							g_Bar.nX[0]++;
							g_Bar.nX[1]++;
							g_Bar.nX[2]++;
						}
					}
					break;
				case 'j':
					CurTime = clock();
					if(CurTime - g_Bar.OldTime > g_Bar.MoveTime) {
						g_Bar.OldTime = CurTime;
						if(g_Bar.nX[0] < 77) {
							g_Bar.nX[0]--;
							g_Bar.nX[1]--;
							g_Bar.nX[2]--;
						}
					}
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':	
				case '5':
					nDirect = key - '0';
					g_Ball.nReady =1;
					g_Ball.nX=g_Bar.nX[1];
					g_Ball.nY=g_Bar.nY - 1;
					g_Ball.nDirect = nDirect;
					g_Ball.OldTime = clock();
					break;
				default:
					break;
			}
		}
		UpDate();
		usleep(100000);
		Render();
		refresh();
	}
	return 0;
}
