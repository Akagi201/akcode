/**	@file ringbuffer.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 环形缓冲区的管理 
 *
 *	@author		zhoualei
 *	@date		2012/08/29
 *
 *	@note 环形缓冲区组织形式 rb头 rb_blk头 数据 rb_blk头 数据...... 
 *	@note 环形缓冲区的创建及管理函数库 
 */
#ifndef MREAD_RINGBUFFER_H
#define MREAD_RINGBUFFER_H

typedef struct
{
	int size;/*ringbuffer的总大小*/ 
	int head;/*即将分配的空间的偏移量*/ 
}RINGBUFFER;

typedef struct{
	int length;/*数据块长度包括数据块头和数据*/ 
	int offset;/*数据块所在的偏移位置*/ 
	int id;/*数据块所属*/
	int next;/*同一连接的下一数据块的偏移位置*/
}RBUF_BLOCK;

RINGBUFFER * ringbuffer_new(int size);
void ringbuffer_delete(RINGBUFFER * rb);
void ringbuffer_link(RINGBUFFER *rb , RBUF_BLOCK * prev, RBUF_BLOCK * next);
RBUF_BLOCK * ringbuffer_alloc(RINGBUFFER * rb, int size);
int ringbuffer_collect(RINGBUFFER * rb);
void ringbuffer_resize(RINGBUFFER * rb, RBUF_BLOCK * blk, int size);
void ringbuffer_free(RINGBUFFER * rb, RBUF_BLOCK * blk);
int ringbuffer_data(RINGBUFFER * rb, RBUF_BLOCK * blk, int size, int skip, void **ptr);
void * ringbuffer_copy(RINGBUFFER * rb, RBUF_BLOCK * from, int skip, RBUF_BLOCK * to);
RBUF_BLOCK * ringbuffer_yield(RINGBUFFER * rb, RBUF_BLOCK *blk, int skip);

#endif

