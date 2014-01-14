#include <stdio.h>

int main(void)
{
    freopen("input_file.in", "r", stdin);
    freopen("output_file.out", "w", stdout);

    //scanf();
    //printf();

    fclose(stdin);
    fclose(stdout);
    return 0;
}
