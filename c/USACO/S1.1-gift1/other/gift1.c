      
/*
ID : akagi201
LANG : C
TASK : gift1
*/

/*
@brief 不使用结构体实现，person_name[i] 对应 total_money[i] 序号保持一致
*/

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 15
#define MAX_NP 11

int main(void)
{
	FILE *fin = fopen("gift1.in", "r");
	FILE *fout = fopen("gift1.out", "w");

	char person_name[MAX_NP][MAX_NAME_LEN] = {0}; // 存储每个人的姓名 a -> person_name
	char temp_name[MAX_NAME_LEN] = {0};  // 临时存储每个人的名字 b -> temp_name

	int i;
	int j;
	int k;
	int NP;
	int average_money; // 送出的钱平均到每个人
	int money_give; // 每个人要送出的钱
	int number_give; // 每个人要送出的
	int total_money[MAX_NP]={0}; // 每个人拥有的钱的总数

	fscanf(fin, "%d", &NP);
	for (i = 0; i < NP; ++i)
	{
		fscanf(fin, "%s", person_name[i]);
	}

	for (i = 0; i < NP; ++i)
	{
		j = 0;
		fscanf(fin, "%s", temp_name);
		
		// 得到正在处理的人的序号
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
		total_money[j] -= average_money * number_give; // 计算送出者的钱数

		for (k = 0; k < number_give; ++k)
		{
			j = 0;
			fscanf(fin, "%s", temp_name);
			while (strcmp(person_name[j], temp_name))
			{
				++j;
			}
			total_money[j] += average_money; // 计算接收者的钱数
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
