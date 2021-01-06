#include<stdio.h>
#include<stdlib.h>
#include <poll.h>
#include<string.h>
#include<pthread.h>
/* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

//#define POLL
#define SELECT


void* func_select(void *arg)
{
	printf("%s:\n",__FUNCTION__);
	
	//等待时间结构体
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
								
	int ret;
	char array[20];
	memset(array,0,20*sizeof(char));

	//创建可读文件描述符的集合
	fd_set readfds;
	//初始化清空可读文件描述符的集合
	FD_ZERO(&readfds);
	//将0 = stdin 添加到可读文件描述符集合
	FD_SET(0,&readfds);

	//select监听，参数1是最多监听的个数，返回值就是文件描述符
	if(-1 == (ret = select(1024,&readfds,NULL,NULL,&tv)))
	{
		printf("select error\n");
	}
	else
	{
		//返回的文件描述符如果没在监听的集合中，表示该文件描述符退出了监听
		if(FD_ISSET(ret,&readfds) == 0)
		{
			//从退出监听的文件描述符的缓冲中读取数据
			read(ret,array,20);
			printf("%s:%s\n",__FUNCTION__,array);
		}
	}
}


void* func_poll(void *arg)
{
	printf("%s:\n",__FUNCTION__);

	//创建监听的poll对象
	struct pollfd pfd[1];
	pfd[0].fd = 0; //监听的文件描述符
	pfd[0].events = POLLIN;//监听POLLIN输入

	char array[20];
	memset(array,0,20);
	int ret;
	int temp;
	while(1)
	{
		//监听一个poll对象，监听时间为-1，表示一直监听
		if(-1 == (ret =poll(pfd,1,-1)))
		{
			perror("poll error");
		}
		else
		{
			if(pfd[0].revents == POLLIN) //如果这个返回的参数与event的值一样，说明，这个描述符监听退出
			{

				//获取监听的文件描述符号的缓冲区数据
				read(0,array,20*sizeof(int));
				printf("%s\n",array);
			}
		}

	}	
}
void main()
{	
	int ret;
	pthread_t thread[3];
	
	int array[10] = {1,2,3,4,5,6,7,8,9,0};
#ifdef POLL
	if(0!=pthread_create(&thread[0],NULL,func_poll,NULL))
	{
		perror("main:pthread_create 0 error\n");
	}
	if(0!=pthread_detach(thread[0]))
	{
		perror("main:pthread_detach 0 error\n");
	}

#endif

#ifdef SELECT
	if(0!=pthread_create(&thread[1],NULL,func_select,array))
	{
		perror("main:pthread_create 1 error\n");
	}
	if(0!=pthread_detach(thread[1]))
	{
		perror("main:pthread_detach 2 error\n");
	}
#endif

	while(1);
}
