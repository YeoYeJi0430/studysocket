#include <stdio.h>
int main(int argc,char *argv[])
{
	int i = 0;
	printf("인자 수 : %d\n",argc);
	printf("프로그램경로 : %s\n",argv[0]);
	for(i=1;i<argc;i++){
		printf("추가된 인자 : %s\n",argv[i]);
	}
	return 0; //운영체제에 보내줌
}
