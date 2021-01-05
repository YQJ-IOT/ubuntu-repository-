#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ipc.h>
#include <sys/sem.h>

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


//note:由于多进程的输出不容易区分是那个进程的输出，这个在前面加了”2：“来区分
int main(int argc, char const *argv[])
{
	printf("2:%s\n","----systemV-2 main IN----" );
	File_arg file_arg;
	
	//获取key	
	if(-1 == (file_arg.key = ftok("/home/farsight/桌面/text/ubuntu-repository-/systemV-1.c",1))) {perror("ftok error");}
	else{printf("2:ftok return %x\n",file_arg.key);}
	//获取消息队列
	if( -1 == (file_arg.msgid = msgget(file_arg.key,IPC_EXCL))) { perror("msgget error"); }
	else{printf("2:msgget return %d\n",file_arg.msgid);}
	//获取共享内存
	if( -1 == (file_arg.shmid = shmget(file_arg.key,SIZE,IPC_EXCL))) { perror("shmget error"); }
	else{printf("2:shmget return %d\n",file_arg.shmid);}
	//获取信号量数组
	if( -1 == (file_arg.semid = semget(file_arg.key,0,IPC_EXCL))) { perror("msgget error"); }
	else{printf("2:semget return %d\n",file_arg.semid);}

	//共享内存映射
	if((char*)(-1) == (file_arg.shmaddr = (char*)shmat(file_arg.shmid,NULL,0))) {perror("shmat error");}
	//访问共享内存
	printf("2:shm room:%s\n",file_arg.shmaddr);
	
	//接受类型为1的消息，0表示任意消息,最后一个参数0表示阻塞接收
	struct msgbuf son_msgbuf;
	if(-1 == msgrcv(file_arg.msgid,&son_msgbuf,sizeof(son_msgbuf),1,0))
	{
		perror("msgrcv error");
	}
	printf("2:msgrcv return %s\n",son_msgbuf.mtext);

	//信号量数组
	struct sembuf sembuff01_v = {0,-1,SEM_UNDO};//0表示第一个信号量，-1表示等待信号量，SEM_UNDO表示阻塞
	struct sembuf sembuff01_p = {0,1,SEM_UNDO};////0表示第一个信号量，+1表示释放信号量，SEM_UNDO表示阻塞
	if(-1 == semop(file_arg.semid,&sembuff01_v,1)) { perror("semop error"); } 

	//察看第0个信号量的值
	if(-1 == (file_arg.semval = semctl(file_arg.semid,0,GETVAL))) { perror("semctl-GETVAL error");}
	printf("2:file_arg.semval is %d\n",file_arg.semval);
	
	//1表示操作一个信号量
	if(-1 == semop(file_arg.semid,&sembuff01_p,1)) { perror("semop error"); } 
	
	//察看第0个信号量的值
	if(-1 == (file_arg.semval = semctl(file_arg.semid,0,GETVAL))) { perror("semctl-GETVAL error");}
	printf("2:file_arg.semval is %d\n",file_arg.semval);

	
	//断开共享内存映射
	if(-1 == shmdt(file_arg.shmaddr)) { perror("shmtd error"); }	
	exit(0);
}
