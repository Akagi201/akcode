client: rw.o client.o
	gcc -lpthread -Wall rw.o  client.o -o client
client.o: rw.h upgrade_file.h 
rw.o: rw.h 
clean:
	-rm client *.o
.PHONY: clean


