#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	//bind함수에 주소정보 전달하는 용도
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[]="Hello World!";

	if(argc!=2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	/*서버소켓(리스닝 소켓)생성*/
	serv_sock = socket(PF_INET, SOCK_STREAM, 0); //소켓생성 socket(IPv4,TCP,)
	if(serv_sock == -1)
		error_handling("socket() error");

	/*주소정보 초기화*/
	memset(&serv_addr, 0, sizeof(serv_addr));
	//구조체변수 serv_addr의 모든  멤버 0으로 초기화
	serv_addr.sin_family = AF_INET;
	//.주소체계=IPv4인터넷프로토콜에 적용하는 주소체계
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	//.32비트 IP주소.32비트 IPv4인터넷주소 =host to network long.short(소켓이 동작하는 컴퓨터의 IP주소가 자동 할당);//문자열 기반의 IP주소 초기화
	serv_addr.sin_port=htons(atoi(argv[1]));
	//.16비트 TCP/UDP PORT번호=?(atoi:문자열로 표현되어 있는 값을 정수로 변환해서 반환(serv_port));//문자열 기반의 PORT번호 초기화

	/*소켓에 인터넷 주소정보 할당*/
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error");
	//bind(주소정보(클라이언트:IP,서버:PORT)를 할당할 소켓,할당하고자 하는 주소정보를 지니는 구조체 변수의 주소 값, 두 번째 인자로 전달된 구조체 변수의 길이정보)

	/*연결요청 대기상태로의 진입*/
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	//listen(연결요청 대기상태에 두고자 하는 소켓,연결요청 대기 큐의 크기정보 전달:클라이언트의 연결요청을 5개까지 대기 가능.)

	/*클라이언트의 연결요청 수락*/
	clnt_addr_size=sizeof(clnt_addr);
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	//accept(서버 소켓, 연결요청 한 클라이언트의 주소정보 담을 변수 주소 값 전달, 두 번째 매개변수에 전달된 주소의 변수 크기를 바이트 단위로 전달)

	if(clnt_sock==-1)
		error_handling("accept() error");

	write(clnt_sock, message, sizeof(message));
	//write(데이터 전송대상, 전송할 데이터가 저장된 버퍼의 주소 값, 전송할 데이터의 바이트 수)

	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
