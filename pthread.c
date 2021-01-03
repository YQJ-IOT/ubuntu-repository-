#include<stdio.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */


#define CODE2

typedef struct NODE{
	pthread_t thread;	

}Pthread_arg;


pthread_mutex_t mutex;
int i = 0;

static void* pthread_function01(void *arg)
{
	printf("-->%s->进程ID：%d ->内核线程ID：%ld ->用户态线程ID：%ld\n"\
		,__FUNCTION__,getpid(),syscall(SYS_gettid),pthread_self());
	while(1)
	{
		sleep(5);
		printf("-->%s in\n",__FUNCTION__);
		pthread_mutex_lock(&mutex);
		printf("-->i == %d\n",++i);
		pthread_mutex_unlock(&mutex);

	}
	pthread_exit(NULL);
	
}


static void* pthread_function02(void *arg)
{
printf("-->%s->进程ID：%d ->内核线程ID：%ld ->用户态线程ID：%ld\n"\
		,__FUNCTION__,getpid(),syscall(SYS_gettid),pthread_self());
	while(1)
	{
		sleep(5);
		printf("-->%s in\n",__FUNCTION__);
		pthread_mutex_lock(&mutex);
		printf("-->i == %d\n",++i);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

static void* pthread_function03(void *arg)
{
	printf("-->%s->进程ID：%d ->内核线程ID：%ld ->用户态线程ID：%ld\n"\
		,__FUNCTION__,getpid(),syscall(SYS_gettid),pthread_self());
	while(1)
	{
		sleep(5);
		printf("-->%s in\n",__FUNCTION__);
		pthread_mutex_lock(&mutex);
		printf("-->i == %d\n",++i);
		pthread_mutex_unlock(&mutex);

	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
		int	ret; 	//function return
		Pthread_arg pthread_arg[3];
		
		printf("-->%s->进程ID：%d ->内核线程ID：%ld ->用户态线程ID：%ld\n" \
			,__FUNCTION__,getpid(),syscall(SYS_gettid),pthread_self());

		ret = pthread_mutex_init(&mutex,NULL);
			if(ret != 0)	perror("-->pthread_create error:");


		ret = pthread_create(&pthread_arg[0].thread,NULL,pthread_function01,NULL);
			if(ret != 0)	perror("-->pthread_create error:");
		ret = pthread_create(&pthread_arg[1].thread,NULL,pthread_function02,NULL);
			if(ret != 0)	perror("-->pthread_create error:");
		ret = pthread_create(&pthread_arg[2].thread,NULL,pthread_function03,NULL);
			if(ret != 0)	perror("-->pthread_create error:");


#ifdef CODE1
		printf("-->%s\n","CODE1 IN" );
		ret = pthread_join(pthread_arg[0].thread,NULL);
			if(ret != 0)	perror("-->pthread_join error:");

		ret = pthread_join(pthread_arg[1].thread,NULL);
			if(ret != 0)	perror("-->pthread_join error:");

		ret = pthread_join(pthread_arg[2].thread,NULL);
			if(ret != 0)	perror("-->pthread_join error:");
		return 0;
#endif


#ifdef CODE2
		printf("-->%s\n","CODE2 IN" );
		ret = pthread_detach(pthread_arg[0].thread);
			if(ret != 0)	perror("-->pthread_join error:");
		ret = pthread_detach(pthread_arg[1].thread);
			if(ret != 0)	perror("-->pthread_join error:");
		ret = pthread_detach(pthread_arg[2].thread);
			if(ret != 0)	perror("-->pthread_join error:");
		while(1); 
		return 0;
#endif
}