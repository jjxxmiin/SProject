#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define insert_size 4

typedef struct user{
	char id[30];
	char pw[30];
	char email[30];
}user;

int userCount();
void insert(struct user *p, int size);
int login(struct user *p, int size);
int isoverlap(char *id);
void startgame();
void moveFirstPage();
void exitGame();

int main(){
	user use[insert_size];
	int size = sizeof(use)/sizeof(use[0]);

	int select;
	int issuccess_login;
	int start;
	int start_page;
	system("clear");
	while(1){
		printf("=========================================\n");
		printf("===========   1. 회원가입   =============\n");
		printf("=========================================\n");
		printf("===========   2. 로그인     =============\n");
		printf("=========================================\n");
		printf("===========   3. 게임종료   =============\n");
		printf("=========================================\n\n");
		printf("메뉴를 선택해 주세요 :  ");        
		scanf("%d", &select);

		switch(select){
			case 1:
				system("clear");
				insert(use, size);
				break;
			case 2:
				system("clear");
				issuccess_login = login(use, size);
				if(issuccess_login >= 0){
						system("clear");
						printf("======    [%s님 환영합니다.]   ========\n", use[issuccess_login].id);
						printf("=========================================\n");
						printf("=====     1. single play     ============\n");
						printf("=========================================\n");
						printf("=====     2. multi play    ==============\n");
						printf("=========================================\n");
						printf("======    3. 처음 화면으로 이동      ====\n");
						printf("=========================================\n");
						printf("======    4. 게임 종료    ===============\n");
						printf("=========================================\n");
						printf("선택 : ");
						scanf("%d", &start);

						switch(start){
							case 1:
								startgame();
								system("clear");
								while(1){
									char se = '\0';
									printf("========================================\n");
									printf("=========  R :  게임 다시 시작 =========\n");
									printf("=========  F :  처음 화면으로 이동 =====\n");
									printf("========================================\n");
									printf("선택 : ");
									while(getchar() != '\n');
									scanf("%c", &se);
									if(se == 'R' || se == 'r')
										startgame();	
									else{
										moveFirstPage();
										break;
									}
								}
								break;
							case 2:
								system("clear");
								printf("개발 준비중입니다...\n");
								sleep(2);
								break;
							case 3: 
								moveFirstPage();
								break;
							case 4:
								exitGame();
								break;
						}
				}
				else{
					printf("로그인에 실패하셨습니다.\n");
					sleep(2);
					system("clear");
				}
				break;
			case 3:
				exitGame();
				break;
		}
	}
}

int userCount(){
	FILE *fp;

	if((fp = fopen("information.txt", "r")) == NULL)
		return 0;

	int cnt = 0;
	char buf1[30], buf2[30], buf3[30];
	fscanf(fp, "%s%s%s", buf1, buf2, buf3);
	cnt++;
	while(!feof(fp)){
		fscanf(fp, "%s%s%s", buf1, buf2, buf3);
		cnt++;
	}
	return cnt;
}

void insert(user *p, int size){
	int flag = 0;
	char ch = '\0';
	int cnt = userCount();
	if(cnt <= insert_size){
		printf("=========================================\n");
		printf("============   회원 가입   ==============\n");
		printf("=========================================\n\n");

		printf("id 입력 : ");
		scanf("%s", p[cnt].id);
		printf("pw 입력 : ");
		scanf("%s", p[cnt].pw);
		printf("email 입력 : ");
		scanf("%s", p[cnt].email);

		if((flag = isoverlap(p[cnt].id)) == 1){
			printf("이미 존재하는 id입니다.\n");
			printf("다시 기입하시겠습니까? [Y/N] => ");
			while(getchar() != '\n');
			scanf("%c", &ch);
			if(ch == 'Y' || 'y'){
				system("clear");
				insert(p, size);
			}
			else
				moveFirstPage();
		}
		else{
			printf("회원가입 성공!\n");
			FILE *fp = fopen("information.txt", "a");
			fprintf(fp, "%-10s%-10s%-30s\n", p[cnt].id, p[cnt].pw, p[cnt].email);
			fclose(fp);
			cnt++;
			sleep(2);
			system("clear");
		}
	}
	else{
		system("clear");
		printf("가입 수용인원을 넘었습니다.\n");
		printf("처음 메뉴로 돌아갑니다.\n");
		sleep(2);
		system("clear");
	}
	
}
int isoverlap(char *id){
	FILE *fp;
	if((fp = fopen("information.txt", "r")) == NULL){
		printf("당신이 첫 회원입니다!\n");
		return 0;
	}
	char buf1[30], buf2[30], buf3[30];

	fscanf(fp, "%s%s%s", buf1, buf2, buf3);
	if(!strcmp(buf1, id)){
		fclose(fp);
		return 1;
	}
	while(!feof(fp)){
		fscanf(fp, "%s%s%s", buf1, buf2, buf3);
		if(!strcmp(buf1, id)){
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

int login(user *p, int size){
	char my_id[30];
	char my_pw[30];

	FILE *fp;
	fp = fopen("information.txt", "r");

	int cnt = 0;
	fscanf(fp, "%s%s%s", p[cnt].id, p[cnt].pw, p[cnt].email);
	cnt++;
	while(!feof(fp)){
		fscanf(fp, "%s%s%s", p[cnt].id, p[cnt].pw, p[cnt].email);
		cnt++;
	}
	fclose(fp);
	
	printf("=========================================\n");
	printf("============      로그인      ===========\n");
	printf("=========================================\n\n");

	printf("id 입력 : ");
	scanf("%s", my_id);
	printf("pw 입력 : ");
	scanf("%s", my_pw);

	for(int i = 0; i < cnt; i++){
		if(!strcmp(p[i].id, my_id) && !strcmp(p[i].pw, my_pw)){
			return i;
			break;
		}
	}
	return -1;

}

void startgame(){
	system("clear");
	printf("게임을 시작합니다!\n");
	system("clear");
	system("./block_game2");
}

void moveFirstPage(){
	printf("처음 페이지로 이동합니다.\n");
	sleep(2);
	system("clear");
}

void exitGame(){
	printf("게임을 종료합니다.\n");
	sleep(2);
	system("clear");
	exit(1);
}
