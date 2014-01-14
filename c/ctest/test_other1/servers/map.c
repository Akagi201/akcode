/**	@file map.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief hash表操作函数 
 *
 *	@author		zhoualei
 *	@date		2012/09/03 
 *
 *	@note hash表的创建、插入、删除、寻找 
 *	@note
 */
#include "map.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/**	@fn	HASH_MAP * map_new(int max)
 *	@brief	创建hash表用来存放SOCKET id 
 *	@param	max 创建的hash表的大小 
 *	@return	指向hash表的指针 
 */
HASH_MAP * map_new(int max)
{
	int sz = 1;
	while (sz <= max)/*调整hash表大小为2的n次方*/ 
	{
		sz *= 2;
	}
	/*为hash表头结构分配空间*/ 
	HASH_MAP * hash_map = malloc(sizeof(*hash_map));
	if(NULL == hash_map)
	{
		perror("malloc");
	}
	hash_map->size = sz;
	/*为hash表内节点分配空间*/
	hash_map->hash = malloc(sizeof(HASH_NODE) * sz);
	if(NULL == hash_map->hash)
	{
		perror("malloc");
	}
	int i;
	/*初始化hash表*/
	for (i=0;i<sz;i++)
	{
		hash_map->hash[i].fd = -1;
		hash_map->hash[i].id = 0;
		hash_map->hash[i].next = -1;
	}
	return hash_map;
}
/**	@fn	void map_delete(HASH_MAP * hash_map)
 *	@brief	删除hash表 
 *	@param	hash_map 指向hash表的指针
 *	@return	N\A 
 */
void map_delete(HASH_MAP * hash_map)
{
	/*参数检查*/ 
	if(NULL == hash_map)
	{
		return;
	} 
	free(hash_map->hash);
	free(hash_map);
}

/**	@fn	int map_search(HASH_MAP * hash_map, int fd)
 *	@brief	根据sockfd查找SOCKET i
 *	@param	hash_map 指向hash表的指针
 *	@param	fd	sockfd 
 *	@return	SOCKET i 
 */
int map_search(HASH_MAP * hash_map, int fd)
{
	/*参数检查*/ 
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
		/*若发生碰撞则根据next下移*/
		hash_node = &hash_map->hash[hash_node->next];
	}
}
/**	@fn	void map_insert(HASH_MAP * hash_map, int fd, int id)
 *	@brief	根据sockfd将SOCKET i插入到hash表
 *	@param	hash_map 指向hash表的指针
 *	@param	fd	sockfd 
 *	@return	N\A 
 */
void map_insert(HASH_MAP * hash_map, int fd, int id)
{
	/*参数检查*/
	if(NULL == hash_map)
	{
		return;
	} 
	int hash = fd & (hash_map->size-1);
	HASH_NODE * hash_node = &hash_map->hash[hash];
	/*指向发生碰撞的最后一个*/ 
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
	/*寻找一个未分配的hash表节点分配并做好碰撞链接*/ 
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
 *	@brief	根据sockfd擦除SOCKET i 
 *	@param	hash_map 指向hash表的指针
 *	@param	fd	sockfd 
 *	@return	N\A 
 */
void map_erase(HASH_MAP *hash_map , int fd) 
{
	/*参数检查*/ 
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
		/*发生碰撞，根据next下移*/ 
		hash_node = &hash_map->hash[hash_node->next];
	}
}
