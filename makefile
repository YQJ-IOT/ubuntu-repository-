OBJ: execl process pthread systemV-1 systemV-2 io-multiplex socket-tcp-ser socket-tcp-cli1 socket-tcp-cli2

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
io-multiplex:io-multiplex.c
	gcc io-multiplex.c -o io-multiplex -g -l pthread
socket-tcp-ser:socket-tcp-ser.c
	gcc socket-tcp-ser.c -o socket-tcp-ser -l pthread -g

socket-tcp-cli1:socket-tcp-cli1.c
	gcc socket-tcp-cli1.c -o socket-tcp-cli1 -l pthread -g

socket-tcp-cli2:socket-tcp-cli2.c
	gcc socket-tcp-cli2.c -o socket-tcp-cli2 -l pthread -g
.phony:clean
clean:
	rm -rf process pthread execl systemV-1	systemV-2 io-multiplex socket-tcp-ser socket-tcp-cli1 socket-tcp-cli2
