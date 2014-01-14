/**	@file ringbuffer.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief ���λ������Ĺ��� 
 *
 *	@author		zhoualei
 *	@date		2012/08/29
 *
 *	@note ���λ�������֯��ʽ rbͷ rb_blkͷ ���� rb_blkͷ ����...... 
 *	@note ���λ������Ĵ������������� 
 */

#include "ringbuffer.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

/*4�ֽڶ���*/
#define ALIGN(s) (((s) + 3 ) & ~3)

/**	@fn	static inline int block_offset(RINGBUFFER * rb, RBUF_BLOCK * blk)
 *	@brief	���ݵ�ǰ���ݿ�ָ��������ݿ��ƫ��λ�� 
 *	@param rb ���λ�����ָ��.
 *	@param blk ��ǰ���ݿ�ָ��.
 *	@return	���ݿ��ƫ���� 
 */
static inline int block_offset(RINGBUFFER * rb, RBUF_BLOCK * blk) 
{
	char * start = (char *)(rb + 1);
	return (char *)blk - start;
}

/**	@fn	static inline RBUF_BLOCK *block_ptr(RINGBUFFER * rb, int offset)
 *	@brief	�������ݿ��ƫ��λ��Ѱ�����ݿ� 
 *	@param rb ���λ�����ָ��.
 *	@param offset ���ݿ�ƫ��λ��.
 *	@return	ָ�����ݿ��ָ�� 
 */
static inline RBUF_BLOCK *block_ptr(RINGBUFFER * rb, int offset)
{
	char * start = (char *)(rb + 1);
	return (RBUF_BLOCK *)(start + offset);
}

/**	@fn	static inline RBUF_BLOCK *block_next(RINGBUFFER * rb, RBUF_BLOCK * blk)
 *	@brief	ָ��ָ����һ�����ݿ� 
 *	@param rb ���λ�����ָ��.
 *	@param blk ��ǰ���ݿ�ָ��.
 *	@return	ָ����һ�����ݿ��ָ�� 
 */
static inline RBUF_BLOCK *block_next(RINGBUFFER * rb, RBUF_BLOCK * blk)
{
	/*�����ʱ���ǰ�4�ֽڶ���ġ��м������Ƭ*/ 
	int align_length = ALIGN(blk->length);
	int head = block_offset(rb, blk);
	/*�����ǰ��blk�Ѿ���buffer����β�����ʾû����һ��*/ 
	if (align_length + head == rb->size) 
	{
		return NULL;
	}
	/*�����ǰ����Ŀռ���ϵ�ǰ��λ�ô���ringbuffersize��δ���*/ 
	assert(align_length + head < rb->size);
	return block_ptr(rb, head + align_length);
}

/**	@fn	RINGBUFFER *ringbuffer_new(int size)
 *	@brief	����һ��ringbuffer 
 *	@param size ringbuffer�Ĵ�С 
 *	@return	ָ��ringbuffer��ָ��rb 
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
 *	@brief	ɾ��ringbuffer 
 *	@param rb ���λ�����ָ��
 *	@return	N/A
 */
void ringbuffer_delete(RINGBUFFER * rb)
{
	free(rb);
	rb = NULL;
}

/**	@fn	void ringbuffer_link(RINGBUFFER *rb , RBUF_BLOCK * head, RBUF_BLOCK * next)
 *	@brief	��ͬһ���ӵ����ݿ��������� 
 *	@param rb ���λ�����ָ��.
 *	@param head ָ��ͬһ���ӵ�һ�����ݿ�
 *	@param next ָ��Ҫ���ӵ����ݿ� 
 *	@return	N/A
 */
void ringbuffer_link(RINGBUFFER *rb , RBUF_BLOCK * head, RBUF_BLOCK * next)
{
	/*ָ��ͬһ���ӵ����ݿ����β*/ 
	while (head->next >=0) 
	{
		head = block_ptr(rb, head->next);
	}
	next->id = head->id;
	head->next = block_offset(rb, next);
}

/**	@fn	static RBUF_BLOCK *_alloc(RINGBUFFER * rb, int total_size , int size) 
 *	@brief	Ϊ���ݿ����ռ� 
 *	@param rb	���λ�����ָ��.
 *	@param total_size	��������������������ܿռ�	 
 *	@param size 	��Ҫ����Ŀռ� 
 *	@return	ָ���ȷ������ݿ��ָ�� 
 */
static RBUF_BLOCK *_alloc(RINGBUFFER * rb, int total_size , int size) 
{
	RBUF_BLOCK * blk = block_ptr(rb, rb->head);
	int align_length = ALIGN(sizeof(RBUF_BLOCK) + size);
	/*��ʼ�������*/
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
 *	@brief	��ringbuffer�п���size��С�����ݿ� 
 *	@param rb	���λ�����ָ��.
 *	@param size	��Ҫ���ٵ����ݿ�Ĵ�С	 
 *	@return	ָ������ݿ��ָ�� 
 */
RBUF_BLOCK *ringbuffer_alloc(RINGBUFFER * rb, int size) 
{
	/*�������ݿ��С4�ֽڶ���*/ 
	int align_length = ALIGN(sizeof(RBUF_BLOCK) + size);
	int i;
	/*ѭ��2��ʵ��һ�����εĹ���*/
	for (i = 0;i < 2;i++) 
	{
		int free_size = 0;
		RBUF_BLOCK * blk = block_ptr(rb, rb->head);
		do 
		{
		//	printf("blk->length:%d\n",blk->length);
		//	printf("blk->offset:%d\n",blk->offset);
			/*�����ǰblk����ʹ���򷵻�NULL��ʾû��ʣ��Ŀռ���Է���*/ 
			if ((blk->length >= sizeof(RBUF_BLOCK)) && (blk->id >= 0))
			{ 
				return NULL;
			}
			/*������Ƭ*/ 
			free_size += ALIGN(blk->length);
			if (free_size >= align_length) 
			{
				return _alloc(rb, free_size , size);
			}
			blk = block_next(rb, blk);
		} while(blk);
		/*�������û���㹻�Ŀռ�������ͷ����*/
		rb->head = 0;
	}
	return NULL;
}

/**	@fn	static int _last_id(RINGBUFFER * rb) 
 *	@brief Ѱ���������Ļ���ʹ�õ����ݿ�id���Ա��ռ�����	 
 *	@param rb	���λ�����ָ��.	 
 *	@return	���ظ����ݿ�id 
 */
static int _last_id(RINGBUFFER * rb)
{
	int i;
	/*ѭ��2��ʵ��һ�����εĹ���*/
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
 *	@brief �ռ��������Ļ���ʹ�õ�ͬһid�����ݿ��Ա㸲��	 
 *	@param rb	���λ�����ָ��	 
 *	@return	���ظ����ݿ�id 
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
 *	@brief ringbuffer�յ����ݣ�����ringbuffer	 
 *	@param rb	���λ�����ָ��
 *	@param blk	�������ݵĵ�ǰ���п� 
 *	@param size	�������ݵĴ�С 
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
 *	@brief ��ǰ�����ݿ������ĸ�id��	 
 *	@param blk	�������ݵĵ�ǰ���п� 
 *	@return	���ص�ǰ���ݿ��id		
 */
static int _block_id(RBUF_BLOCK * blk)
{
	assert(blk->length >= sizeof(RBUF_BLOCK));
	int id = blk->id;
	assert(id>=0);
	return id;
}

/**	@fn	void ringbuffer_free(RINGBUFFER * rb, RBUF_BLOCK * blk)
 *	@brief ���Ѿ��Ͽ����ӵ����ݿ�id��-1	 
 *	@param rb	ָ��ringbuffer��ָ�� 
 *	@param blk	ָ���ѶϿ����ӵĵ�һ�����ݿ� 
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
 *	@brief �����ݿ��ж�ȡ����	 
 *	@param rb	ָ��ringbuffer��ָ�� 
 *	@param blk	Ҫ��ȡ���������ڵ����ݿ�
 *	@param size Ҫ��ȡ���ֽ���
 *	@param skip	Ҫ��ȡ�����ݵ�ƫ����
 *	@param ptr	ָ��Ҫ��ȡ���ݵĵ�ַ�ĵ�ַ 
 *	@return	��ȡ���ֽ���		
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
 *	@brief �����Ҫ��ȡ�����ݲ���������copy�������Ŀռ���ж�ȡ	 
 *	@param rb	ָ��ringbuffer��ָ�� 
 *	@param from	Դ����ָ�� 
 *	@param skip Դ���ݵ����ƫ���� 
 *	@param to	Ŀ������ָ�� 
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
 *	@brief ����ƫ���������ظ���ȡ	 
 *	@param rb	ָ��ringbuffer��ָ�� 
 *	@param blk	���ݲ��ֶ�ȡ�����ݿ�ָ�� 
 *	@param skip �Ѿ���ȡ�����ݸ��� 
 *	@return	ָ������ݿ��ָ��	
 */
RBUF_BLOCK * ringbuffer_yield(RINGBUFFER * rb, RBUF_BLOCK *blk, int skip) 
{
	int length = blk->length - sizeof(RBUF_BLOCK) - blk->offset;
	for (;;) 
	{
		if (length > skip) 
		{
			blk->offset += skip;// skip �Ѿ���ȡ�������������´��ظ���ȡ�� 
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
