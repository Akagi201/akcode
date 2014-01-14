/**	@file map.h 
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief hash��������� 
 *
 *	@author		zhoualei
 *	@date		2012/09/03 
 *
 *	@note hash��Ĵ��������롢ɾ����Ѱ�� 
 *	@note
 */
#ifndef MREAD_MAP_H
#define MREAD_MAP_H
typedef struct
{
	int fd;/*sockfd*/ 
	int id;/*�ͻ���id*/
	int next;/*������ײ����ָλ��*/
}HASH_NODE;

typedef struct
{
	int size;/*hash���С*/
	HASH_NODE *hash;/*ָ��hash��ڵ��ָ��*/
}HASH_MAP;

HASH_MAP * map_new(int max);
void map_delete(HASH_MAP *hash_map);
int map_search(HASH_MAP *hash_map, int fd);
void map_insert(HASH_MAP *hash_map, int fd, int id);
void map_erase(HASH_MAP *hash_map, int fd);
#endif
