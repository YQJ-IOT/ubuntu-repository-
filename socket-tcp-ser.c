#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include<pthread.h>
#include<string.h>

#define CLI 5
typedef struct {
	int sockfd;
}SERVER;

int cli_num; //记录客户端线程数
pthread_mutex_t mutex;

//与客户端01通信进程
void* clifunc1(void *arg)
{
	//接收客户端的socket文件描述符,将指针里面的值取出来
	int sockfd = *(int *)arg;    
	char array[20] = {0};
	//断开与main的联系
	if(0!=pthread_detach(pthread_self()))
	{
		perror("pthread_detach error");
	}
	else
	{
		printf("pthread_detach ok\n");
	}

	while(1)
	{
		//清空接收缓冲
		memset(array,0,20);
		//从客户端的socket文件读取
		if(0 == read(sockfd,array,20))
		{
			break;
			//当没有读到数据退出while(1)
		}
		printf("%s\n",array);
		sleep(2);
	}
	
	printf("sockfd %d\n",sockfd);
	//关闭客户端的socket文件
	if(-1 == close(sockfd))
	{
		perror("close error");
	}
	else
	{
		printf("close sockfd ok\n");
		//客户端线程数-1
	}
	//退出线程
	pthread_mutex_lock(&mutex);
	cli_num--; //进程数-1
	pthread_mutex_unlock(&mutex);

	
	pthread_exit(NULL);
}


//与客户端02通信进程
void* clifunc2(void *arg)
{
	//接收客户端的socket文件描述符,,将指针里面的值取出来
	int sockfd = *(int *)arg;	
	char array[20] = {0};
	//断开与main的联系
	if(0!=pthread_detach(pthread_self()))
	{
		perror("pthread_detach error");
	}
	else
	{
		printf("pthread_detach ok\n");
	}
	while(1)
	{
		//清空接收缓冲
		memset(array,0,20);
		//从客户端的socket文件读取
		if(0 == read(sockfd,array,20))
		{
			break;
			//当没有读到数据退出while(1)
		}
		printf("%s\n",array);
		sleep(2);
	}
	//关闭客户端的socket文件
	printf("sockfd %d\n",sockfd);
	if(-1 == close(sockfd))
	{
		perror("close error");
	}
	else
	{
		printf("close sockfd ok\n");
		//客户端线程数-1
	}
	//退出线程
	pthread_mutex_lock(&mutex);
	cli_num--;//进程数-1
	pthread_mutex_unlock(&mutex);
	
	
	pthread_exit(NULL);
}



void main()
{
	
	SERVER server;
	cli_num = 0;  //进程数 =0
	struct sockaddr_in serdef; //服务端地址信息
	struct sockaddr_in clidef; //客户端地址信息
	pthread_mutex_init(&mutex,NULL); 
	
	pthread_t pthread[CLI];  //创建线程对象数组
	
	//生成服务器端文件
	if(-1==(server.sockfd = socket(AF_INET,SOCK_STREAM,0)))
	{
		perror("socket error");
	}
	else
	{
		printf("socket return %d\n",server.sockfd);
	}
	
	/*设置套接字的文件的属性 */
	int opt = 1;	/* 使能地址重新使用 */
	setsockopt(server.sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//为服务器端文件绑定地址:ipv4+10000端口+本地地址
	//INADDR_ANY转换过来就是0.0.0.0，泛指本机的意思，也就是表示本机的所有IP
	serdef.sin_family = AF_INET;
	serdef.sin_port = htons(10000);
	serdef.sin_addr.s_addr = htonl(INADDR_ANY);
	if(-1 == bind(server.sockfd,(struct sockaddr*)&serdef,sizeof(serdef)))
	{
		perror("bind error");
	}
	else
	{
		printf("bind ok\n");
	}


	//监听最多10个客户端
	if(-1 == listen(server.sockfd,10))
	{
		perror("listen error");
	}
	else
	{
		printf("listen ok\n");
	}
	
	//客户端地址信息:ipv4+任意端口+本地任意地址0.0.0.0
	clidef.sin_family = AF_INET;
	clidef.sin_port = htons(INADDR_ANY);
	clidef.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int sockfd_new = 0;
	socklen_t addrlen = sizeof(clidef);
	
	//重要:创建函数指针数组
	void *(*funcp[])(void *) = {
		clifunc1,
		clifunc2
	};
	
	while(1)
	{
		//接收客户端的connect,并分配一个客户端文件用于读写通信
		if( -1 == (sockfd_new = accept(server.sockfd,(struct sockaddr*)&clidef,&addrlen)))
		{
			perror("accept error");
		}
		else
		{
			printf("sockfd_new is %d\n",sockfd_new);
			//分配线程clifunc1与客户端通信
			if(0!=pthread_create(&pthread[cli_num],NULL,funcp[cli_num],(void*)&sockfd_new))
			{
				perror("pthread_create error\n");
			}
			else
			{
				printf("pthread_create ok\n");
				//进程数+1
				cli_num++;
			}
		}
		/*服务器进程一般不需要关闭,只要确保各个线程关闭就行!
		if(cli_num == 0)
		{
			break;
		}
		*/
	}
	//关闭客户端文件
	if(-1 == close(server.sockfd))
	{
		perror("close error");
	}
	else
	{
		printf("close ok\n");
	}
	//释放互斥锁
	pthread_mutex_destroy(&mutex);
}
