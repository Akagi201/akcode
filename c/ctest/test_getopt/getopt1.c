/** @file getopt1.c
 * @brief    echo程序中，对于命令行参数的解析是手动实现的。
 * @brief    -n 参数表示不换行
 *
 * @author   liuboyf1
 * @date     2012-10-11
 * @version  V1.0.0
 *
 * @note ///Description here
 * @note History:
 * @note     <author>   <time>    <version >   <desc>
 * @note
 * @warning
 */

#include <stdio.h>
int main(int argc, char **argv) {
	int i;
	int nflg;
	nflg = 0;
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n') {
	//if (argc > 1 && argv[1] == "-n")
		nflg = 1;
		--argc;
		++argv;
	}
	for (i = 1; i < argc; ++i) {
		fputs(argv[i], stdout);
		if (i < argc - 1) {
			putchar(' ');
		}
	}
	if (nflg == 0) {
		putchar('\n');
	}
	return 0;
}
