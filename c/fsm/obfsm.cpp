
/*
 * @file obfsm.c
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
  将自动机封装为一个对象, 隐藏自动机实现的细节.
  即设计模式: 状态模式
 *
 */

#include <stdio.h>

/*
 * @brief 有限状态机
 */
class state_machine {
	enum states {
		before = 0,
		inside = 1,
		after = 2
	} state;
	
	struct branch {
		enum states new_state:2;
		int should_putchar:1;
	};
	
	static struct branch the_table[3][3];
	
public:
	state_machine(): state(before) {
		
	}
	
	void feed_char(int c) {
		int idx2 = (c == ' ') ? 0 : (c == '\n') ? 1 : 2;
		struct branch *b = &the_table[state][idx2];
		state = b->new_state;
		if (b->should_putchar) {
			putchar(c);
		}
	}
};

struct state_machine::branch state_machine::the_table[3][3] = {
					 /* ' '        '\n'         others */
		/* before */ {{before, 0}, {before, 1}, {inside, 1}},
		/* inside */ {{after, 0},  {before, 1}, {inside, 1}},
		/* after */  {{after, 0},  {before, 1}, {after, 0}}
};

int main(void) {
	int c = 0;
	state_machine machine;
	
	while ((c = getchar()) != EOF) {
		machine.feed_char(c);
	}

	return 0;
}





















