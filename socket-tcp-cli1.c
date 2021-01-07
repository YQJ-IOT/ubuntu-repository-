#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

typedef struct {
	int sockfd;
}CLIENT;

void main()
{
	CLIENT client;
	struct sockaddr_in serdef;  //服务器地址信息
	struct sockaddr_in clidef;  //客户端地址信息
	
	//创建客户端socket文件
	if(-1==(client.sockfd = socket(AF_INET,SOCK_STREAM,0)))
	{
		perror("socket error");
	}
	else
	{
		printf("socket return %d\n",client.sockfd);
	}
	
	/*设置套接字的文件的属性*/
	int opt = 1;	/* 使能地址重新使用 */
	setsockopt(client.sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//绑定客户端地址信息:ipv4+10001端口+本地任意ip地址
	clidef.sin_family = AF_INET;
	clidef.sin_port = htons(10001);
	clidef.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//绑定客户端地址信息:ipv4+10000端口+本地任意ip地址
	serdef.sin_family = AF_INET;
	serdef.sin_port = htons(10000);
	serdef.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//客户端socket文件绑定客户端地址信息
	if(-1 == bind(client.sockfd,(struct sockaddr*)&clidef,sizeof(clidef)))
	{
		perror("bind error");
	}
	else
	{
		printf("bind ok\n");
	}

	
	//访问服务器地址
	if(-1 == connect(client.sockfd,(struct sockaddr*)&serdef,sizeof(serdef)))
	{
		perror("accept error");
	}
	else
	{
		printf("connect ok\n");
	}
	
	char array[20];
	
	//通信
	while(1)
	{
		//键盘获取想要传输的数据
		scanf("%s",array);
		
		//键盘输入end表示客户端停止,客户端将关闭
		if(strcmp(array,"end") == 0){
			break;
		}
		write(client.sockfd,array,20);

		sleep(2);		
	}
	
	//关闭客户端socket文件
	if(-1 == close(client.sockfd))
	{
		perror("close error");
	}
	else
	{
		printf("close sockfd ok\n");
	}
}
