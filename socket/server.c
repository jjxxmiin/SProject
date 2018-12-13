#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#define BUFSIZE 1024
/*
struct clientInfo {
	char name[20];
	char msg[BUFSIZE];
};
*/
int server_sockfd;

static void sigint_handler(int signo) {
	printf("Prepping to exit...\n");
	close(server_sockfd);
	fflush(stdout);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
	int client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int nfds, fd;
	fd_set readfds, testfds;

	//struct clientInfo info;
	int cnt = 0, num = 0, idx = 0;
	int client_list[10];
	char client_user[10][20];
	char snd_msg[BUFSIZE], rcv_msg[BUFSIZE], tmp[BUFSIZE];

	if(signal(SIGINT, sigint_handler) == SIG_ERR) {
		fprintf(stderr, "Cannot handle SIGINT!!!\n");
		exit(EXIT_SUCCESS);
	}

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);
	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr *)&server_address, server_len); // 소켓 이름 부여

	listen(server_sockfd, 5); // 대기열 설정하고 연결 요청 대기

	printf("[Server] server waiting\n");

	signal(SIGCHLD, SIG_IGN);

	FD_ZERO(&readfds);
	FD_SET(server_sockfd, &readfds);

	nfds = server_sockfd;

	while(1) {
		int nread;

		testfds = readfds;

		if(select(nfds+1, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 1) {
			perror("server 5");
			close(server_sockfd);
			exit(EXIT_FAILURE);
		}

		for(fd = 0; fd < nfds+1; fd++) {
			if(FD_ISSET(fd, &testfds)) {
				if(fd == server_sockfd) {
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len); // 클라이언트의 연결 요청 수락
					FD_SET(client_sockfd, &readfds);
					printf("[Server] 새로운 클라이언트 %d번 파일 디스크립터 접속\n", client_sockfd);
					client_list[cnt] = client_sockfd;
					cnt++;
					nfds++;

					if(client_sockfd > nfds)
						nfds = client_sockfd;
				}
				else {
					nread = read(fd, rcv_msg, sizeof(rcv_msg));

					while(1) {
						client_user[num][idx] = rcv_msg[idx+1];
						idx++;
						if(rcv_msg[idx+1] == ']')
							break;
					}
					client_user[num][idx] = '\0';
					idx = 0;
					//printf("[Server] %s 님이 접속 되었습니다.\n", client_user[num]);

					if(nread < 0) {
						printf("[Server] read error\n");
						exit(1);
					}
					else if(nread == 0) {
						close(fd);
						FD_CLR(fd, &readfds);
						if(nfds == fd) {
							nfds--;
							cnt--;
							num--;
						}
						printf("[Server] 클라이언트 %d번 파일 디스크립터 해제\n", fd);
					}
					else {
						//sprintf(tmp, "[%s] : %s", client_user[num], rcv_msg);
						for(int i = 0; i < cnt; i++) {
							if(client_list[i] == client_sockfd)
								continue;
							write(client_list[i], rcv_msg, sizeof(rcv_msg));
						}
					}
				}
			}
		}
	}

	return 0;
}
