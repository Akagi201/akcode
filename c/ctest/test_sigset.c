
/*
 * @file test_sigset.c
 *
 * @author Akagi201
 * @date 2014/05/07
 *
 * 信号集
 * 默认对信号进行状态阻塞,此时输入任何信号都不执行,ctrl+c 与ctrl+\都不会被执行,
 * 但是当输入任意字符并回车后,原来的信号就会被立即执行.可以先输入任意字符解除SIG_BLOCK状态,
 * 然后执行SIG_INT与SIG_QUIT,SIG_INT调用我们自己的函数my_func,所以程序会一直不退出,
 * 但是SIG_QUIT仍旧是系统的函数,因此可以正常表达
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void my_func(int signum) {
	printf("If you want to quit, please try SIGQUIT\n");
}

int main(void) {
	sigset_t set;
	sigset_t pendset;
	struct sigaction action1;
	struct sigaction action2;
	
	if (sigemptyset(&set) < 0) {
		perror("sigemptyset");
		exit(1);
	}
	
	if (sigaddset(&set, SIGQUIT) < 0) {
		perror("sigaddset");
		exit(1);
	}
	
	if (sigaddset(&set, SIGQUIT) < 0) {
		perror("sigaddset");
		exit(1);
	}
	
	if (sigismember(&set, SIGINT)) {
		sigemptyset(&action1.sa_mask);
		action1.sa_handler = my_func;
		action1.sa_flags = 0;
		sigaction(SIGINT, &action1, 0);
	}
	
	if (sigismember(&set, SIGQUIT)) {
		sigemptyset(&action2.sa_mask);
		action2.sa_handler = SIG_DFL;
		action2.sa_flags = 0;
		sigaction(SIGQUIT, &action2, 0);
	}

	if (sigprocmask(SIG_BLOCK, &set, NULL) < 0) {
		perror("sigprocmask");
		exit(1);
	} else {
		printf("Signal set was blocked, Press any key!\n");
		getchar();
	}

	if (sigprocmask(SIG_UNBLOCK, &set, NULL) < 0) {
		perror("sigprocmask");
		exit(1);
	} else {
		printf("Signal set is unblock state\n");
	}

	while (1) {
		;
	}

	return 0;
}

