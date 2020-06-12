#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

char webpage[]="HTTP/1.1 200 OK\r\n"
     "Server:Linux Web Server\r\n"
     "Content-Type:text/html; charset=UTF-8\r\n\r\n"
     "<!DOCTYPE html>\r\n"
     "<html><head><title> YYJ </title>\r\n"
     "<style>body {background-color: #FFFF00 }</style></head>\r\n"
     "<body><center><h1>Yeji world~!</h1><br>\r\n"
     "<img src=\"cat.jpg\"></center></body></html>\r\n";
	 //if(strcmp(buf,"GET /game.jpg", 13);
	 //open("game.jpg",O_RDONLY);

#define BUF_SIZE 8000
void error_handling(char *message);
void read_childproc(int sig);


int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz = sizeof(clnt_adr);
	int str_len, state;
	char img_buf[70000];
	int fdimg;
	char buf[BUF_SIZE];

	if(argc!=2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	/*소켓생성*/
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	while(1)
	{
		//adr_sz = sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock == -1) continue;
		puts("new client connected..");
		if(!fork()){
			close(serv_sock);
			//memset(buf, 0, BUF_SIZE);
			read(clnt_sock, buf, sizeof(buf)-1);
			printf("%s\n",buf);
			if(!strncmp(buf, "GET /cat.jpg", 12)){
				fdimg=open("cat.jpg", O_RDONLY);
				printf("!!!!!!!!!!!!!!!!!!!!!");
				//int len =
				read(fdimg, img_buf, sizeof(img_buf));
				//printf("%d dfceeefefer\n", len);
				//if(len == -1) printf("errorororororo\n");
				write(clnt_sock, img_buf, sizeof(img_buf));
				close(fdimg);
			}
			else
				write(clnt_sock, webpage, sizeof(webpage));
			close(clnt_sock);
			puts("client disconnected..");
			exit(1);
		}
		
		
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
	
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
