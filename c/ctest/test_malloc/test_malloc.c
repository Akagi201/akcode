
// 标准3部曲: malloc + free + 指针置空
/*
 * malloc 使用规范探讨
 * in参数: 申请的堆内存字节数, 注意int, short, float 需要自己乘上相应的字节数
 * out返回值: void *
 */

int main()
{
    char *str = NULL;
    str = (char *)malloc(10); // 注意 malloc返回值是void *, 申请时需要强制转换成需要的类型
    memset(str, 0, 10); // 如果不清空, 申请的区域值是随机的, 养成好习惯
    strcpy(str, "happylife"); // 使用strcpy特别注意拷贝的字符串长度 <= 10 - 1,即要预留字符串结束标志'\0'
    puts(str);
    free(str);
    printf("str[address]: %s[%x]\n", str, str); // 这里的str内容为空, 但是指针地址还在

    str = NULL; // 注意指针free之后该指针仍存在, 最好将它指向天空.
    printf("str[address]:%s[%x]\n", str, str); // 这里的str内容为空, 地址也为空
    return 0;
}

