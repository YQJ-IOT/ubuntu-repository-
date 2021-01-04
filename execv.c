#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>



typedef struct 
{
	sem_t *sem;
	char sem_name[20];
	int sval;
}File_arg;

int main(int argc, char const *argv[])
{
	printf("%s\n","----execv.c IN----" );
	int ret;

	File_arg file_arg;
	file_arg.sem = (sem_t *)malloc(sizeof(sem_t));
	sprintf(file_arg.sem_name,"sem01");

	file_arg.sem = sem_open(file_arg.sem_name,O_EXCL);
	if (file_arg.sem == SEM_FAILED)
	{
		perror("sem_open error");
	}

	sem_wait(file_arg.sem);
	sem_getvalue(file_arg.sem,&file_arg.sval);
	printf("%s%d\n","file_arg.sval is ",file_arg.sval);

	printf("%s%d%s%s%s%s\n","process pid is ",getpid(),"argv[0] is ",argv[0],"argv[1] is ",argv[1]);

	sem_post(file_arg.sem);
	sem_close(file_arg.sem);




	printf("%s\n","----execv.c OUT----" );
	return 0;
}