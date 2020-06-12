#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if(argc!=3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	/*서버 접속을 위한 소켓 생성*/
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);//문자열로 표현된 IP주소를 32비트 정수형으로 변환
	serv_addr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error!");
	//connect(클라이언트 소켓, 연결요청 할 서버의 주소정보, 두 번째 매개변수에 전달된 주소의 변수 크기

	str_len=read(sock, message, sizeof(message)-1);
	//read(데이터 수신대상, 수신한 데이터를 저장할 버퍼의 주소 값, 수신할 최대 바이트 수)
	if(str_len==-1)
		error_handling("read() error!");

	printf("Message from server : %s \n", message);
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
