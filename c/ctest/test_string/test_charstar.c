#include <stdio.h>

//char message[10]="123456789";
//char *message_ptr;

int main()
{
    char message[10]="123456789";
    char *message_ptr;
    message_ptr = message;
    printf("%s\n", message_ptr);
    return 0;
}
