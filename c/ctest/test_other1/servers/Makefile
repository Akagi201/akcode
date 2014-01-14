main: main.o map.o rw.o ringbuffer.o server.o
	gcc -lpthread -Wall main.o map.o rw.o ringbuffer.o server.o -o main
server.o: server.h 
ringbuffer.o : ringbuffer.h
rw.o: rw.h 
map.o: map.h 
main.o: server.h rw.h 
clean:
	-rm main *.o
.PHONY: clean


