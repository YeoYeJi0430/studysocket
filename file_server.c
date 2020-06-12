#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sd, clnt_sd;
	FILE * fp; //FILE 파일타입-->RAM에 저장(하드디스크) ex>RAM책상:FILE책
			   //파일자료형:파일오픈(하드디스크에서 RAM으로 올라옴 STREAM구조체형태로)
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc!=2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);//1:정상적인종료 운영체제->리턴
				//:비정상적인 종료
	}

	fp=fopen("file_server.c","rb");//read, binary<>text:원본변형가능성있음(저장시binary형태이기때문)
	serv_sd=socket(PF_INET, SOCK_STREAM, 0);//(인터넷프로토콜(PF=AF),TCP,)
											//만들어진 소켓은 운영체제가 관리

	memset(&serv_adr, 0, sizeof(serv_adr));//구조체 초기화
	serv_adr.sin_family=AF_INET;//주소체계
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);//호스트를 네트워크로.
	serv_adr.sin_port=htons(atoi(argv[1]));

	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));//주소할당
	listen(serv_sd, 5);//리스닝소켓(문지기)만들어짐.

	clnt_adr_sz=sizeof(clnt_adr);
	clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	//accecp():클라이언트와 데이터를 주고받을수있는 소켓생성

	while(1)
	{
		read_cnt=fread((void*)buf, 1, BUF_SIZE, fp);
		//fileread:위쪽 버퍼자료형char:여기선 void(어떤자료형이든참조가능)
		//(,1바이트 읽어옴,버퍼사이즈만큼,):1*버퍼사이즈=30
		//리턴 : 읽어온바이트수
		if(read_cnt<BUF_SIZE)
		{
			write(clnt_sd, buf, read_cnt);//write()
			break//WILEㅁㅜㄴ ㅃㅏㅈㅕㄴㅏㅇㅗㅁ 
		}
		write(clnt_sd, buf, BUF_SIZE);
	}
	shutdown(clnt_sd, SHUT_WR);
	read(clnt_sd, buf, BUF_SIZE);
	printf("Message from client: %s \n", buf);

	fclose(fp);
	close(clnt_sd);
	close(serv_sd);
	return 0;
}
//입:0 출:1 에:2

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
