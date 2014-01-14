/**	@file map.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief hash��������� 
 *
 *	@author		zhoualei
 *	@date		2012/09/03 
 *
 *	@note hash��Ĵ��������롢ɾ����Ѱ�� 
 *	@note
 */
#include "map.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/**	@fn	HASH_MAP * map_new(int max)
 *	@brief	����hash���������SOCKET id 
 *	@param	max ������hash��Ĵ�С 
 *	@return	ָ��hash���ָ�� 
 */
HASH_MAP * map_new(int max)
{
	int sz = 1;
	while (sz <= max)/*����hash���СΪ2��n�η�*/ 
	{
		sz *= 2;
	}
	/*Ϊhash��ͷ�ṹ����ռ�*/ 
	HASH_MAP * hash_map = malloc(sizeof(*hash_map));
	if(NULL == hash_map)
	{
		perror("malloc");
	}
	hash_map->size = sz;
	/*Ϊhash���ڽڵ����ռ�*/
	hash_map->hash = malloc(sizeof(HASH_NODE) * sz);
	if(NULL == hash_map->hash)
	{
		perror("malloc");
	}
	int i;
	/*��ʼ��hash��*/
	for (i=0;i<sz;i++)
	{
		hash_map->hash[i].fd = -1;
		hash_map->hash[i].id = 0;
		hash_map->hash[i].next = -1;
	}
	return hash_map;
}
/**	@fn	void map_delete(HASH_MAP * hash_map)
 *	@brief	ɾ��hash�� 
 *	@param	hash_map ָ��hash���ָ��
 *	@return	N\A 
 */
void map_delete(HASH_MAP * hash_map)
{
	/*�������*/ 
	if(NULL == hash_map)
	{
		return;
	} 
	free(hash_map->hash);
	free(hash_map);
}

/**	@fn	int map_search(HASH_MAP * hash_map, int fd)
 *	@brief	����sockfd����SOCKET i
 *	@param	hash_map ָ��hash���ָ��
 *	@param	fd	sockfd 
 *	@return	SOCKET i 
 */
int map_search(HASH_MAP * hash_map, int fd)
{
	/*�������*/ 
	if(NULL == hash_map)
	{
		return -1;
	}
	
	int hash = fd & (hash_map->size-1);
	HASH_NODE * hash_node = &hash_map->hash[hash];
	for(;;)
	{
		if(hash_node->fd == fd)
		{
			return hash_node->id;
		}
		if(hash_node->next < 0)
		{
			return -1;
		}
		/*��������ײ�����next����*/
		hash_node = &hash_map->hash[hash_node->next];
	}
}
/**	@fn	void map_insert(HASH_MAP * hash_map, int fd, int id)
 *	@brief	����sockfd��SOCKET i���뵽hash��
 *	@param	hash_map ָ��hash���ָ��
 *	@param	fd	sockfd 
 *	@return	N\A 
 */
void map_insert(HASH_MAP * hash_map, int fd, int id)
{
	/*�������*/
	if(NULL == hash_map)
	{
		return;
	} 
	int hash = fd & (hash_map->size-1);
	HASH_NODE * hash_node = &hash_map->hash[hash];
	/*ָ������ײ�����һ��*/ 
	for (;;) 
	{
		if (hash_node->fd < 0) 
		{
			hash_node->fd = fd;
			hash_node->id = id;
			return;
		}
		if (hash_node->next < 0 ) 
		{
			break;
		}
		hash_node = &hash_map->hash[hash_node->next];
	}
	int i;
	/*Ѱ��һ��δ�����hash��ڵ���䲢������ײ����*/ 
	for (i = 0; i < hash_map->size; i++) 
	{
		HASH_NODE * temp = &hash_map->hash[i];
		if (temp->fd < 0) 
		{
			temp->fd = fd;
			temp->id = id;
			hash_node->next = i;
			return;
		}
	}
	assert(0);
}
/**	@fn	void map_erase(HASH_MAP *hash_map , int fd)
 *	@brief	����sockfd����SOCKET i 
 *	@param	hash_map ָ��hash���ָ��
 *	@param	fd	sockfd 
 *	@return	N\A 
 */
void map_erase(HASH_MAP *hash_map , int fd) 
{
	/*�������*/ 
	if(NULL == hash_map)
	{
		return;
	}
	int hash = fd & (hash_map->size-1);
	HASH_NODE * hash_node = &hash_map->hash[hash];
	for(;;)
	{
		if(hash_node->fd == fd) 
		{
			hash_node->fd = -1;
			return;
		}
		if (hash_node->next < 0)
		{ 
			return;
		} 
		/*������ײ������next����*/ 
		hash_node = &hash_map->hash[hash_node->next];
	}
}
