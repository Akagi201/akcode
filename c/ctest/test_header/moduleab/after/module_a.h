// module_a.h

#ifndef MODULE_A_H_
#define MODULE_A_H_ 1

struct B_s; // 这里是个不完整定义
struct A_s;
typedef struct A_s A_t;

int func_a(struct B_s * _data);

#endif
