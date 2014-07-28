/** @file getopt4.c
 * @brief
 *
 * @author   liuboyf1
 * @date     2012-10-12
 * @version  V1.0.0
 *
 * @note ///Description here
 * @note History:
 * @note     <author>   <time>    <version >   <desc>
 * @note
 * @warning
 */
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int oc; /*选项字符 */
	char ec; /*无效的选项字符*/
	char *b_opt_arg; /*选项参数字串 */
	while ((oc = getopt(argc, argv, ":ngl:")) != -1) {
		switch (oc) {
		case 'n':
			printf("new.\n");
			break;
		case 'g':
			printf("great.\n");
			break;
		case 'l':
			b_opt_arg = optarg;
			printf("l is %s\n", b_opt_arg);
			break;
		case '?':
			ec = (char) optopt;
			printf("无效的选项字符 \'%c\'\n", ec);
			break;
		case ':':
			printf("缺少选项参数！\n");
			break;
		}
	}
	return 0;
}

