
/*
ID:smilecl1
LANG:C
TASK:gift1
*/

/*
TIME:2012年12月11日
*/

#include<stdio.h>
#include<string.h>

#define max 10
#define max_length 15

struct person
{
    char name[max_length];
    int  money;
};

struct person people[max];

int main()
{
    FILE *fin,*fout;
    char giver[max_length]={0};
    char receiver[max_length]={0};
    int np;
    int i=0;
    int j=0;
    int k=0;
    int giver_money=0;
    int giver_number=0;
    int money_temp=0;

    fin=fopen("input_file.in","r");
    fout=fopen("output_file.out","w");
    fscanf(fin, "%d",&np);
    for(i=0;i<max;i++)
    {
        // people[i].name={0};
        people[i].money=0;
    }

    for(i=0;i<np;++i)
    {
        fscanf(fin,"%s", people[i].name);
    }

    for(i=0;i<np;++i)
    {
        k=0;
        fscanf(fin,"%s%d%d",giver,&giver_money,&giver_number);
        if(0==giver_money || 0==giver_number)
            money_temp=0;
        else
            money_temp=giver_money/giver_number;
        while(strcmp(people[k].name,giver))
            k++;
        people[k].money-=money_temp*giver_number;
        for(j=0;j<giver_number;++j)
        {
            fscanf(fin,"%s",receiver);
            k=0;
            while(strcmp(people[k].name,receiver))
                k++;
            people[k].money+=money_temp;
        }
    }

    for(i=0;i<np;++i)
    {
        fprintf(fout,"%s %d\n",people[i].name,people[i].money);
    }
    fclose(fin);
    fclose(fout);
    exit(0);
}

