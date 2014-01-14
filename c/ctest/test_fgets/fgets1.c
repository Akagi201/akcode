// fgets1.c

#include <string.h>
#include <stdio.h>

int main(void)
{
    FILE *stream;
    char string[] = "This is a test";
    char msg[20];

    // open a file for update
    stream = fopen("a.txt", "w+");
    // write a string into the file
    fwrite(string, strlen(string), 1, stream);
    // seek to the start of the file
    fseek(stream, 0, SEEK_SET);
    // read a string from the file
    fgets(msg, strlen(string) + 1, stream);
    // display the string
    printf("%s", msg);
    fclose(stream);

    return 0;
}
