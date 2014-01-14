
/*
ID : akagi201
LANG : C
TASK : ride
*/

#include <stdio.h>
#include <string.h>

int main(void)
{
    int i;
    int comet_product = 1;
    int group_product = 1;

    FILE *fin = fopen("ride.in", "r");
    FILE *fout = fopen("ride.out", "w");

    char comet_name[7] = "";
    char group_name[7] = "";

    fscanf(fin, "%s", comet_name);
    fscanf(fin, "%s", group_name);

    for (i = 0; i < strlen(comet_name); ++i)
    {
        comet_product *= ((int)comet_name[i] - 'A' + 1);
        comet_product %= 47;
    }

    for (i = 0; i < strlen(group_name); ++i)
    {
        group_product *= ((int)group_name[i] - 'A' + 1);
        group_product %= 47;
    }
    
    if (comet_product == group_product)
    {
        fprintf(fout, "%s\n", "GO");
    }
    else
    {
        fprintf(fout, "%s\n", "STAY");
    }
    
    fclose("ride.in");
    fclose("ride.out");
    return 0;
}
