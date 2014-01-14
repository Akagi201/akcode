/**	@file ringbuffer.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 环形缓冲区的管理 
 *
 *	@author		zhoualei
 *	@date		2012/08/29
 *
 *	@note 环形缓冲区组织形式 rb头 rb_blk头 数据 rb_blk头 数据...... 
 *	@note 环形缓冲区的创建及管理函数库 
 */

#include "ringbuffer.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

/*4字节对齐*/
#define ALIGN(s) (((s) + 3 ) & ~3)

/**	@fn	static inline int block_offset(RINGBUFFER * rb, RBUF_BLOCK * blk)
 *	@brief	根据当前数据块指针计算数据块的偏移位置 
 *	@param rb 环形缓冲区指针.
 *	@param blk 当前数据块指针.
 *	@return	数据块的偏移量 
 */
static inline int block_offset(RINGBUFFER * rb, RBUF_BLOCK * blk) 
{
	char * start = (char *)(rb + 1);
	return (char *)blk - start;
}

/**	@fn	static inline RBUF_BLOCK *block_ptr(RINGBUFFER * rb, int offset)
 *	@brief	根据数据块的偏移位置寻找数据块 
 *	@param rb 环形缓冲区指针.
 *	@param offset 数据块偏移位置.
 *	@return	指向数据块的指针 
 */
static inline RBUF_BLOCK *block_ptr(RINGBUFFER * rb, int offset)
{
	char * start = (char *)(rb + 1);
	return (RBUF_BLOCK *)(start + offset);
}

/**	@fn	static inline RBUF_BLOCK *block_next(RINGBUFFER * rb, RBUF_BLOCK * blk)
 *	@brief	指针指向下一个数据块 
 *	@param rb 环形缓冲区指针.
 *	@param blk 当前数据块指针.
 *	@return	指向下一块数据块的指针 
 */
static inline RBUF_BLOCK *block_next(RINGBUFFER * rb, RBUF_BLOCK * blk)
{
	/*分配的时候是按4字节对齐的。中间会有碎片*/ 
	int align_length = ALIGN(blk->length);
	int head = block_offset(rb, blk);
	/*如果当前的blk已经在buffer的最尾，则表示没有下一块*/ 
	if (align_length + head == rb->size) 
	{
		return NULL;
	}
	/*如果当前分配的空间加上当前的位置大于ringbuffersize则段错误*/ 
	assert(align_length + head < rb->size);
	return block_ptr(rb, head + align_length);
}

/**	@fn	RINGBUFFER *ringbuffer_new(int size)
 *	@brief	创建一个ringbuffer 
 *	@param size ringbuffer的大小 
 *	@return	指向ringbuffer的指针rb 
 */
RINGBUFFER *ringbuffer_new(int size)
{
	RINGBUFFER * rb = NULL; 
	if(NULL == (rb = (RINGBUFFER *)malloc(sizeof(*rb) + size)))
	{
		perror("malloc");	
	}
	rb->size = size;
	rb->head = 0;
	RBUF_BLOCK * blk = block_ptr(rb, 0);
	blk->length = size;
	blk->id = -1;
	return rb;
}

/**	@fn	void ringbuffer_delete(RINGBUFFER * rb)
 *	@brief	删除ringbuffer 
 *	@param rb 环形缓冲区指针
 *	@return	N/A
 */
void ringbuffer_delete(RINGBUFFER * rb)
{
	free(rb);
	rb = NULL;
}

/**	@fn	void ringbuffer_link(RINGBUFFER *rb , RBUF_BLOCK * head, RBUF_BLOCK * next)
 *	@brief	将同一连接的数据块连接起来 
 *	@param rb 环形缓冲区指针.
 *	@param head 指向同一连接的一块数据块
 *	@param next 指向要连接的数据块 
 *	@return	N/A
 */
void ringbuffer_link(RINGBUFFER *rb , RBUF_BLOCK * head, RBUF_BLOCK * next)
{
	/*指到同一连接的数据块的最尾*/ 
	while (head->next >=0) 
	{
		head = block_ptr(rb, head->next);
	}
	next->id = head->id;
	head->next = block_offset(rb, next);
}

/**	@fn	static RBUF_BLOCK *_alloc(RINGBUFFER * rb, int total_size , int size) 
 *	@brief	为数据块分配空间 
 *	@param rb	环形缓冲区指针.
 *	@param total_size	可以用来分配的连续的总空间	 
 *	@param size 	需要分配的空间 
 *	@return	指向先分配数据块的指针 
 */
static RBUF_BLOCK *_alloc(RINGBUFFER * rb, int total_size , int size) 
{
	RBUF_BLOCK * blk = block_ptr(rb, rb->head);
	int align_length = ALIGN(sizeof(RBUF_BLOCK) + size);
	/*初始化分配块*/
	blk->length = sizeof(RBUF_BLOCK) + size;
	blk->offset = 0;
	blk->next = -1;
	blk->id = -1;
	RBUF_BLOCK * next = block_next(rb, blk);
	rb->head = block_offset(rb, next);
	if (align_length < total_size) 
	{
		next->length = total_size - align_length;
		if (next->length >= sizeof(RBUF_BLOCK)) 
		{
			next->id = -1;
		}
	}
//	printf("rb->head:%d\n",rb->head);
	return blk;
}
/**	@fn	RBUF_BLOCK *ringbuffer_alloc(RINGBUFFER * rb, int size) 
 *	@brief	在ringbuffer中开辟size大小的数据块 
 *	@param rb	环形缓冲区指针.
 *	@param size	需要开辟的数据块的大小	 
 *	@return	指向该数据块的指针 
 */
RBUF_BLOCK *ringbuffer_alloc(RINGBUFFER * rb, int size) 
{
	/*调整数据块大小4字节对齐*/ 
	int align_length = ALIGN(sizeof(RBUF_BLOCK) + size);
	int i;
	/*循环2次实现一个环形的功能*/
	for (i = 0;i < 2;i++) 
	{
		int free_size = 0;
		RBUF_BLOCK * blk = block_ptr(rb, rb->head);
		do 
		{
		//	printf("blk->length:%d\n",blk->length);
		//	printf("blk->offset:%d\n",blk->offset);
			/*如果当前blk还在使用则返回NULL表示没有剩余的空间可以分配*/ 
			if ((blk->length >= sizeof(RBUF_BLOCK)) && (blk->id >= 0))
			{ 
				return NULL;
			}
			/*回收碎片*/ 
			free_size += ALIGN(blk->length);
			if (free_size >= align_length) 
			{
				return _alloc(rb, free_size , size);
			}
			blk = block_next(rb, blk);
		} while(blk);
		/*如果后面没有足够的空间分配则从头分配*/
		rb->head = 0;
	}
	return NULL;
}

/**	@fn	static int _last_id(RINGBUFFER * rb) 
 *	@brief 寻找最早分配的还在使用的数据块id，以便收集覆盖	 
 *	@param rb	环形缓冲区指针.	 
 *	@return	返回该数据块id 
 */
static int _last_id(RINGBUFFER * rb)
{
	int i;
	/*循环2次实现一个环形的功能*/
	for (i = 0;i < 2;i++) 
	{
		RBUF_BLOCK * blk = block_ptr(rb, rb->head);
		do 
		{
			if ((blk->length >= sizeof(RBUF_BLOCK)) && (blk->id >= 0))
			{
				return blk->id;
			}
			blk = block_next(rb, blk);
		} while(blk);
		rb->head = 0;
	}
	return -1;
}

/**	@fn	int ringbuffer_collect(RINGBUFFER * rb) 
 *	@brief 收集最早分配的还在使用的同一id的数据块以便覆盖	 
 *	@param rb	环形缓冲区指针	 
 *	@return	返回该数据块id 
 */
int ringbuffer_collect(RINGBUFFER * rb) 
{
	int id = _last_id(rb);
	RBUF_BLOCK *blk = block_ptr(rb, 0);
	do 
	{
		if (blk->length >= sizeof(RBUF_BLOCK) && blk->id == id) 
		{
			blk->id = -1;
		}
		blk = block_next(rb, blk);
	} while(blk);
	return id;
}

/**	@fn	void ringbuffer_resize(RINGBUFFER * rb, RBUF_BLOCK * blk, int size) 
 *	@brief ringbuffer收到数据，调整ringbuffer	 
 *	@param rb	环形缓冲区指针
 *	@param blk	接受数据的当前空闲块 
 *	@param size	接受数据的大小 
 *	@return	N/A
 */
void ringbuffer_resize(RINGBUFFER * rb, RBUF_BLOCK * blk, int size) 
{
	if (size == 0) 
	{
		rb->head = block_offset(rb, blk);
		return;
	}
	int align_length = ALIGN(sizeof(RBUF_BLOCK) + size);
	int old_length = ALIGN(blk->length);
	assert(align_length < old_length);
	blk->length = size + sizeof(RBUF_BLOCK);
	if (align_length == old_length) 
	{
		return;
	}
	blk = block_next(rb, blk);
	blk->length = old_length - align_length;
	if (blk->length >= sizeof(RBUF_BLOCK)) 
	{
		blk->id = -1;
	}
	rb->head = block_offset(rb, blk);
}

/**	@fn	static int _block_id(RBUF_BLOCK * blk) 
 *	@brief 当前的数据块属于哪个id的	 
 *	@param blk	接受数据的当前空闲块 
 *	@return	返回当前数据块的id		
 */
static int _block_id(RBUF_BLOCK * blk)
{
	assert(blk->length >= sizeof(RBUF_BLOCK));
	int id = blk->id;
	assert(id>=0);
	return id;
}

/**	@fn	void ringbuffer_free(RINGBUFFER * rb, RBUF_BLOCK * blk)
 *	@brief 将已经断开连接的数据块id置-1	 
 *	@param rb	指向ringbuffer的指针 
 *	@param blk	指向已断开连接的第一块数据块 
 *	@return	N/A		
 */
void ringbuffer_free(RINGBUFFER * rb, RBUF_BLOCK * blk)
{
	if (blk == NULL)
	{ 
		return;
	} 
	int id = _block_id(blk);
	blk->id = -1;
	while (blk->next >= 0) 
	{
		blk = block_ptr(rb, blk->next);
		assert(_block_id(blk) == id);
		blk->id = -1;
	}
}

/**	@fn	int ringbuffer_data(RINGBUFFER * rb, RBUF_BLOCK * blk, int size, int skip, void **ptr)
 *	@brief 从数据块中读取数据	 
 *	@param rb	指向ringbuffer的指针 
 *	@param blk	要读取的数据所在的数据块
 *	@param size 要读取的字节数
 *	@param skip	要读取的数据的偏移量
 *	@param ptr	指向要读取数据的地址的地址 
 *	@return	读取的字节数		
 */
int ringbuffer_data(RINGBUFFER * rb, RBUF_BLOCK * blk, int size, int skip, void **ptr) 
{
	int length = blk->length - sizeof(RBUF_BLOCK) - blk->offset;
	for (;;) 
	{
		if (length > skip) 
		{
			if (length - skip >= size) 
			{
				char * start = (char *)(blk + 1);
				*ptr = (start + blk->offset + skip);
				return size;
			}
			*ptr = NULL;
			int ret = length - skip;
			while (blk->next >= 0) 
			{
				blk = block_ptr(rb, blk->next);
				ret += blk->length - sizeof(RBUF_BLOCK);
				if (ret >= size)
				{ 
					return size;
				} 
			}
			return ret;
		}
		if (blk->next < 0) 
		{
			assert(length == skip);
			*ptr = NULL;
			return 0;
		}
		blk = block_ptr(rb, blk->next);
		assert(blk->offset == 0);
		skip -= length;
		length = blk->length - sizeof(RBUF_BLOCK);
	}
}

/**	@fn	void *ringbuffer_copy(RINGBUFFER * rb, RBUF_BLOCK * from, int skip, RBUF_BLOCK * to) 
 *	@brief 如果需要读取的数据部连续则先copy到连续的空间进行读取	 
 *	@param rb	指向ringbuffer的指针 
 *	@param from	源数据指针 
 *	@param skip 源数据的相对偏移量 
 *	@param to	目的数据指针 
 *	@return	N/A	
 */
void *ringbuffer_copy(RINGBUFFER * rb, RBUF_BLOCK * from, int skip, RBUF_BLOCK * to) 
{
	int size = to->length - sizeof(RBUF_BLOCK);
	int length = from->length - sizeof(RBUF_BLOCK) - from->offset;
	char * ptr = (char *)(to+1);
	for (;;) 
	{
		if (length > skip) 
		{
			char * src = (char *)(from + 1);
			src += from->offset + skip;
			length -= skip;
			while (length < size) 
			{
				memcpy(ptr, src, length);
				assert(from->next >= 0);
				from = block_ptr(rb , from->next);
				assert(from->offset == 0);
				ptr += length;
				size -= length;
				length = from->length;
				src =  (char *)(from + 1);
			}
			memcpy(ptr, src , size);
			to->id = from->id;
			return (char *)(to + 1);
		}
		assert(from->next >= 0);
		from = block_ptr(rb, from->next);
		assert(from->offset == 0);
		skip -= length;
		length = from->length - sizeof(RBUF_BLOCK);
	}
}

/**	@fn	RBUF_BLOCK * ringbuffer_yield(RINGBUFFER * rb, RBUF_BLOCK *blk, int skip) 
 *	@brief 设置偏移量避免重复读取	 
 *	@param rb	指向ringbuffer的指针 
 *	@param blk	数据部分读取的数据块指针 
 *	@param skip 已经读取的数据个数 
 *	@return	指向该数据块的指针	
 */
RBUF_BLOCK * ringbuffer_yield(RINGBUFFER * rb, RBUF_BLOCK *blk, int skip) 
{
	int length = blk->length - sizeof(RBUF_BLOCK) - blk->offset;
	for (;;) 
	{
		if (length > skip) 
		{
			blk->offset += skip;// skip 已经读取的数量。避免下次重复读取。 
			return blk;
		}
		blk->id = -1;
		if (blk->next < 0) 
		{
			return NULL;
		}
		blk = block_ptr(rb, blk->next);
		assert(blk->offset == 0);
		skip -= length;
		length = blk->length - sizeof(RBUF_BLOCK);
		printf("length yy:%d\n",length);
	}
}
