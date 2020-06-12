#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void * read(void * arg);
void * accu(void * arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char *argv[])
{
	pthread_t id_t1, id_t2;
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);
	//(,임계영역 두군데에서 쓸 때 0,초기값(1은키가있음))

	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	return 0;
}

void * read(void * arg)
{
	int i;
	for(i=0; i<5; i++)
	{
		fputs("Input num: ",stdout);//(,표준출력:모니터-콘솔창)stdin(표준입력:키보드)

		sem_wait(&sem_two);//two를 1감소->two의 초기값이 1이었음->키가 사라짐
		scanf("%d", &num);//two가 키를 가지고 있을때 실행가능
		sem_post(&sem_one);//one 1증가->키 획득.one에 대한 키 생성
	}
	return NULL;
}

void * accu(void * arg)
{
	int sum=0, i;
	for(i=0; i<5; i++)
	{
		sem_wait(&sem_one);
		sum+=num;
		sem_post(&sem_two);
	}
	printf("Result: %d \n", sum);
	return NULL;
}
