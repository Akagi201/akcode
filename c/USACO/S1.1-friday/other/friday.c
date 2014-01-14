
/*
ID : akagi201
LANG : C
TASK : friday
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>

int week[8];
int get_week(int c, int y, int m, int d)
{
	return y + y / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + d - 1;
}

int main(void)
{
	int n;
	int tmp;
	int year;
	int month;
	int i;
	int j;
	
	FILE *fin = fopen("friday.in", "r");
	FILE *fout = fopen("friday.out", "w");
	
	assert(fin != NULL && fout != NULL);

	memset(week, 0, sizeof(week));

	scanf("%d", &n);

	for (i = 0; i < n; ++i)
	{
		for (j = i; j <= 12; ++j)
		{
			year = i;
			month = j;
			if (j < 3)
			{
				year -= 1;
				month += 12;
			}
			tmp = get_week((1900 + year) / 100, (1900 + year) % 100, month, 13) % 7; 
			if (tmp < 0)
			{
				tmp += 7;
			}
			++week[tmp];
		}
	}

	fprintf(fout, "%d ", week[6]);

	for(i = 0;i < 5; ++i)
	{
		fprintf(fout, " ", week[i]);
	}

	fprintf(fout, "%d\n", week[5]);
	
	fclose(fin);
	fclose(fout);
	return 0;
}

