#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define SIZE 1000
#define NSEMS 10

typedef struct 
{
	key_t key;
	int msgid;
	int semid;
	int semval;
	int shmid;
	char* shmaddr;
}File_arg;


typedef struct {
	pid_t pid;
}Porcess_arg;

struct msgbuf {
         long mtype;       /* message type, must be > 0 */
         char mtext[100];    /* message data */
};


//note：所以printf（“\n”），必须带\n,并且放在右边，防止stdin与stdout缓冲区不能输出！
int main(int argc, char const *argv[])
{
	printf("1:%s\n","----systemV-1 main IN----"); //note: \n放在后面--触发fflush
	Porcess_arg process_arg;
	File_arg file_arg;
	
	//获取key值
	if(-1 == (file_arg.key = ftok("/home/farsight/桌面/text/ubuntu-repository-/systemV-1.c",1)))
   	{
		perror("ftok error");
	}
	else
	{
		printf("1:ftok return %x\n",file_arg.key);
	}
	
	//创建消息队列
	if( -1 == (file_arg.msgid = msgget(file_arg.key,IPC_CREAT))) 
	{
	   	perror("msgget error"); 
	}
	else
	{
		printf("1:msgget return %d\n",file_arg.msgid);
	}
	
	//创建共享内存
	if( -1 == (file_arg.shmid = shmget(file_arg.key,SIZE,IPC_CREAT))) 
	{ 
		perror("msgget error"); 
	}
	else
	{
		printf("1:shmget return %d\n",file_arg.shmid);
	}

	//创建信号量数组,含有NSEMS个信号量
	if( -1 == (file_arg.semid = semget(file_arg.key,NSEMS,IPC_CREAT))) 
	{ 
		perror("msgget error"); 
	}
	else
	{
		printf("1:semget return %d\n",file_arg.semid);
	}
	
	//共享内存映射，权限0777，NULL自动映射地址
	if((char*)(-1) == (file_arg.shmaddr = (char*)shmat(file_arg.shmid,NULL,0777))) 
	{
		perror("shmat error");
	}
	
	
	
	//创建进程
	process_arg.pid = fork();
	if (process_arg.pid >0 )
	{
		//进入父进程
		printf("1:----父进程 IN----\n");
		
		//向共享内存写入
		sprintf(file_arg.shmaddr,"0123456789");
		printf("1:----0123456789 is sprintf in shm\n");
		//断开内存映射	
		if(-1 == shmdt(file_arg.shmaddr)) { perror("shmtd error"); }	
	
		//发送消息，消息操作权限0777
		struct msgbuf father_msgbuf = {1,"0123456789"};
		if(-1 == msgsnd(file_arg.msgid,&father_msgbuf,sizeof(father_msgbuf),0777))
		{
			perror("msgsnd error");
		}

		//设置信号量数组的第一个信号量的值为8；注意：第一个信号量都是从0开始计数
		if(-1 == semctl(file_arg.semid,0,SETVAL,8)) { perror("semctl-SETVAL error");}
		
		//获取信号量数组的第一个信号量的值；
		if(-1 == (file_arg.semval = semctl(file_arg.semid,0,GETVAL))) { perror("semctl-SETVAL error");}
		else {printf("1:file_arg.semval is %d\n",file_arg.semval);}
		
		struct sembuf sembuff01_v = {0,-1,SEM_UNDO};//0表示第1个信号量，-1表示等待信号量，SEM_UNDO表示阻塞
		struct sembuf sembuff01_p = {0,1,SEM_UNDO};//0表示第1个信号量，+1表示释放信号量，SEM_UNDO表示阻塞
		
		//1表示操作1个信号量
		if(-1 == semop(file_arg.semid,&sembuff01_v,1)) { perror("semop error"); } 
		
		 //获取信号量数组的第一个信号量的值；
		if(-1 == (file_arg.semval = semctl(file_arg.semid,0,GETVAL))) { perror("semctl-SETVAL error");}
		else {printf("1:file_arg.semval is %d\n",file_arg.semval);}
		
		//1表示操作1个信号量
		if(-1 == semop(file_arg.semid,&sembuff01_p,1)) { perror("semop error"); } 

		 //获取信号量数组的第一个信号量的值；
		if(-1 == (file_arg.semval = semctl(file_arg.semid,0,GETVAL))) { perror("semctl-SETVAL error");}
		else{printf("1:file_arg.semval is %d\n",file_arg.semval);}

		//等待子进程
		if(-1 == wait(NULL)){ perror("wait error");}
		else{ printf("1:the son is exit\n"); }

		//释放system-v的共享内存，信号量数组，消息队列
		if(-1 == shmctl(file_arg.shmid,IPC_RMID,NULL)) { perror("shmtcl rmid error");}
		if(-1 == semctl(file_arg.semid,1,IPC_RMID) ) { perror("semctl rmid error");}
		if(-1 == msgctl(file_arg.msgid,IPC_RMID,NULL)) {perror("msgctl rmid error");}
		return 0;
	}
	//子进程
	if (process_arg.pid == 0)
	{
		//进入子进程
		printf("1:%s\n","----子进程 IN----" );
		if(-1 == execl("/home/farsight/桌面/text/ubuntu-repository-/systemV-2",NULL,(char*)NULL)) {printf("1:execl error");}
		printf("1:%s\n","----子进程 OUT----" );
	}
}
