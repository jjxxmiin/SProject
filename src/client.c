#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
	int sockfd, nfds;
	int len, n;
	struct sockaddr_in address;
	fd_set readfds, testfds;
	int number;
	pid_t pid, status;
	char name[20], msg[BUFSIZE];
	char snd_msg[BUFSIZE], rcv_msg[BUFSIZE];

	if(argc != 1) {
		printf("argc error\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(9734);
	len = sizeof(address);

	if(connect(sockfd, (struct sockaddr *)&address, len) == -1) {
		perror("connect error\n");
		exit(1);
	} // 서버의 명명된 소켓과 연결 시도

	printf("==================================================\n");
	printf("벽돌깨기 게임!!!\n");
	printf("1.참여\t2.종료\n");
	printf("==================================================\n");
	
	printf("닉네임을 입력하세요 : ");
	fgets(name, sizeof(name), stdin);
	name[strlen(name)-1] = '\0';
	write(sockfd, name, sizeof(name));

	while(1) {
		printf("선택하세요 : ");
        	scanf("%d", &number);

		switch(number) {
			case 1:
				printf("Game Start\n");

				pid = fork();

				if(pid == -1) {
					printf("[Client] fork error\n");
					exit(1);
				}
				if(pid == 0) {
					if(execl("/home/kim/git/SProject/src/game/block_game2", "./block_game2", NULL) == -1) {
						printf("[Client] execl error\n");
					}
				}
				else {
					wait(&status);
					sleep(1);
					printf("Game End\n");
				}

				break;
			case 2:
				printf("종료합니다.\n");
				close(sockfd);
				exit(1);
				break;
			default :
				break;
		}
	}
/*
	while(1) {
		if(pid == -1) {
			printf("[Client] fork error\n");
			exit(1);
		}

		if(pid == 0) {
		//while(1) {
			//printf("전송할 메시지를 입력하세요 : ");
			//fgets(snd_msg, sizeof(snd_msg), stdin);
			//snd_msg[strlen(snd_msg)-1] = '\0';
			//write(sockfd, snd_msg, sizeof(snd_msg));
		//}
		}
		else {
		//while(1) {
			//n = read(sockfd, rcv_msg, BUFSIZE);
			//if(n == 0) {
			//	printf("서버와 접속이 끊겼습니다.\n");
			//	break;
			//}
			//printf("%s\n", rcv_msg);
		//}
		}
	}
*/
	close(sockfd);

	return 0;
}
