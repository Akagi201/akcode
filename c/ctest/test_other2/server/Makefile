CC = gcc
CFLAGS = -Wall -o2

obj1 = server.o server_heart.o server_send_file.o

TARGET = server
SERVER = server

all: $(SERVER)

server: 
$(SERVER): $(obj1)
	$(CC) $(CFLAGS) $^ -o $@
	$(RM)  *.o

.PATHY : clean
clean:
	$(RM) $(TARGET) *.o
