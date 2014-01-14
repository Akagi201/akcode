
/*
ID : akagi201
LANG : C
TASK : friday
*/

/*
@brief ʹ������ķ�������
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/** @brief    �ж��Ƿ�������
  * @param[in]  ����� 1990
  * @param[out]  
  * @return  1 : ���� 0 : ��������
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
    int day_of_week_numbers[7]; // ����13����������,������,����һ...������Ĵ���

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

	/* ��������֮���пո���ͷû�пո��д�� */
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




