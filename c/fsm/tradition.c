
/*
 * @file tradition.c
 *
 * @author Akagi201
 * @date 2014/05/19
 *
 * 考虑一个C语言的程序，由标准输入流一行一行的读取数据，
 打印各一行的第一个英文单字。因此一开始需确认第一个英文单字之前是否有空白，
 若有，需读取所有空白后略过不打印，读取第一个英文单字然后打印，
 之后读取其他内容略过不打印，直到读到换行符号为止。任何情形下只要读到换行符号，
 就重新开始此算法，任何情形下只要读到文件退出（end-of-file）的符号，就退出程序。
 传统解法
 *
 */

#include <stdio.h>

int main(void) {
	int c = 0;
	do {
		c = getchar();
		while (c == ' ') {
			c = getchar();
		}

		while ((c != EOF) && (c != ' ') && (c != '\n')) {
			putchar(c);
			c = getchar();
		}

		putchar('\n');

		while (c != EOF && c != '\n') {
			c = getchar();
		}
	} while (c != EOF);

	return 0;
}


