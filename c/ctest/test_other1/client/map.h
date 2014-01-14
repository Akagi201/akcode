/**	@file map.h 
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief hash表操作函数 
 *
 *	@author		zhoualei
 *	@date		2012/09/03 
 *
 *	@note hash表的创建、插入、删除、寻找 
 *	@note
 */
#ifndef MREAD_MAP_H
#define MREAD_MAP_H
typedef struct
{
	int fd;/*sockfd*/ 
	int id;/*客户端id*/
	int next;/*发生碰撞的下指位置*/
}HASH_NODE;

typedef struct
{
	int size;/*hash表大小*/
	HASH_NODE *hash;/*指向hash表节点的指针*/
}HASH_MAP;

HASH_MAP * map_new(int max);
void map_delete(HASH_MAP *hash_map);
int map_search(HASH_MAP *hash_map, int fd);
void map_insert(HASH_MAP *hash_map, int fd, int id);
void map_erase(HASH_MAP *hash_map, int fd);
#endif
