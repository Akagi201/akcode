
/*
 * @file test_sigaction.c
 *
 * @author Akagi201
 * @date 2014/05/07
 *
 * sigaction()的用法
 * 同时捕捉ctrl + c发送的SIGINT与ctrl + \发送的SIGQUIT信号
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void my_func(int sign_no) {
	if (SIGINT == sign_no) {
		printf("I have got SIGINT\n");
	} else if (SIGQUIT == sign_no) {
		printf("I have got SIGQUIT\n");
	}
	
	return;
}

int main(void) {
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	
	printf("waiting for signal SIGINT or SIGQUIT ...\n");
	action.sa_handler = my_func;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, 0);
	sigaction(SIGQUIT, &action, 0);
	pause();

	return 0;
}

