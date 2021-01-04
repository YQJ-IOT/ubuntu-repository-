#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <semaphore.h>

/*
technology used in list
----1,thread use
----2,POSIX无名信号量
----3,mutex of the thread

*/

#define CODE2

typedef struct 
{
	pthread_t thread;
	
}Pthread_arg;


pthread_mutex_t mutex;

sem_t *sem; 
int i = 0;

/*note : pthread_function01 == pthread_function02 == pthread_function03*/
static void* pthread_function01(void *arg)
{
	int sem_sval;
	printf("-->%s->进程ID：%d ->内核线程ID：%ld ->用户态线程ID：%ld\n"\
		,__FUNCTION__,getpid(),syscall(SYS_gettid),pthread_self());
	while(1)
	{
		sleep(5);
		printf("-->%s in\n",__FUNCTION__);
		if(-1 == sem_wait(sem)) { perror("sem_wait error");}

		if(-1 == sem_getvalue(sem,&sem_sval)) { perror("sem_getvalue error");}
		else{printf("%s%d\n","sem_getvalue is ", sem_sval);}
		pthread_mutex_lock(&mutex);
		printf("-->i == %d\n",++i);
		pthread_mutex_unlock(&mutex);

		if(-1 == sem_post(sem)) { perror("sem_post error");}
	}
	pthread_exit(NULL);
}


static void* pthread_function02(void *arg)
{
int sem_sval;
	printf("-->%s->进程ID：%d ->内核线程ID：%ld ->用户态线程ID：%ld\n"\
		,__FUNCTION__,getpid(),syscall(SYS_gettid),pthread_self());
	while(1)
	{
		sleep(5);
		printf("-->%s in\n",__FUNCTION__);
		if(-1 == sem_wait(sem)) { perror("sem_wait error");}

		if(-1 == sem_getvalue(sem,&sem_sval)) { perror("sem_getvalue error");}
		else{printf("%s%d\n","sem_getvalue is ", sem_sval);}
		pthread_mutex_lock(&mutex);
		printf("-->i == %d\n",++i);
		pthread_mutex_unlock(&mutex);

		if(-1 == sem_post(sem)) { perror("sem_post error");}
		
	}
	pthread_exit(NULL);
}

static void* pthread_function03(void *arg)
{
	int sem_sval;
	printf("-->%s->进程ID：%d ->内核线程ID：%ld ->用户态线程ID：%ld\n"\
		,__FUNCTION__,getpid(),syscall(SYS_gettid),pthread_self());
	while(1)
	{
		sleep(5);
		printf("-->%s in\n",__FUNCTION__);
		if(-1 == sem_wait(sem)) { perror("sem_wait error");}

		if(-1 == sem_getvalue(sem,&sem_sval)) { perror("sem_getvalue error");}
		else{printf("%s%d\n","sem_getvalue is ", sem_sval);}
		pthread_mutex_lock(&mutex);
		printf("-->i == %d\n",++i);
		pthread_mutex_unlock(&mutex);

		if(-1 == sem_post(sem)) { perror("sem_post error");}
		
	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{

		int	ret; 	//function return
		Pthread_arg pthread_arg[3];
		sem = (sem_t*)malloc(sizeof(sem_t));

		if(-1 == sem_init(sem,0,10))	{perror("sem_init error");}


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
		
		if(-1 == sem_destroy(sem))	{perror("sem_destroy error");}
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

		if(-1 == sem_destroy(sem))	{perror("sem_destroy error");}
		while(1); 
		return 0;
#endif



}