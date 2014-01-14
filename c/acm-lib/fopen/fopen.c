#include <stdio.h>

int main(void)
{
    FILE *fin = fopen("input_file.in", "r");
    FILE *fout = fopen("output_file", "w");
    assert(fin != NULL && fout != NULL);

    //fscanf();
    //fprintf();

    fclose(fin);
    fclose(fout);

    return 0;
}
