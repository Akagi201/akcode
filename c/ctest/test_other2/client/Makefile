CC = gcc
CFLAGS = -Wall -o2

obj2 = client.o client_heart.o client_recv_file.o client_check_file.o

TARGET = client
CLIENT = client

all: $(CLIENT) 
client:
$(CLIENT): $(obj2)
	$(CC) $(CFLAGS) $(obj2) -o $(CLIENT)
	$(RM)  *.o

.PATHY : clean
clean:
	$(RM) $(TARGET) *.o
