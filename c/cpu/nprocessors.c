/*
 * @file nprocessors.c
 * @author Akagi201
 * @date 2014/12/23
 * _SC_NPROCESSORS_CONF: The number of processors configured.
 * _SC_NPROCESSORS_ONLN: The number of processors currently online (available).
 */

#include <stdio.h>
#include <unistd.h>

int main(void) {
  int cpu_num = 0;

  cpu_num = sysconf(_SC_NPROCESSORS_CONF);
  printf("_SC_NPROCESSORS_CONF: %d\n", cpu_num);

  cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
  printf("_SC_NPROCESSORS_ONLN: %d\n", cpu_num);
  return 0;
}
