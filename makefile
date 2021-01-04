OBJ : execl process pthread

execl:execl.c 
	gcc -g -Wall execl.c -o execl -l pthread
process:process.c 
	gcc -g -Wall process.c -o process -l pthread
pthread:pthread.c 
	gcc -g -Wall pthread.c -o pthread -l pthread


.phony:clean
clean:
	rm -rf process pthread execl