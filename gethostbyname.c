#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int i;
	struct hostent *host;
	if(argc!=2){
		printf("Usage: %s <addr>\n", argv[0]);
		exit(1);
	}

	host=gethostbyname(argv[1]);//main함수를 통해서 전달된 문자열을 인자로 gethostbyname함수를 호출하고있다.
	if(!host)
		error_handling("gethost......error");

	printf("Official name: %s \n", host->h_name);//h_name:공식 도메인 이름 저장
	for(i=0; host->h_aliases[i]; i++)
		printf("Aliases %d: %s \n", i+1, host->h_aliases[i]);//하나의 ip에 둘 이상의 도메인 이름 지정 가능
		//188p메모
		printf("Address type: %s \n",(host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");//주소체계 IPv4인 경우 AF_INET 저장
	for(i=0; host->h_addr_list[i]; i++)
		printf("IP addr %d: %s \n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
