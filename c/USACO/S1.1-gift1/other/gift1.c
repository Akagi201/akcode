      
/*
ID : akagi201
LANG : C
TASK : gift1
*/

/*
@brief ��ʹ�ýṹ��ʵ�֣�person_name[i] ��Ӧ total_money[i] ��ű���һ��
*/

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 15
#define MAX_NP 11

int main(void)
{
	FILE *fin = fopen("gift1.in", "r");
	FILE *fout = fopen("gift1.out", "w");

	char person_name[MAX_NP][MAX_NAME_LEN] = {0}; // �洢ÿ���˵����� a -> person_name
	char temp_name[MAX_NAME_LEN] = {0};  // ��ʱ�洢ÿ���˵����� b -> temp_name

	int i;
	int j;
	int k;
	int NP;
	int average_money; // �ͳ���Ǯƽ����ÿ����
	int money_give; // ÿ����Ҫ�ͳ���Ǯ
	int number_give; // ÿ����Ҫ�ͳ���
	int total_money[MAX_NP]={0}; // ÿ����ӵ�е�Ǯ������

	fscanf(fin, "%d", &NP);
	for (i = 0; i < NP; ++i)
	{
		fscanf(fin, "%s", person_name[i]);
	}

	for (i = 0; i < NP; ++i)
	{
		j = 0;
		fscanf(fin, "%s", temp_name);
		
		// �õ����ڴ�����˵����
		while (strcmp(person_name[j], temp_name))
		{
			++j;
		}
		
		fscanf(fin, "%d %d", &money_give, &number_give);
		if (0 == number_give)
		{
			average_money= 0;
		}
		else
		{
			average_money = money_give / number_give;
		}
		total_money[j] -= average_money * number_give; // �����ͳ��ߵ�Ǯ��

		for (k = 0; k < number_give; ++k)
		{
			j = 0;
			fscanf(fin, "%s", temp_name);
			while (strcmp(person_name[j], temp_name))
			{
				++j;
			}
			total_money[j] += average_money; // ��������ߵ�Ǯ��
		}
	}

	for (i = 0; i < NP; ++i)
	{
		fprintf(fout, "%s %d\n", person_name[i], total_money[i]);
	}
	
	fclose(fin);
	fclose(fout);
	
	return 0;
}
