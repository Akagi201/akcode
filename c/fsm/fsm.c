
/*
 * @file fsm.c
 *
 * @author Akagi201
 * @date 2014/05/19
 *
 * 考虑一个C语言的程序，由标准输入流一行一行的读取数据，
 打印各一行的第一个英文单字。因此一开始需确认第一个英文单字之前是否有空白，
 若有，需读取所有空白后略过不打印，读取第一个英文单字然后打印，
 之后读取其他内容略过不打印，直到读到换行符号为止。任何情形下只要读到换行符号，
 就重新开始此算法，任何情形下只要读到文件退出（end-of-file）的符号，就退出程序。
 有限状态机解法
 *
 */

/* 为每一个状态写独立的处理程序
#include <stdio.h>

int main(void) {
	enum states {
		before, inside, after
	} state;
	
	int c = 0;
	
	state = before;
	
	while ((c = getchar()) != EOF) {
		switch(state) {
		case before:
			if (c == '\n') {
				putchar('\n');
			} else {
				if (c != ' ') {
					putchar(c);
					state = inside;
				}
			}
			break;
		case inside:
			switch(c) {
			case ' ':
				state = after;
				break;
			case '\n':
				putchar('\n');
				state = before;
				break;
			default:
				putchar(c);
			}
			break;
		case after:
			if (c == '\n') {
				putchar('\n');
				state = before;
			}
		}
	}
	
	return 0;
}
*/

/* 自动机步骤的程序区块都只使用区域变量
#include <stdio.h>

int main(void) {
	enum states {
		before, inside, after
	} state;
	
	int c = 0;
	
	state = before;
	
	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			putchar('\n');
			state = before;
		} else {
			switch (state) {
			case before:
				if (c != ' ') {
					putchar(c);
					state = inside;
				}
				break;
				
			case inside:
				if (c == ' ') {
					state = after;
				} else {
					putchar(c);
				}
				break;
				
			case after:
				break;
			}
		}
	}
	
	return 0;
}
*/

// 将自动机步骤集成为一个独立的函数step()

/*
#include <stdio.h>

enum states {
	before, inside, after
};

void step(enum states *state, int c) {
	if (c == '\n') {
		putchar('\n');
		*state = before;
	} else {
		switch (*state) {
		case before:
			if (c != ' ') {
				putchar(c);
				*state = inside;
			}
			break;
			
		case inside:
			if (c == ' ') {
				*state = after;
			} else {
				putchar(c);
			}
			break;
			
		case after:
			break;
		}
	}
	
	return;
}

int main(void) {
	int c = 0;
	enum states state = before;
	
	while ((c = getchar()) != EOF) {
		step(&state, c);
	}
	
	return 0;
}
*/

// 显示状态转换表

#include <stdio.h>

enum states {
	before = 0,
	inside = 1,
	after = 2
};

struct branch {
	unsigned char new_state:2; // 下一个状态状态
	unsigned char should_putchar:1; // 是否打印
};

// 状态转换表
struct branch the_table[3][3] = {
					/* ' '         '\n'         others */
		/* before */ {{before, 0}, {before, 1}, {inside, 1}},
		/* inside */ {{after, 0},  {before, 1}, {inside, 1}},
		/* after */  {{after, 0},  {before, 1}, {after, 0}}
};

void step(enum states *state, int c) {
	int idx2 = (c == ' ') ? 0 : (c == '\n') ? 1 : 2; // 根据输入, 转换成对应的索引
	struct branch *b = &the_table[*state][idx2]; // 得到转换后的状态和处理方式(即是否打印a)
	*state = (enum states)(b->new_state);
	if (b->should_putchar) {
		putchar(c);
	}
	
	return;
}

int main(void) {
	int c = 0;
	enum states state = before;
	
	while ((c = getchar()) != EOF) {
		step(&state, c);
	}
	
	return 0;
}

/*
 * 制造周期由2个程序组成:
 * 1. 根据输入数据决定状态的程序.
 * 2. 根据目前状态输出对应数据的程序.
 */




















