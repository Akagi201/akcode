
// 一直想做个宏开关，来控制打印信息，于是总结了整个文档

#include <stdio.h>
#include <time.h>

#define DEBUG   // 在文件中的宏开关,也可以在gcc 后面加-DDEBUG

// 方法一, 不明
#ifdef DEBUG
#define DBG_PRINT(flag) printf flag
#else
#define DBG_PRINT(flag)
#endif

// 方法二，不定参函数
#ifdef DEBUG
#include <stdarg.h>
void debug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}
#else
void debug(const char *fmt, ...)
{
}
#endif

// 方法3，使用注释的方法屏蔽 
#ifdef DEBUG
#define DBG_PRINT1 printf 
#else
#define DBG_PRINT1 //DBG_PRINT1
#endif

// 方法4，通过空白
#ifdef DEBUG
#define DBG(CODE) CODE
#else
#define DBG(CODE)
#endif

int main()
{
    printf("正常0: %ds\n", time(NULL));
    DBG_PRINT(("方法1: %ds\n", time(NULL)));
    debug("方法2: %ds\n", time(NULL));
    DBG_PRINT1("方法3: %ds\n", time(NULL));
    DBG(printf("方法4: %ds\n", time(NULL)););
    return 0;
}
