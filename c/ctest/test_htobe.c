
/*
 * @file test_htobe.c
 *
 * @author Akagi201
 * @date 2014/05/12
 *
 *
 *
 */

#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	union {
		uint32_t u32;
		uint8_t arr[4];
	}x;

	x.arr[0] = 0x11; // lowest-address byte
	x.arr[1] = 0x22;
	x.arr[2] = 0x33;
	x.arr[3] = 0x44; // highest-address byte

	printf("x.u32 = 0x%x\n", x.u32);
	printf("htole32(x.u32) = 0x%x\n", htole32(x.u32));
	printf("htobe32(x.u32) = 0x%x\n", htobe32(x.u32));

	return 0;
}


