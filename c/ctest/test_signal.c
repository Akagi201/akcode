
/*
 * @file test_signal.c
 *
 * @author Akagi201
 * @date 2014/05/07
 *
 * signal()的用法
 * 捕捉ctrl + c发送的SIGINT与ctrl + \发送的SIGQUIT信号
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void my_func(int sign_no) {
	if (SIGINT == sign_no) {
		printf("I have got SIGINT\n");
	} else if (SIGQUIT == sign_no) {
		printf("I have got SIGQUIT\n");
	}

	return;
}

int main(void) {
	printf("waiting for signal SIGINT or SIGQUIT ...\n");
	signal(SIGINT, my_func);
	signal(SIGQUIT, my_func);
	pause();
	return 0;
}
