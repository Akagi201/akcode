/*
 * @file calltrace.c
 * @author Akagi201
 * @date 2014/12/15
 */

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

// gcc need option -rdynamic

void print_calltrace(void)
{
  void *callstack[128];
  int i = 0;
  int num = 0;

  printf("----CallTrace Begin:\n");

  num = backtrace(callstack, 128);
  char **strs = backtrace_symbols(callstack, num);
  for (i = 0; i < num; i++) {
    printf("----CallTrace %d : %s\n", i, strs[i]);
  }
  free(strs);
}
