
/*
ID : akagi201
LANG : C
TASK : gift1
*/

/*
@brief ʹ�ýṹ������������ݱ�����޸Ĺ�����
*/


#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_NAME_LEN 15
#define MAX_NP 11

/** @brief    person attributes
  */

typedef struct person
{
	char name[MAX_NAME_LEN];
	int total; //> ÿ������ӵ�е�ȫ����Ǯ
}person_t;
 
person_t people[MAX_NAME_LEN];

int n_people; //> �˵��������

void add_person(char *name)
{
	assert(n_people < MAX_NP);
	strcpy(people[n_people].name, name);
	++n_people;
}

person_t *lookup(char *name)
{
	int i;

	/* look for name in people table */
	for (i = 0; i < n_people; ++i)
	{
		if (0 == strcmp(name, people[i].name))
		{
			return &people[i];
		}
	}
	assert(0); /* should have found name */
}

int main(void)
{
	int NP;
	int i;
	int j;
	char temp_name[MAX_NAME_LEN];  // ��ʱ�洢���������
	int money_give; //> ��ʱ�洢ÿ����Ҫ�ͳ���Ǯ
	int number_give; //> ��ʱ�洢ÿ����Ҫ�ͳ����˵ĸ���
	person_t *giver;
	person_t *receiver;

	FILE *fin = fopen("gift1.in", "r");
	FILE *fout = fopen("gift1.out", "w");
	
	fscanf(fin, "%d", &NP);
	assert (NP <= MAX_NP);

	n_people = 0;
	for (i = 0; i < NP; ++i)
	{
		fscanf(fin, "%s", temp_name);
		add_person(temp_name);
	}

	/* process gift lines */
	for (i = 0; i < NP; ++i)
	{
		fscanf(fin, "%s %d %d", temp_name, &money_give, &number_give);
		giver = lookup(temp_name);

		for (j = 0; j < number_give; ++j)
		{
			fscanf(fin, "%s", temp_name);
			receiver = lookup(temp_name);
			giver->total -= money_give / number_give;
			receiver->total += money_give / number_give;
		}
	}

	/* print gift totals */
	for (i = 0; i < NP; ++i)
	{
		fprintf(fout, "%s %d\n", people[i].name, people[i].total);
	}

	fclose(fin);
	fclose(fout);
	
    return 0;
}
