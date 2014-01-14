/**	@file ringbuffer.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief ���λ������Ĺ��� 
 *
 *	@author		zhoualei
 *	@date		2012/08/29
 *
 *	@note ���λ�������֯��ʽ rbͷ rb_blkͷ ���� rb_blkͷ ����...... 
 *	@note ���λ������Ĵ������������� 
 */
#ifndef MREAD_RINGBUFFER_H
#define MREAD_RINGBUFFER_H

typedef struct
{
	int size;/*ringbuffer���ܴ�С*/ 
	int head;/*��������Ŀռ��ƫ����*/ 
}RINGBUFFER;

typedef struct{
	int length;/*���ݿ鳤�Ȱ������ݿ�ͷ������*/ 
	int offset;/*���ݿ����ڵ�ƫ��λ��*/ 
	int id;/*���ݿ�����*/
	int next;/*ͬһ���ӵ���һ���ݿ��ƫ��λ��*/
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

