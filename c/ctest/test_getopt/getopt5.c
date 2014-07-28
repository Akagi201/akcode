
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int aflag = 0;
	int bflag = 0;
	int cflag = 0;
	int ch = 0;
	
	while ((ch = getopt(argc, argv, "ab:c")) != -1) {
		printf("optind: %d\n", optind);
		
		switch (ch) {
		case 'a':
			printf("HAVE option: -a\n");
			aflag = 1;
			break;
		case 'b':
			printf("HAVE option: -b\n");
			bflag = 1;
			printf("The argument of -b is %s\n", optarg);
			break;
		case 'c':
			printf("HAVE option: -c\n");
			cflag = 1;
			break;
		case '?':
			printf("Unknown option: %c\n", (char)optopt); // optopt 保存不能识别的选项
			break;
		}
	}
	
	printf("optopt: %c\n", (char)optopt);
	
	return 0;
}
