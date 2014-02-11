
// char * Óë char[] Çø±ð

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(void)
{
	char *c1 = "abc";
	char c2[] = "abc";
	char *c3 = (char *)malloc(3);

	c3 = "abc";

	printf("c1: &c1 = %d, c1 = %d, c1 = %s.\n", &c1, c1, c1);
	printf("c2: &c2 = %d, c2 = %d, c2 = %s.\n", &c2, c2, c2);
	printf("c3: &c3 = %d, c3 = %d, c3 = %s.\n", &c3, c3, c3);
	
	return 0;
}

