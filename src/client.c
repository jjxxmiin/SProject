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

	pid = fork();

	if(pid == -1) {
		printf("[Client] fork error\n");
		exit(1);
	}
	if(pid == 0) {
		if(execl("./Menu", "Menu", NULL) == -1) {
			printf("[Client] execl error\n");
		}
	}
	else {
		wait(&status);
	}

	close(sockfd);

	return 0;
}
