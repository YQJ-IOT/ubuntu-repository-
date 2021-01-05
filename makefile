OBJ: execl process pthread systemV-1 systemV-2

execl:execl.c 
	gcc -g  execl.c -o execl -l pthread
process:process.c 
	gcc -g  process.c -o process -l pthread
pthread:pthread.c 
	gcc -g  pthread.c -o pthread -l pthread
systemV-1:systemV-1.c
	gcc  systemV-1.c -o systemV-1 -g
systemV-2:systemV-2.c
	gcc  systemV-2.c -o systemV-2 -g


.phony:clean
clean:
	rm -rf process pthread execl systemV-1	systemV-2
