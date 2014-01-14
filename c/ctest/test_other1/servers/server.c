/**	@file server.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief ������API���� 
 *
 *	@author		zhoualei
 *	@date		2012/09/08
 *
 *	@note �����������ڴ漰������API���� 
 *	@note ���������ӡʵ�ֶ�дһ���Ļ����д���ݿ� 
 */
#include "server.h"

/**	@fn	static SOCKET *_create_sockets(int max)
 *	@brief	�����ͳ�ʼ��SOCKET�ṹ������ 
 *	@param max �ṹ������ 
 *	@return	ָ��ýṹ�������ָ�� 
 */
static SOCKET *_create_sockets(int max) 
{
	int i;
	/*ΪSOCKET�ṹ���������ռ�*/ 
	SOCKET * sock = malloc(max * sizeof(SOCKET));
	if(NULL == sock)
	{
		perror("malloc");	
	} 
	/*SOCKET�ṹ�������ʼ��*/
	for (i=0;i<max;i++) 
	{
		sock[i].fd = i+1;
		sock[i].pthread_flag = 1;
		sock[i].recev_filesize = 0;
		sock[i].send_ready_filesize = 0;
		sock[i].filesize_total = 0; 
		sock[i].node = NULL;
		sock[i].temp = NULL;
		sock[i].status = SOCKET_INVALID;		
	}
	sock[max-1].fd = -1;/*��ʶ�����ṹ������*/ 
	return sock;
}

/**	@fn	static RINGBUFFER *_create_rb(int size)
 *	@brief	�����ͳ�ʼ��ringbuffer 
 *	@param size ringbuffer�Ĵ�С����Ϊ0��Ĭ��Ϊ1K�� 
 *	@return	ָ���ringbuffer��ָ�� 
 */
static RINGBUFFER *_create_rb(int size) 
{
	/*4�ֽڶ���*/ 
	size = (size + 3) & ~3;
	/*����2������copy*/ 
	if (size < READBLOCKSIZE * 2) 
	{
		size = READBLOCKSIZE * 2;
	}
	/*������ʼ��ringbuffer*/
	RINGBUFFER * rb = ringbuffer_new(size);

	return rb;
}
/**	@fn	static void _release_rb(RINGBUFFER * rb)
 *	@brief	ɾ��ringbuffer 
 *	@param rb  ָ���ringbuffer��ָ�� 
 *	@return	N\A
 */ 
static void _release_rb(RINGBUFFER * rb) 
{
	ringbuffer_delete(rb);
}

/**	@fn	MREAD_POLL * mread_create(int port , int max , int buffer_size) 
 *	@brief	����poll�������˿ڣ������������������ʼ��ringbuffer��map��SOCKET�ṹ������ 
 *	@param max ��������� 
 *	@param buffer_size ringbuffer�Ĵ�С 
 *	@return	ָ��MREAD_POLL�ṹ���ָ�� 
 */
MREAD_POLL * mread_create(int port , int max , int buffer_size) 
{
	int listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (listen_fd == -1) 
	{
		return NULL;
	}
	int reuse = 1;
	/*�����׽ӿ�����*/ 
	/*���õ���closesocket()��,�Կɼ������ø�socket������closesocket()һ�㲻�������ر�socket��������TIME_WAIT�Ĺ���*/
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
	
	struct sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_LOOPBACK;
	pthread_mutex_lock(&prin_mutex);
	printf("MREAD bind %s:%u\n",inet_ntoa(my_addr.sin_addr),ntohs(my_addr.sin_port));
	pthread_mutex_unlock(&prin_mutex);
	/*��*/
	if (bind(listen_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		close(listen_fd);
		return NULL;
	}
	/*����*/ 
	if (listen(listen_fd, BACKLOG) == -1) 
	{
		close(listen_fd);
		return NULL;
	}
	/*����һ��epollר�õ��ļ�������������ܴ��max+1��socket fd*/ 
	int epoll_fd = epoll_create(max + 1);
	if (epoll_fd == -1) 
	{
		close(listen_fd);
		return NULL;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN;/*��Ӧ���ļ����������Զ�*/ 
	ev.data.fd = listen_fd;/*������Ҫ�����������ɵ��ļ�������*/ 
	/*ע��epoll�¼�*/ 
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) 
	{
		close(listen_fd);
		close(epoll_fd);
		return NULL;
	}
	/*�����ͳ�ʼ���������ݿ�*/ 
	MREAD_POLL * self = malloc(sizeof(MREAD_POLL));
	self->listen_fd = listen_fd;
	self->epoll_fd = epoll_fd;
	self->max_connection = max;
	self->closed = 0;
	self->active = -1;
	self->skip = 0;
	self->pthread_print = 0;
	self->sockets = _create_sockets(max);
	self->free_socket = &self->sockets[0];
	self->socket_hash = map_new(max);
	self->queue_len = 0;
	self->queue_head = 0;
	if (buffer_size == 0) 
	{
		self->rb = _create_rb(RINGBUFFER_DEFAULT);
	} 
	else 
	{
		self->rb = _create_rb(buffer_size);
	}

	return self;
}
 
/**	@fn	void mread_close(MREAD_POLL *self) 
 *	@brief	ɾ��poll 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ 
 *	@return	N\A
 */
void mread_close(MREAD_POLL *self) 
{
	/*������*/ 
	if (self == NULL)
	{
		return;
	}
	int i;
	self->pthread_print = 0;
	SOCKET * sock = self->sockets;
	/*�ر����е������׽���*/ 
	for (i=0;i<self->max_connection;i++) 
	{
		if (sock[i].status != SOCKET_INVALID) 
		{
			close(sock[i].fd);
		}
	}
	free(sock);
	/*�رռ����׽���*/ 
	if (self->listen_fd >= 0) 
	{
		close(self->listen_fd);
	}
	close(self->epoll_fd);	
	/*ɾ��ringbuffer�ͷ��ڴ�*/ 
	_release_rb(self->rb);
	/*ɾ��hash���ͷ��ڴ�*/ 
	map_delete(self->socket_hash);
	/*�ͷŹ���ṹ��Ԫ*/ 
	free(self);
	self = NULL; 
}
/**	@fn	static int _read_queue(MREAD_POLL * self, int timeout) 
 *	@brief	��ȡע����epollfd�ϵ�socketfd���������� 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ 
 *	@param timeout -1:��Զ�ȴ�;0:���ȴ�;>0:��ʾ�ȴ�ʱ�� 
 *	@return	��������
 */
static int _read_queue(MREAD_POLL * self, int timeout) 
{
	self->queue_head = 0;
	/*��ѯI/O����Ĳ��������ΪREADQUEUE*/
	int n = epoll_wait(self->epoll_fd , self->ev, READQUEUE, timeout);
	if (n == -1) 
	{
		self->queue_len = 0;
		return -1;
	}
	self->queue_len = n;
	return n;
}
/**	@fn	inline static int _read_one(MREAD_POLL * self)  
 *	@brief	��ȡע����epollfd�ϵ���������¼���socketfd 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ 
 *	@return	�����׽��־�� 
 */
inline static int _read_one(MREAD_POLL * self) 
{
	/*������*/
	if(NULL == self)
	{
		return -1;
	} 
	if (self->queue_head >= self->queue_len) 
	{
		return -1;
	}
	return self->ev[self->queue_head ++].data.fd;
}
/**	@fn	static SOCKET * _alloc_socket(MREAD_POLL * self)   
 *	@brief	������õ�SOCKET�ṹ��������׽��� 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ 
 *	@return	���õ�SOCKET�ṹ��ָ�� 
 */
static SOCKET * _alloc_socket(MREAD_POLL * self) 
{
	/*������*/
	if(NULL == self)
	{
		return NULL;
	}
	/*û�п��õ�SOCKET�ṹ��*/ 
	if (NULL == self->free_socket) 
	{
		return NULL;
	}
	SOCKET * sock = self->free_socket;
	int next_free = sock->fd;/*ÿһ���յ�SOCKET�ṹ���fd���Ǽ�¼��һ���յ�SOCKET�ı��*/ 
	if (next_free < 0 ) 
	{
		self->free_socket = NULL;
	} 
	else 
	{
		self->free_socket = &self->sockets[next_free];/*ָ������*/ 
	}
	return sock;
}
/**	@fn	static void _add_client(MREAD_POLL * self, int fd)   
 *	@brief	��ӿͻ���Ϊ�ͻ��˷�����Ӧ����Դ 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ
 *	@param fd	�����׽��־�� 
 *	@return	N\A 
 */
static void _add_client(MREAD_POLL * self, int fd) 
{
	/*������*/
	if(NULL == self)
	{
		return ;
	}
	/*����SOCKET�ṹ����Դ*/
	SOCKET * sock = _alloc_socket(self);
	if (sock == NULL) 
	{
		close(fd);
		return;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	/*Ϊ�ͻ��˵������׽���ע������*/ 
	if (epoll_ctl(self->epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) 
	{
		close(fd);
		return;
	}
	
	sock->fd = fd;
	sock->node = NULL;
	sock->status = SOCKET_SUSPEND;
	int id = sock - self->sockets;
	/*��hash���в���fd*/ 
	map_insert(self->socket_hash , fd , id);
}
/**	@fn	static int _report_closed(MREAD_POLL * self) 
 *	@brief	�ͻ���socket�ر� 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ 
 *	@return	�رյĿͻ���id 
 */
static int _report_closed(MREAD_POLL * self) 
{
	/*������*/
	if(NULL == self)
	{
		return -1;
	}
	int i;
	for (i = 0;i < self->max_connection;i++) 
	{
		if (self->sockets[i].status == SOCKET_CLOSED) 
		{
			self->active = i;
			return i;
		}
	}
	assert(0);
	return -1;
}

/**	@fn	int mread_poll(MREAD_POLL * self , int timeout)   
 *	@brief	Ѱ�ҵ�ǰ��Ҫ����Ŀͻ���id 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ
 *	@param timeout -1:��Զ�ȴ�;0:���ȴ�;>0:��ʾ�ȴ�ʱ��
 *	@return	�ͻ���id���ĸ������׽��ֿ��Զ���SOCKET[i]���� 
 */
int mread_poll(MREAD_POLL * self , int timeout) 
{
	/*������*/
	if(NULL == self)
	{
		return -1;
	}
	/*ringbuffer���ݿ����ƫ����0��mread_yield���Ѿ������������ɾ����*/
	self->skip = 0;
	/*�ж��Ƿ����µĿͻ���*/ 
	if (self->active >= 0) 
	{
		SOCKET * sock = &self->sockets[self->active];
		if (sock->status == SOCKET_READ) 
		{
			return self->active;
		}
	}
	/*�ж��Ƿ���Ҫ�رմ˿ͻ���*/ 
	if (self->closed > 0 ) 
	{
		return _report_closed(self);
	}
	/*�鿴�Ƿ������¼�����*/ 
	if (self->queue_head >= self->queue_len) 
	{
		if (_read_queue(self, timeout) == -1) 
		{
			self->active = -1;
			return -1;
		}
	}
	for (;;) 
	{
		/*��ȡ�¼�*/ 
		int fd = _read_one(self);
		if (fd == -1) 
		{
			self->active = -1;
			return -1;
		}
		/*��ʾ���µĿͻ�������*/ 
		if (fd == self->listen_fd) 
		{
			struct sockaddr_in remote_addr;
			socklen_t len = sizeof(struct sockaddr_in);
			int client_fd = accept(self->listen_fd , (struct sockaddr *)&remote_addr ,  &len);
			if (client_fd >= 0) 
			{
				pthread_mutex_lock(&prin_mutex);	
				printf("MREAD connect %s:%u (fd=%d)\n",inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port), client_fd);
				printf("\033[1A");
				pthread_mutex_unlock(&prin_mutex);
				/*��ӿͻ���*/ 
				_add_client(self, client_fd);
				pthread_rwlock_rdlock(&sock_mutex);
				self->pthread_print = self->pthread_print + 1;
				pthread_rwlock_unlock(&sock_mutex);

			}
		} 
		else 
		{
			/*����ǾɵĿͻ��˾���hash���и���fdѰ�Ҵ˿ͻ���id����SOCKET[i]��*/
			int index = map_search(self->socket_hash , fd);
			if (index >= 0) 
			{
				self->active = index;
				SOCKET * sock = &self->sockets[index];
				sock->status = SOCKET_POLLIN;
				return index;
			}
		}
	}
}
/**	@fn	int mread_socket(MREAD_POLL * self, int index)    
 *	@brief	���ݿͻ���id����sockfd 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ
 *	@param index �ͻ���id 
 *	@return	�����׽��� 
 */
int mread_socket(MREAD_POLL * self, int index) 
{
	/*������*/
	if(NULL == self)
	{
		return -1;
	}
	return self->sockets[index].fd;
}

/**	@fn	static void _link_node(RINGBUFFER * rb, int id, SOCKET * sock , RBUF_BLOCK * blk) 
 *	@brief	������ͬ�ͻ��˲�ͬʱ�ڷ����������� 
 *	@param rb ringbufferָ�� 
 *	@param id �ͻ���id 
 *	@return	N\A 
 */
static void _link_node(RINGBUFFER * rb, int id, SOCKET * sock , RBUF_BLOCK * blk) 
{
	/*������*/
	if((NULL == rb) && (NULL == sock))
	{
		return;
	}
	if (sock->node) 
	{
		/*����Ѿ�������������*/
		ringbuffer_link(rb, sock->node , blk);	
	} 
	else 
	{
		/*���û��������sock->nodeָ������ݿ�*/
		blk->id = id;
		sock->node = blk;
	}
}
/**	@fn	void _close_client(MREAD_POLL * self, int id) 
 *	@brief	�رտͻ��� 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ
 *	@param id �ͻ���id 
 *	@return	N\A 
 */
void _close_client(MREAD_POLL * self, int id) 
{
	/*������*/
	if(NULL == self)
	{
		return;
	}
	SOCKET * sock = &self->sockets[id];
	/*��ʼ����ӦSOCKET�ṹ��*/ 
	sock->status = SOCKET_CLOSED;
	sock->node = NULL;
	sock->temp = NULL;

	sock->recev_filesize = 0;
	sock->filesize_total = 0;
	sock->send_ready_filesize = 0;
	/*�ÿͻ����̱߳�־0�Ա㰲ȫ�˳��߳�*/ 
	sock->pthread_flag = 0;
	/*�ر���Ӧ�������׽���*/ 
	close(sock->fd);
	pthread_mutex_lock(&prin_mutex);
	printf("\nMREAD close %d (fd=%d)\n",id,sock->fd);
	printf("\033[2A");
	pthread_mutex_unlock(&prin_mutex);
	/*ɾ������*/ 
	epoll_ctl(self->epoll_fd, EPOLL_CTL_DEL, sock->fd , NULL);
	++self->closed;
	/*�ͻ�������-1*/
	pthread_rwlock_rdlock(&sock_mutex);
	self->pthread_print = self->pthread_print - 1;
	pthread_rwlock_unlock(&sock_mutex);

}
/**	@fn	void _close_active(MREAD_POLL * self) 
 *	@brief	�ҵ���Ҫ�رյĿͻ���Ȼ��رտͻ��� 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ
 *	@return	N\A 
 */
void _close_active(MREAD_POLL * self) 
{
	/*������*/
	if(NULL == self)
	{
		return;
	}
	int id = self->active;
	SOCKET * sock = &self->sockets[id];
	/*�ͷŴ˿ͻ����߳�ռ�õ���Դ*/ 
	ringbuffer_free(self->rb, sock->temp);
	ringbuffer_free(self->rb, sock->node);
	_close_client(self, id);
}
/**	@fn	static char * _ringbuffer_read(MREAD_POLL * self, int *size)  
 *	@brief	����Ӧ��ringbuffer block�ж�ȡsize�ֽڸ����ݡ� 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ
 *	@param size ��ȡ���ֽ��� 
 *	@return	Ҫô����NULL��Ҫô���ش��size���ֽڵ����ݵĵ�ַָ�� 
 */
static char * _ringbuffer_read(MREAD_POLL * self, int *size) 
{
	/*������*/
	if(NULL == self)
	{
		return NULL;
	}
	SOCKET * sock = &self->sockets[self->active];
	if (sock->node == NULL) 
	{
		*size = 0;
		return NULL;
	}
	int sz = *size;
	void * ret;
	/*��ringbuffer ���ݿ��ж�ȡsize�ֽڵ�����*/ 
	*size = ringbuffer_data(self->rb, sock->node, sz , self->skip, &ret);
	return ret;
}


/**	@fn	void * mread_pull(MREAD_POLL * self , int size)  
 *	@brief	�ӵ�ǰ�Ŀͻ��˶�Ӧ��ringbuffer���ݿ��ж�ȡԭ�ӵĶ�ȡsize�ֽڸ����� 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ
 *	@param size ��ȡ���ֽ��� 
 *	@return	Ҫô����NULL��Ҫô���ش��size���ֽڵ����ݵĵ�ַָ�� 
 */
void * mread_pull(MREAD_POLL * self , int size) 
{
	/*������*/
	if(NULL == self)
	{
		return NULL;
	}
	/*��ʾ��ǰû�пͻ���*/ 
	if (self->active == -1) 
	{
		return NULL;
	}
	SOCKET *sock = &self->sockets[self->active];
	int rd_size = size;
	/*����Ӧ��ringbuffer block�ж�ȡsize�ֽڸ�����*/
	char * buffer = _ringbuffer_read(self, &rd_size);
	if (buffer) 
	{
		self->skip += size;
		return buffer;
	}
	/*���sockfd�رգ��ͷ���ռ�е����ݿ�*/
	if (sock->status == SOCKET_CLOSED) 
	{
		ringbuffer_free(self->rb , sock->node);
		sock->node = NULL;
		return NULL;
	}
	/*sock״̬Ϊ�ɶ������ݲ���*/ 
	if (sock->status == SOCKET_READ) 
	{
		sock->status = SOCKET_SUSPEND;
		return NULL;
	}
	/*�ж�sock״̬�Ƿ�Ϊд��״̬*/
	assert(sock->status == SOCKET_POLLIN);
	int sz = size - rd_size;
	int rd = READBLOCKSIZE;
	if (rd < sz) 
	{
		rd = sz;
	}

	int id = self->active;
	RINGBUFFER * rb = self->rb;
	/*һ�η����������60*/ 
	RBUF_BLOCK * blk = ringbuffer_alloc(rb , rd);
	while (blk == NULL) 
	{
		/*����ʧ�ܣ���ر����ȿ����Ŀͻ���*/ 
		int collect_id = ringbuffer_collect(rb);
		_close_client(self , collect_id);
		if (id == collect_id) 
		{
			return NULL;
		}
		/*�����ռ�*/
		blk = ringbuffer_alloc(rb , rd);
	}

	buffer = (char *)(blk + 1);

	for (;;) 
	{
		int bytes = recv(sock->fd, buffer, rd, MSG_DONTWAIT); 
		if (bytes > 0)
		 {
		 	/*�յ����ݲ��������ݿ��С*/ 
			ringbuffer_resize(rb, blk , bytes);
			if (bytes < sz) 
			{
				/*����յ������ݼ���֮ǰ������ringbuffer�ж�ȡ������֮�ͻ�����Ҫ��ȡ������ʱ*/ 
				_link_node(rb, self->active, sock , blk);
				/*sock��Ϊ����״̬���ȴ���һ��ȡ����*/ 
				sock->status = SOCKET_SUSPEND;
				return NULL;
			}
			/*��ʾ�Ѿ�����������*/ 
			sock->status = SOCKET_READ;
			break;
		}
		if (bytes == 0) 
		{
			_close_active(self);
			return NULL;
		}
		if (bytes == -1) 
		{
			switch(errno) 
			{
			case EWOULDBLOCK:/*�ж�*/ 
				return NULL;
			default:
				if (errno == EAGAIN)/*��ʾ��Ҫrelloc����Ϊ�Է�Ҫ���͵����ݴ�����Ļ�����ʣ����*/ 
				{
					continue;
				}
				_close_active(self);
				return NULL;
			}
		}
	}
	/*�����վݺ��������*/ 
	_link_node(rb, self->active , sock , blk);
	void * ret;
	/*���¶�ȡ����*/
	int real_rd = ringbuffer_data(rb, sock->node , size , self->skip, &ret);
	/*���ݿ�����*/ 
	if (ret) 
	{
		self->skip += size;
		return ret;
	}
	/*���ݿ鲻����*/
	assert(real_rd == size);
	RBUF_BLOCK * temp = ringbuffer_alloc(rb, size);
	while (temp == NULL) 
	{
		int collect_id = ringbuffer_collect(rb);
		if (id == collect_id) 
		{
			return NULL;
		}
		temp = ringbuffer_alloc(rb , size);
	}
	temp->id = id;
	if (sock->temp) 
	{
		ringbuffer_link(rb, temp, sock->temp);
	}
	sock->temp = temp;
	/*���ݲ�������copy��������*/ 
	ret = ringbuffer_copy(rb, sock->node, self->skip, temp);
	assert(ret);
	self->skip += size;
	return ret;
}


/**	@fn	void  mread_yield(MREAD_POLL * self) 
 *	@brief	���Ѿ���ȡ������ɾ�� 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ 
 *	@return	N\A
 */
void  mread_yield(MREAD_POLL * self) 
{
	/*������*/
	if(NULL == self)
	{
		return;
	}
	/*��ǰû�пͻ���*/ 
	if (self->active == -1) 
	{
		return;
	}
	SOCKET *sock = &self->sockets[self->active];
	/*�ͷ�����ringbuffer�п������ݵĿ�*/ 
	ringbuffer_free(self->rb , sock->temp);
	sock->temp = NULL;
	if (sock->status == SOCKET_CLOSED && sock->node == NULL) 
	{
		--self->closed;
		sock->status = SOCKET_INVALID;
		/*��hash����ɾ����Ϣ*/ 
		map_erase(self->socket_hash , sock->fd);
		/*�ٴν�������*/ 
		sock->fd = self->free_socket - self->sockets;
		self->free_socket = sock;
		self->skip = 0;
		self->active = -1;
	} 
	else 
	{
		if (sock->node) 
		{
			/*ɾ�����ݿ����Ѿ���ȡ������*/
			sock->node = ringbuffer_yield(self->rb, sock->node, self->skip);
		}
		self->skip = 0;
		if (sock->node == NULL) 
		{
			self->active = -1;
		}
	}
}

// When the id is closed, it returns 1
/**	@fnint  mread_closed(MREAD_POLL * self) 
 *	@brief	�鿴�ͻ����Ƿ�ر����ر��ͷ���Դ 
 *	@param self ��¼poll�����Ϣ�Ĺ������ݵ�Ԫ 
 *	@return	1��ʾ�ͻ��˹رգ�0��ʾ�ͻ���λ�رա� 
 */
int  mread_closed(MREAD_POLL * self) 
{
	/*������*/
	if(NULL == self)
	{
		return -1;
	}
	if (self->active == -1) 
	{
		return 0;
	}
	SOCKET * sock = &self->sockets[self->active];
	if (sock->status == SOCKET_CLOSED && sock->node == NULL) 
	{
		mread_yield(self);
		return 1;
	}
	return 0;
}
