#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

#define BC 50

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

typedef struct _BLOCK {
	int nLife;
	int nX,nY;
}BLOCK;

typedef struct _USER {
	int point;
	int win;
}USER;

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
int g_StateTable[4][6] = {
        {3,2,-1,-1,-1,4},
        {-1,5,4,-1,-1,-1},
        {-1,-1,1,0,5,-1},
        {-1,-1,-1,-1,2,1}
};

int g_BlockState[6] = {3,2,1,0,5,4};
int g_BlockCount = BC;
int g_over = 0;

BAR g_Bar;
BALL g_Ball;
BLOCK g_Block[BC];
USER user;

void init() {
	user.point = 0;
	user.win = 0;

	g_Bar.nY = 30;
	g_Bar.nX[0] = 30;
	g_Bar.nX[1] = 34;
	g_Bar.nX[2] = 38;
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

int Search(int End,int nX,int nY) { //블록이 있는지 검사
	int i;
	for(i=0;i<End;i++) {
		if(g_Block[i].nY == nY) {
			if(g_Block[i].nX == nX || (g_Block[i].nX + 1) == nX || g_Block[i].nX == nX + 1 || (g_Block[i].nX + 1) == nX + 1) return 1; //상하좌우
		}
	}
	return 0;
}

void SetBlock(int BlockCount) { //블록 셋팅
	int nX,nY,i;

	srand((unsigned int)time(NULL));
	for(i=0;i<BlockCount;i++) {
		g_Block[i].nLife = 1;

		while(1) {
			nX = rand() % 79;
			nY = rand() % 11;

			if(Search(i,nX,nY)==0) {
				g_Block[i].nX = nX;
				g_Block[i].nY = nY;
				break;
			}
		}
	}
}
void save() {
	FILE *fd;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
        fd  = fopen("score.txt","a");
        fprintf(fd,"%d-%d-%d %d:%d:%d =  %d point",tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec,user.point);
        fclose(fd);
}

void gameover() {
	save();
	clear();
	
	mvprintw(0,0,"=======================================");
	mvprintw(1,0,"==============Game over================");
	mvprintw(2,0,"==============score : %d===============",user.point);
	mvprintw(3,0,"=======================================");
	
	refresh();
}

int Collision(int nX,int nY) { //충돌
	int i,count = 0;

	//블록 충돌
	for(i = 0; i<g_BlockCount ;i++) {
		if(g_Block[i].nLife == 1) {
			if(g_Block[i].nY == nY) {
				if(g_Block[i].nX == nX || (g_Block[i].nX + 1) == nX || g_Block[i].nX == nX + 1 || (g_Block[i].nX + 1) == nX + 1) {
					g_Ball.nDirect = g_BlockState[g_Ball.nDirect];
					g_Block[i].nLife = 0;
					user.point++;
					count++;
				}
			}
		}
	}

	if(count != 0) return 1;

	//막대기 충돌
	for(i = 0; i<3;i++) {
		if(nY == g_Bar.nY) {
			if(nX >= g_Bar.nX[0] && nX < g_Bar.nX[1] || (nX + 1) >= g_Bar.nX[0] && (nX + 1) < g_Bar.nX[1]) {
				g_Ball.nDirect = RIGHT_TOP;
			}else if(nX >= g_Bar.nX[1] && nX < g_Bar.nX[2] || (nX + 1) >= g_Bar.nX[1] && (nX + 1) < g_Bar.nX[2]) {
				g_Ball.nDirect = g_BlockState[g_Ball.nDirect];
			}else if(nX >= g_Bar.nX[2] && nX <= g_Bar.nX[2] + 3 || (nX + 1) >= g_Bar.nX[2] && (nX + 1) <= g_Bar.nX[2] + 3) {
				g_Ball.nDirect = LEFT_TOP;
			}
			else {
				g_over = 1;
			}
			return 1;
		}
	}

	//테두리 충돌
	if(nY < 0) {
		g_Ball.nDirect = g_StateTable[0][g_Ball.nDirect];
		return 1;
	}
	if(nX > 78) {
		g_Ball.nDirect = g_StateTable[1][g_Ball.nDirect];
		return 1;
	}
	if(nY > 31) {
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
			int e = 1;

			switch(g_Ball.nDirect){
				case TOP :
					if(Collision(g_Ball.nX,g_Ball.nY-e) == 0) {
						g_Ball.nY--;
					}
					break;

				case LEFT_TOP :
					if(Collision(g_Ball.nX+e,g_Ball.nY-e) == 0) {
						g_Ball.nX++;
						g_Ball.nY--;
					}
					break;
				case LEFT_DOWN :
					if(Collision(g_Ball.nX+e,g_Ball.nY+e) == 0) {
						g_Ball.nX++;
						g_Ball.nY++;
					}
					break;
				case DOWN :
					if(Collision(g_Ball.nX,g_Ball.nY+e) == 0) {
						g_Ball.nY++;
					}
					break;
				case RIGHT_DOWN :
					if(Collision(g_Ball.nX-e,g_Ball.nY+e) == 0) {
						g_Ball.nX--;
						g_Ball.nY++;
					}
					break;
				case RIGHT_TOP :
					if(Collision(g_Ball.nX-e,g_Ball.nY-e) == 0) {
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
		mvprintw(g_Bar.nY,g_Bar.nX[i],"====");
	}
	for(i=0;i<BC;i++) {
		if(g_Block[i].nLife == 1) {
			mvprintw(g_Block[i].nY,g_Block[i].nX,"O");
		}
	}
	mvprintw(g_Ball.nY,g_Ball.nX,"@");
}

int main(int argc,char* argv[]) {
	int key,nDirect;
	clock_t CurTime;
	pid_t pid;

	init();
	SetBlock(BC);


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
		if(g_over == 1) break; //게임 오버
		UpDate();
		usleep(150000);
		Render();
		refresh();

	}
	gameover(); //게임 오버 메세지
	sleep(2);
	getchar(); //아무키 입력시 종료
	endwin();
	return 0;
}
