#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>


char webpage[] = "HTTP/1.1 200 OK\r\n"
						"Server:Linux Web Server\r\n"
					  "Content-Type: text/html; charset=UTF-8\r\n\r\n"
					  "<!DOCTYPE html>\r\n"
					  "<html><head><title> My Web Page </title>\r\n"
					  "<style>body {background-color: #FFFF00 }</style></head>\r\n"
					  "<body><center><h1>Hello world!!</h1><br>\r\n"
					  "<img src=\"cat.jpg\"></center></body></html>\r\n";

int main(int argc, char *argv[])
{
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t sin_len = sizeof(clnt_addr);
	int serv_sock, clnt_sock;
	char buf[2048];
	int fdimg;
	char img_buf[700000];

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		perror("bind() error!!");
	if(listen(serv_sock, 5) == -1) perror("listen() error!!");

	while(1) {
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &sin_len);
		puts("New client connection.....");
		/* chile process */
		if(!fork()) {
			close(serv_sock);
			read(clnt_sock, buf, sizeof(buf) - 1);

			if(!strncmp(buf, "GET /cat.jpg", 12)) {
		        fdimg = open("cat.jpg", O_RDONLY);
				read(fdimg, img_buf, sizeof(img_buf));
				write(clnt_sock, img_buf, sizeof(img_buf));
				close(fdimg);
			}
			else
				write(clnt_sock, webpage, sizeof(webpage));
			close(clnt_sock);

			exit(1);
		}
		close(clnt_sock);
	}
}
