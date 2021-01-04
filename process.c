#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

/*
technology used in list
----1,process use
----2,POSIX有名信号量
----3,execl()
----4,system()
*/

typedef struct 
{
	sem_t *sem;
	char  sem_name[20];
	int sval;
}File_arg;

typedef struct {
	pid_t pid;

}Porcess_arg;

int main(int argc, char const *argv[])
{
	printf("%s\n","----process.c IN----" );
	Porcess_arg process_son01;
	File_arg file_arg;
	file_arg.sem = (sem_t *)malloc(sizeof(sem_t));
	sprintf(file_arg.sem_name,"sem01");
	int sonid;

	system("./execv");

	sonid = fork();
	if (sonid >0)
	{
		/*father porcess*/
		file_arg.sem = sem_open(file_arg.sem_name,O_CREAT|O_EXCL,0666,10);
		sem_wait(file_arg.sem);
		sem_getvalue(file_arg.sem,&file_arg.sval);

		printf("%s%d\n","file_arg.sval is ",file_arg.sval);
		printf("%s%d\n","process pid is ",getpid());
		printf("%s%d\n","porcess is exit:",wait(NULL));


		sem_post(file_arg.sem);
		sem_close(file_arg.sem);
		if(-1 == sem_unlink(file_arg.sem_name))
		{
			perror("sem_unlink error");
		}
		printf("%s\n","----process.c OUT----" );
		return 0;
	}
	if (sonid == 0)
	{
		/*son porcess*/
		process_son01.pid = getpid();
		printf("%s%d\n","-->process pid is ",process_son01.pid);

		/*the execl is used batter,the parameter one is the a.out file*/
		if (execl("/home/yqj/文档/2021-1-2/execv","parameter01","parameter02",(char*)NULL) == -1)
		{
			perror("execv error:");
		}
		/*note:there is no need exit,because execv.c had return 0*/
		//exit(0); 
	}
}