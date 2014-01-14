/*
 * scull.h -- definitions for the char module
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 * $Id: scull.h,v 1.15 2004/11/04 17:51:18 rubini Exp $
 */

#ifndef _SCULL_H_
#define _SCULL_H_
#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */


struct scull_kfifo {
//	unsigned int access_key;  /* used by sculluid and scullpriv */
	struct semaphore sem;     /* mutual exclusion semaphore     */
	struct cdev cdev;	  /* Char device structure		*/
	spinlock_t lock;
	struct kfifo *tekkamankfifo;
        wait_queue_head_t inq, outq;       /* read and write queues */

	
};

#define  BUFSIZE		64

/*
 * Split minors in two parts
 */
#define TYPE(minor)	(((minor) >> 4) & 0xf)	/* high nibble */
#define NUM(minor)	((minor) & 0xf)		/* low  nibble */


/*
 * The different configurable parameters
 */
extern int scull_major;     /* main.c */


/*
 * Prototypes for shared functions
 */


ssize_t scull_read(struct file *filp, char __user *buf, size_t count,
                   loff_t *f_pos);
ssize_t scull_write(struct file *filp, const char __user *buf, size_t count,
                    loff_t *f_pos);
/*
 * Ioctl definitions
 */

/* Use 'k' as magic number */
#define SCULL_KFIFO_MAGIC  'k'
/* Please use a different 8-bit number in your code */

#define SCULL_KFIFO_SIZE  _IO(SCULL_KFIFO_MAGIC,   0)
#define SCULL_KFIFO_RESET _IO(SCULL_KFIFO_MAGIC,   1)
/* ... more to come */

#define SCULL_KFIFO_MAXNR 1



#endif /* _SCULL_H_ */
