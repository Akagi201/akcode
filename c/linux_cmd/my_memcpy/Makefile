my_memcpy_test:main.o my_memcpy.o
	gcc -o my_memcpy_test main.o my_memcpy.o
main.o:main.c my_memcpy.h
	gcc -c main.c my_memcpy.h
my_memcpy.o:my_memcpy.c my_memcpy.h
	gcc -c my_memcpy.c my_memcpy.h
.PHONY:clean
clean:
	rm -f my_memcpy_test my_memcpy.o main.o
