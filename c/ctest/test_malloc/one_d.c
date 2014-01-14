#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int count;
    int *array;
    if (NULL == (array = (int *)malloc(10 * sizeof(int))))
    {
        printf("malloc failed\n");
        return 0;
    }
    
    for (count = 0; count < 10; ++count)
    {
        array[count] = count;
        printf("%2d",array[count]);
    }
    printf("\n");
	free(array);
    return 0;
}
