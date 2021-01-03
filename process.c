#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
	pid_t pid;

}Porcess_arg;

int main(int argc, char const *argv[])
{
	Porcess_arg process_son01;
	int sonid;

	system("./execv");

	sonid = fork();
	if (sonid >0)
	{
		/*father porcess*/
		printf("%s%d\n","-->process pid is ",getpid());
		printf("%s%d\n","-->porcess is exit:",wait(NULL));
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