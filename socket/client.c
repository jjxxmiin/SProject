#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
/*
struct clientInfo {
	char name[20];
	char msg[BUFSIZE];
};
*/
int main(int argc, char *argv[]) {
	int sockfd;
	int len, n;
	struct sockaddr_in address;
	fd_set readfds, testfds;
	int nfds;

	pid_t pid;
	//struct clientInfo info;
	char name[20], msg[BUFSIZE];
	char snd_msg[BUFSIZE], rcv_msg[BUFSIZE];

	if(argc != 2) {
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

	strcpy(name, argv[1]);
	//strcpy(info.name, argv[1]);
	//strcpy(info.msg, "");
	//write(sockfd, (struct clientInfo *)&info, sizeof(info));

/*	
	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);
	FD_SET(0, &readfds);

	nfds = sockfd;

	while(1) {
		testfds = readfds;

		if(select(nfds+1, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0) {
			printf("select error\n");
			exit(1);
		}

		if(FD_ISSET(sockfd, &testfds)) {
			n = read(sockfd, rcv_msg, BUFSIZE);
			if(n == 0) {
				printf("서버와 접속이 끊겼습니다.\n");
				break;
			}
			printf("[Client] rcv_msg: %s\n", rcv_msg);
		}
		else if(FD_ISSET(0, &testfds)) {
			printf("전송할 메시지를 입력하세요 : ");
			fgets(snd_msg, sizeof(snd_msg), stdin);
			snd_msg[strlen(snd_msg)-1] = '\0';
			sprintf(msg, "[%s] : %s", name, snd_msg);
			write(sockfd, msg, sizeof(msg));
		}
	}
*/	

	pid = fork();

	if(pid == -1) {
		printf("fork error\n");
		exit(1);
	}

	if(pid == 0) {
		while(1) {
			printf("전송할 메시지를 입력하세요 : ");
			fgets(snd_msg, sizeof(snd_msg), stdin);
			snd_msg[strlen(snd_msg)-1] = '\0';
			sprintf(msg, "[%s] : %s", name, snd_msg);
			write(sockfd, msg, sizeof(msg));
		}
	}
	else {
		while(1) {
			n = read(sockfd, rcv_msg, BUFSIZE);
			if(n == 0) {
				printf("[Client] 서버와 접속이 끊겼습니다.\n");
				break;
			}
			printf("%s\n", rcv_msg);
		}
	}

	close(sockfd);

	return 0;
}
