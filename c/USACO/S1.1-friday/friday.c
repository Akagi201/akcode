
/*
ID : akagi201
LANG : C
TASK : friday
*/

/*
@brief 使用求余的方法计算
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/** @brief    判断是否是闰年
  * @param[in]  年份如 1990
  * @param[out]  
  * @return  1 : 闰年 0 : 不是闰年
  */
int is_leap(int y)
{
    return y%4==0 && (y%100 != 0 || y%400 == 0);
}

int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // mtab -> month_days

/* return length of month m in year y */
int month_len(int y, int m) // mlen -> month_len
{
    if(m == 1)    /* february */
    {
        return month_days[m]+is_leap(y);
    }
    else
    {
        return month_days[m];
    }
}

int main(void)
{
    FILE *fin;
	FILE *fout;
    int i;
	int m;
	int day_of_week; // day of week
	int N;
	int y;
    int day_of_week_numbers[7]; // 代表13日是星期六,星期日,星期一...星期五的次数

    fin = fopen("friday.in", "r");
    fout = fopen("friday.out", "w");
    assert(fin != NULL && fout != NULL);

    fscanf(fin, "%d", &N);
	memset(day_of_week_numbers, 0, sizeof(day_of_week_numbers));

    day_of_week = 0;    /* day of week: January 13, 1900 was a Saturday = 0 */
    for(y=1900; y<1900+N; ++y)
	{
        for(m=0; m<12; ++m)
		{
            ++day_of_week_numbers[day_of_week];
            day_of_week = (day_of_week+month_len(y, m)) % 7;
        }
    }

	/* 数据两两之间有空格，两头没有空格的写法 */
    for(i=0; i<7; ++i)
	{
        if(i)
        {
            fprintf(fout, " ");
        }
        fprintf(fout, "%d", day_of_week_numbers[i]);
    }
    fprintf(fout, "\n");
	
	fclose(fin);
	fclose(fout);
    return 0;
}




