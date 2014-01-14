// #和##的应用特例

#include <stdio.h>
// 合并匿名变量名

#define ___ANONYMOUS1(type, var, line) type var##line
#define __ANONYMOUS0(type, line) ___ANOYMOUS1(type, _anonymous, line)
#define ANONYMOUS(type) __ANONYMOUS0(type, __LINE__)

// 例子 ANONYMOUS(static int); -> static int _anonymous70; 70表示该行行号
// 第一层: ANONYMOUS(static int); -> __ANONYMOUS0(static int, __LINE__);
// 第二层:                        -> ___ANONYMOUS1(static int, _anonymous, 70);
// 第三层:                        -> static int _anonymous70;
// 即每次只能解开当前层的宏，所以__LINE__在第二层才能被解开

// 填充结构
#define FILL(a) {a, #a}

enum IDD{OPEN, CLOSE};
typedef struct MSG
{
    IDD id;
    const char *msg;
}MSG_T;

MSG_T _msg[] = {FILL(OPEN), FILL(CLOSE)};
// 相当于下面
MSG_T _msg[] = {{OPEN, "OPEN"}, {CLOSE, "CLOSE"}};

// 记录文件名
#define _GET_FILE_NAME(f) #f
#define GET_FILE_NAME(f) _GET_FILE_NAME(f)
static char FILE_NAME[] = GET_FILE_NAME(__FILE__);

// 得到一个数值类型所对应的字符串缓冲大小
#define _TYPE_BUF_SIZE(type) sizeof #type
#define TYPE_BUF_SIZE(type) _TYPE_BUF_SIZE(type)

/*
char buf[TYPE_BUF_SIZE(INT_MAX)];
    --> char buf[_TYPE_BUF_SIZE(0x7fffffff)];
    --> char buf[sizeof "0x7fffffff"];
这里相当于:
char buf[11];
*/
