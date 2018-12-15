#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct user{
	char id[30];
	char pw[30];
	char email[30];
}user;

void insert(struct user *p, int size);
int login(struct user *p, int size);


static int cnt = 0;

int main(){
	user use[4];
	int size = sizeof(use)/sizeof(use[0]);

	int select;
	int issuccess_login;
	int start;
	while(1){
		printf("1. 회원가입\n");
		printf("2. 로그인\n");
		printf("3. 게임종료\n");
		scanf("%d", &select);
		
		switch(select){
			case 1:
				insert(use, size);
				break;
			case 2:
				issuccess_login = login(use, size);
				if(issuccess_login <= size){
					printf("%s님 환영합니다.\n\n", use[issuccess_login].id);

					//while(1){
						printf("1. single play\n");
						printf("2. multi play\n");
						printf("3. 게임 종료\n");
						scanf("%d", &start);

						switch(start){
							case 1:
								printf("게임 시작 !\n");
								system("./block_game2");
								break;
							case 2:
								printf("멀티게임 시작!\n");
								break;
							case 3:
								exit(1);
						}
					//}
				}
				else
					printf("로그인에 실패하셨습니다.\n");
				break;
			case 3:
				exit(1);
		}
	}
}

void insert(user *p, int size){
	if(cnt < size){
		printf("id 입력 : ");
		scanf("%s", p[cnt].id);
		printf("pw 입력 : ");
		scanf("%s", p[cnt].pw);
		printf("email 입력 : ");
		scanf("%s", p[cnt].email);

		cnt++;
	}
	else{
		printf("가입 수용인원을 넘었습니다.\n");
	}
}

int login(user *p, int size){
	char my_id[30];
	char my_pw[30];

	printf("id 입력 : ");
	scanf("%s", my_id);
	printf("pw 입력 : ");
	scanf("%s", my_pw);

	for(int i = 0; i < size; i++){
		if(!strcmp(p[i].id, my_id) && !strcmp(p[i].pw, my_pw)){
			return i;
			break;
		}
	}
	return size+1;

}





