#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	printf("%s\n", "-->execv.c IN");
	printf("%s%d\n","process pid is ",getpid());
	printf("%s%s\n","argv[0] is ",argv[0]);
	printf("%s%s\n","argv[1] is ",argv[1]);
	return 0;
}