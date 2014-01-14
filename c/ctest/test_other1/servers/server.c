/**	@file server.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 服务器API函数 
 *
 *	@author		zhoualei
 *	@date		2012/09/08
 *
 *	@note 服务器管理内存及创建的API函数 
 *	@note 多进度条打印实现多写一读的互斥读写数据块 
 */
#include "server.h"

/**	@fn	static SOCKET *_create_sockets(int max)
 *	@brief	创建和初始化SOCKET结构体数组 
 *	@param max 结构体数量 
 *	@return	指向该结构体数组的指针 
 */
static SOCKET *_create_sockets(int max) 
{
	int i;
	/*为SOCKET结构体数组分配空间*/ 
	SOCKET * sock = malloc(max * sizeof(SOCKET));
	if(NULL == sock)
	{
		perror("malloc");	
	} 
	/*SOCKET结构体数组初始化*/
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
	sock[max-1].fd = -1;/*标识最后个结构体数组*/ 
	return sock;
}

/**	@fn	static RINGBUFFER *_create_rb(int size)
 *	@brief	创建和初始化ringbuffer 
 *	@param size ringbuffer的大小（若为0则默认为1K） 
 *	@return	指向该ringbuffer的指针 
 */
static RINGBUFFER *_create_rb(int size) 
{
	/*4字节对齐*/ 
	size = (size + 3) & ~3;
	/*创建2倍用于copy*/ 
	if (size < READBLOCKSIZE * 2) 
	{
		size = READBLOCKSIZE * 2;
	}
	/*创建初始化ringbuffer*/
	RINGBUFFER * rb = ringbuffer_new(size);

	return rb;
}
/**	@fn	static void _release_rb(RINGBUFFER * rb)
 *	@brief	删除ringbuffer 
 *	@param rb  指向该ringbuffer的指针 
 *	@return	N\A
 */ 
static void _release_rb(RINGBUFFER * rb) 
{
	ringbuffer_delete(rb);
}

/**	@fn	MREAD_POLL * mread_create(int port , int max , int buffer_size) 
 *	@brief	创建poll，监听端口，设置最大连接数，初始化ringbuffer、map、SOCKET结构体数组 
 *	@param max 最大连接数 
 *	@param buffer_size ringbuffer的大小 
 *	@return	指向MREAD_POLL结构体的指针 
 */
MREAD_POLL * mread_create(int port , int max , int buffer_size) 
{
	int listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (listen_fd == -1) 
	{
		return NULL;
	}
	int reuse = 1;
	/*设置套接口属性*/ 
	/*设置调用closesocket()后,仍可继续重用该socket。调用closesocket()一般不会立即关闭socket，而经历TIME_WAIT的过程*/
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
	
	struct sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_LOOPBACK;
	pthread_mutex_lock(&prin_mutex);
	printf("MREAD bind %s:%u\n",inet_ntoa(my_addr.sin_addr),ntohs(my_addr.sin_port));
	pthread_mutex_unlock(&prin_mutex);
	/*绑定*/
	if (bind(listen_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		close(listen_fd);
		return NULL;
	}
	/*监听*/ 
	if (listen(listen_fd, BACKLOG) == -1) 
	{
		close(listen_fd);
		return NULL;
	}
	/*生成一个epoll专用的文件描述符，最多能存放max+1个socket fd*/ 
	int epoll_fd = epoll_create(max + 1);
	if (epoll_fd == -1) 
	{
		close(listen_fd);
		return NULL;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN;/*对应的文件描述符可以读*/ 
	ev.data.fd = listen_fd;/*设置与要处理的事物相干的文件描述符*/ 
	/*注册epoll事件*/ 
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) 
	{
		close(listen_fd);
		close(epoll_fd);
		return NULL;
	}
	/*创建和初始化共享数据块*/ 
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
 *	@brief	删除poll 
 *	@param self 记录poll相关信息的共享数据单元 
 *	@return	N\A
 */
void mread_close(MREAD_POLL *self) 
{
	/*检查参数*/ 
	if (self == NULL)
	{
		return;
	}
	int i;
	self->pthread_print = 0;
	SOCKET * sock = self->sockets;
	/*关闭所有的连接套接字*/ 
	for (i=0;i<self->max_connection;i++) 
	{
		if (sock[i].status != SOCKET_INVALID) 
		{
			close(sock[i].fd);
		}
	}
	free(sock);
	/*关闭监听套接字*/ 
	if (self->listen_fd >= 0) 
	{
		close(self->listen_fd);
	}
	close(self->epoll_fd);	
	/*删除ringbuffer释放内存*/ 
	_release_rb(self->rb);
	/*删除hash表释放内存*/ 
	map_delete(self->socket_hash);
	/*释放共享结构单元*/ 
	free(self);
	self = NULL; 
}
/**	@fn	static int _read_queue(MREAD_POLL * self, int timeout) 
 *	@brief	获取注册在epollfd上的socketfd的事务数量 
 *	@param self 记录poll相关信息的共享数据单元 
 *	@param timeout -1:永远等待;0:不等待;>0:表示等待时间 
 *	@return	事务数量
 */
static int _read_queue(MREAD_POLL * self, int timeout) 
{
	self->queue_head = 0;
	/*轮询I/O事物的产生，最大为READQUEUE*/
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
 *	@brief	读取注册在epollfd上的最早产生事件的socketfd 
 *	@param self 记录poll相关信息的共享数据单元 
 *	@return	连接套接字句柄 
 */
inline static int _read_one(MREAD_POLL * self) 
{
	/*检查参数*/
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
 *	@brief	分配可用的SOCKET结构体给连接套接字 
 *	@param self 记录poll相关信息的共享数据单元 
 *	@return	可用的SOCKET结构体指针 
 */
static SOCKET * _alloc_socket(MREAD_POLL * self) 
{
	/*检查参数*/
	if(NULL == self)
	{
		return NULL;
	}
	/*没有可用的SOCKET结构体*/ 
	if (NULL == self->free_socket) 
	{
		return NULL;
	}
	SOCKET * sock = self->free_socket;
	int next_free = sock->fd;/*每一个空的SOCKET结构体的fd都是记录下一个空的SOCKET的编号*/ 
	if (next_free < 0 ) 
	{
		self->free_socket = NULL;
	} 
	else 
	{
		self->free_socket = &self->sockets[next_free];/*指针下移*/ 
	}
	return sock;
}
/**	@fn	static void _add_client(MREAD_POLL * self, int fd)   
 *	@brief	添加客户端为客户端分配相应的资源 
 *	@param self 记录poll相关信息的共享数据单元
 *	@param fd	连接套接字句柄 
 *	@return	N\A 
 */
static void _add_client(MREAD_POLL * self, int fd) 
{
	/*检查参数*/
	if(NULL == self)
	{
		return ;
	}
	/*分配SOCKET结构体资源*/
	SOCKET * sock = _alloc_socket(self);
	if (sock == NULL) 
	{
		close(fd);
		return;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	/*为客户端的连接套接字注册事物*/ 
	if (epoll_ctl(self->epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) 
	{
		close(fd);
		return;
	}
	
	sock->fd = fd;
	sock->node = NULL;
	sock->status = SOCKET_SUSPEND;
	int id = sock - self->sockets;
	/*在hash表中插入fd*/ 
	map_insert(self->socket_hash , fd , id);
}
/**	@fn	static int _report_closed(MREAD_POLL * self) 
 *	@brief	客户端socket关闭 
 *	@param self 记录poll相关信息的共享数据单元 
 *	@return	关闭的客户端id 
 */
static int _report_closed(MREAD_POLL * self) 
{
	/*检查参数*/
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
 *	@brief	寻找当前需要处理的客户端id 
 *	@param self 记录poll相关信息的共享数据单元
 *	@param timeout -1:永远等待;0:不等待;>0:表示等待时间
 *	@return	客户端id即哪个连接套接字可以读（SOCKET[i]）。 
 */
int mread_poll(MREAD_POLL * self , int timeout) 
{
	/*检查参数*/
	if(NULL == self)
	{
		return -1;
	}
	/*ringbuffer数据块块内偏移置0，mread_yield将已经处理过的数据删除了*/
	self->skip = 0;
	/*判断是否是新的客户端*/ 
	if (self->active >= 0) 
	{
		SOCKET * sock = &self->sockets[self->active];
		if (sock->status == SOCKET_READ) 
		{
			return self->active;
		}
	}
	/*判断是否需要关闭此客户端*/ 
	if (self->closed > 0 ) 
	{
		return _report_closed(self);
	}
	/*查看是否有新事件产生*/ 
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
		/*读取事件*/ 
		int fd = _read_one(self);
		if (fd == -1) 
		{
			self->active = -1;
			return -1;
		}
		/*表示是新的客户端连接*/ 
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
				/*添加客户端*/ 
				_add_client(self, client_fd);
				pthread_rwlock_rdlock(&sock_mutex);
				self->pthread_print = self->pthread_print + 1;
				pthread_rwlock_unlock(&sock_mutex);

			}
		} 
		else 
		{
			/*如果是旧的客户端就在hash表中根据fd寻找此客户端id（即SOCKET[i]）*/
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
 *	@brief	根据客户端id查找sockfd 
 *	@param self 记录poll相关信息的共享数据单元
 *	@param index 客户端id 
 *	@return	连接套接字 
 */
int mread_socket(MREAD_POLL * self, int index) 
{
	/*检查参数*/
	if(NULL == self)
	{
		return -1;
	}
	return self->sockets[index].fd;
}

/**	@fn	static void _link_node(RINGBUFFER * rb, int id, SOCKET * sock , RBUF_BLOCK * blk) 
 *	@brief	连接相同客户端不同时期发来的数据流 
 *	@param rb ringbuffer指针 
 *	@param id 客户端id 
 *	@return	N\A 
 */
static void _link_node(RINGBUFFER * rb, int id, SOCKET * sock , RBUF_BLOCK * blk) 
{
	/*检查参数*/
	if((NULL == rb) && (NULL == sock))
	{
		return;
	}
	if (sock->node) 
	{
		/*如果已经有数据则连接*/
		ringbuffer_link(rb, sock->node , blk);	
	} 
	else 
	{
		/*如果没有数据则sock->node指向该数据块*/
		blk->id = id;
		sock->node = blk;
	}
}
/**	@fn	void _close_client(MREAD_POLL * self, int id) 
 *	@brief	关闭客户端 
 *	@param self 记录poll相关信息的共享数据单元
 *	@param id 客户端id 
 *	@return	N\A 
 */
void _close_client(MREAD_POLL * self, int id) 
{
	/*检查参数*/
	if(NULL == self)
	{
		return;
	}
	SOCKET * sock = &self->sockets[id];
	/*初始化对应SOCKET结构体*/ 
	sock->status = SOCKET_CLOSED;
	sock->node = NULL;
	sock->temp = NULL;

	sock->recev_filesize = 0;
	sock->filesize_total = 0;
	sock->send_ready_filesize = 0;
	/*置客户端线程标志0以便安全退出线程*/ 
	sock->pthread_flag = 0;
	/*关闭相应的连接套接字*/ 
	close(sock->fd);
	pthread_mutex_lock(&prin_mutex);
	printf("\nMREAD close %d (fd=%d)\n",id,sock->fd);
	printf("\033[2A");
	pthread_mutex_unlock(&prin_mutex);
	/*删除事物*/ 
	epoll_ctl(self->epoll_fd, EPOLL_CTL_DEL, sock->fd , NULL);
	++self->closed;
	/*客户端数量-1*/
	pthread_rwlock_rdlock(&sock_mutex);
	self->pthread_print = self->pthread_print - 1;
	pthread_rwlock_unlock(&sock_mutex);

}
/**	@fn	void _close_active(MREAD_POLL * self) 
 *	@brief	找到需要关闭的客户端然后关闭客户端 
 *	@param self 记录poll相关信息的共享数据单元
 *	@return	N\A 
 */
void _close_active(MREAD_POLL * self) 
{
	/*检查参数*/
	if(NULL == self)
	{
		return;
	}
	int id = self->active;
	SOCKET * sock = &self->sockets[id];
	/*释放此客户端线程占用的资源*/ 
	ringbuffer_free(self->rb, sock->temp);
	ringbuffer_free(self->rb, sock->node);
	_close_client(self, id);
}
/**	@fn	static char * _ringbuffer_read(MREAD_POLL * self, int *size)  
 *	@brief	从相应的ringbuffer block中读取size字节个数据。 
 *	@param self 记录poll相关信息的共享数据单元
 *	@param size 读取的字节数 
 *	@return	要么返回NULL，要么返回存放size个字节的数据的地址指针 
 */
static char * _ringbuffer_read(MREAD_POLL * self, int *size) 
{
	/*检查参数*/
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
	/*从ringbuffer 数据块中读取size字节的数据*/ 
	*size = ringbuffer_data(self->rb, sock->node, sz , self->skip, &ret);
	return ret;
}


/**	@fn	void * mread_pull(MREAD_POLL * self , int size)  
 *	@brief	从当前的客户端对应的ringbuffer数据块中读取原子的读取size字节个数据 
 *	@param self 记录poll相关信息的共享数据单元
 *	@param size 读取的字节数 
 *	@return	要么返回NULL，要么返回存放size个字节的数据的地址指针 
 */
void * mread_pull(MREAD_POLL * self , int size) 
{
	/*检查参数*/
	if(NULL == self)
	{
		return NULL;
	}
	/*表示当前没有客户端*/ 
	if (self->active == -1) 
	{
		return NULL;
	}
	SOCKET *sock = &self->sockets[self->active];
	int rd_size = size;
	/*从相应的ringbuffer block中读取size字节个数据*/
	char * buffer = _ringbuffer_read(self, &rd_size);
	if (buffer) 
	{
		self->skip += size;
		return buffer;
	}
	/*如果sockfd关闭，释放它占有的数据块*/
	if (sock->status == SOCKET_CLOSED) 
	{
		ringbuffer_free(self->rb , sock->node);
		sock->node = NULL;
		return NULL;
	}
	/*sock状态为可读但数据不够*/ 
	if (sock->status == SOCKET_READ) 
	{
		sock->status = SOCKET_SUSPEND;
		return NULL;
	}
	/*判断sock状态是否为写入状态*/
	assert(sock->status == SOCKET_POLLIN);
	int sz = size - rd_size;
	int rd = READBLOCKSIZE;
	if (rd < sz) 
	{
		rd = sz;
	}

	int id = self->active;
	RINGBUFFER * rb = self->rb;
	/*一次分配分配最少60*/ 
	RBUF_BLOCK * blk = ringbuffer_alloc(rb , rd);
	while (blk == NULL) 
	{
		/*分配失败，则关闭最先开启的客户端*/ 
		int collect_id = ringbuffer_collect(rb);
		_close_client(self , collect_id);
		if (id == collect_id) 
		{
			return NULL;
		}
		/*重新收集*/
		blk = ringbuffer_alloc(rb , rd);
	}

	buffer = (char *)(blk + 1);

	for (;;) 
	{
		int bytes = recv(sock->fd, buffer, rd, MSG_DONTWAIT); 
		if (bytes > 0)
		 {
		 	/*收到数据并调整数据块大小*/ 
			ringbuffer_resize(rb, blk , bytes);
			if (bytes < sz) 
			{
				/*如果收到的数据加上之前可以在ringbuffer中读取的数据之和还少于要读取的数据时*/ 
				_link_node(rb, self->active, sock , blk);
				/*sock置为挂起状态，等待下一收取数据*/ 
				sock->status = SOCKET_SUSPEND;
				return NULL;
			}
			/*表示已经收齐数据了*/ 
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
			case EWOULDBLOCK:/*中断*/ 
				return NULL;
			default:
				if (errno == EAGAIN)/*表示需要relloc。因为对方要发送的数据大于你的缓冲区剩余量*/ 
				{
					continue;
				}
				_close_active(self);
				return NULL;
			}
		}
	}
	/*收起收据后进行连接*/ 
	_link_node(rb, self->active , sock , blk);
	void * ret;
	/*重新读取数据*/
	int real_rd = ringbuffer_data(rb, sock->node , size , self->skip, &ret);
	/*数据块连续*/ 
	if (ret) 
	{
		self->skip += size;
		return ret;
	}
	/*数据块不连续*/
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
	/*数据不连续则copy到连续块*/ 
	ret = ringbuffer_copy(rb, sock->node, self->skip, temp);
	assert(ret);
	self->skip += size;
	return ret;
}


/**	@fn	void  mread_yield(MREAD_POLL * self) 
 *	@brief	将已经读取的数据删除 
 *	@param self 记录poll相关信息的共享数据单元 
 *	@return	N\A
 */
void  mread_yield(MREAD_POLL * self) 
{
	/*检查参数*/
	if(NULL == self)
	{
		return;
	}
	/*当前没有客户端*/ 
	if (self->active == -1) 
	{
		return;
	}
	SOCKET *sock = &self->sockets[self->active];
	/*释放用来ringbuffer中拷贝数据的块*/ 
	ringbuffer_free(self->rb , sock->temp);
	sock->temp = NULL;
	if (sock->status == SOCKET_CLOSED && sock->node == NULL) 
	{
		--self->closed;
		sock->status = SOCKET_INVALID;
		/*在hash表中删除信息*/ 
		map_erase(self->socket_hash , sock->fd);
		/*再次建立连接*/ 
		sock->fd = self->free_socket - self->sockets;
		self->free_socket = sock;
		self->skip = 0;
		self->active = -1;
	} 
	else 
	{
		if (sock->node) 
		{
			/*删除数据块中已经读取的数据*/
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
 *	@brief	查看客户端是否关闭若关闭释放资源 
 *	@param self 记录poll相关信息的共享数据单元 
 *	@return	1表示客户端关闭，0表示客户端位关闭。 
 */
int  mread_closed(MREAD_POLL * self) 
{
	/*检查参数*/
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
