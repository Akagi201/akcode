#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/cdev.h>
#include <linux/kfifo.h>

#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */
#include "scull_kfifo.h"		/* local definitions */

/*
 * Our parameters which can be set at load time.
 */
int scull_kfifo_major =   0;
int scull_kfifo_minor =   0;


module_param(scull_kfifo_major, int, S_IRUGO);
module_param(scull_kfifo_minor, int, S_IRUGO);

struct scull_kfifo *scull_kfifo_devices;	/* allocated in scull_kfifo_init_module */
unsigned char *tekkamanbuffer;
unsigned char *tekkaman;

/*
 * Open and close
 */

int scull_kfifo_open(struct inode *inode, struct file *filp)
{
	struct scull_kfifo *dev; /* device information */

	dev = container_of(inode->i_cdev, struct scull_kfifo, cdev);
	filp->private_data = dev; /* for other methods */

	return nonseekable_open(inode, filp);          /* success */
}

int scull_kfifo_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/*
 * Data management: read and write
 */

ssize_t scull_kfifo_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
	struct scull_kfifo *dev = filp->private_data; 
	ssize_t retval = 0;


	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	while (!kfifo_len(dev->tekkamankfifo)) { /* nothing to read */
		up(&dev->sem); /* release the lock */
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		printk("\"%s\" reading: going to sleep\n", current->comm);
		if (wait_event_interruptible(dev->inq, kfifo_len(dev->tekkamankfifo)))
			return -ERESTARTSYS; /* signal: tell the fs layer to handle it */
		/* otherwise loop, but first reacquire the lock */
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;
	}

	if (count > kfifo_len(dev->tekkamankfifo))
		count = kfifo_len(dev->tekkamankfifo);
	count = kfifo_get(dev->tekkamankfifo,tekkaman, count);

	if (copy_to_user(buf, tekkaman, count)) {
		retval = -EFAULT;
		goto out;
	}
	retval = count;

  out:
	up(&dev->sem);
	wake_up_interruptible(&dev->outq);
	if (printk_ratelimit()) 	printk("\"%s\" did read %li bytes\n",current->comm, (long)retval);
	return retval;
}

ssize_t scull_kfifo_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
	struct scull_kfifo *dev = filp->private_data;
	ssize_t retval = -ENOMEM; /* value used in "goto out" statements */

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	if ( count>BUFSIZE ) count = BUFSIZE;
	if (copy_from_user(tekkaman, buf, count)) {
		up (&dev->sem);
		return -EFAULT;
	}
	
	count = kfifo_put(dev->tekkamankfifo,tekkaman, count);
	retval = count;

	up(&dev->sem);
	wake_up_interruptible(&dev->inq);  /* blocked in read() and select() */
	if (printk_ratelimit()) 	printk("\"%s\" did write %li bytes\n",current->comm, (long)count);
	return retval;
}

int scull_kfifo_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
	int tekkaman = 0;
	/*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
	if (_IOC_TYPE(cmd) != SCULL_KFIFO_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > SCULL_KFIFO_MAXNR) return -ENOTTY;


	switch(cmd) {
        /*
         * The following two change the buffer size for scullpipe.
         * The scullpipe device uses this same ioctl method, just to
         * write less code. Actually, it's the same driver, isn't it?
         */
	  case SCULL_KFIFO_SIZE:
		tekkaman = (int) kfifo_len(scull_kfifo_devices->tekkamankfifo);
		break;

	  case SCULL_KFIFO_RESET:
		kfifo_reset(scull_kfifo_devices->tekkamankfifo);
		break;
	  default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	return tekkaman;

}


struct file_operations scull_kfifo_fops = {
	.owner =    THIS_MODULE,
	.read =     scull_kfifo_read,
	.write =    scull_kfifo_write,
	.open =     scull_kfifo_open,
	.release =  scull_kfifo_release,
	.llseek =		no_llseek,
	.ioctl =		scull_kfifo_ioctl,


};

/*
 * Finally, the module stuff
 */

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void scull_kfifo_cleanup_module(void)
{
	dev_t devno = MKDEV(scull_kfifo_major, scull_kfifo_minor);
	int keke = 0xff;

	/* Get rid of our char dev entries */
	if (scull_kfifo_devices) {
		if (scull_kfifo_devices->tekkamankfifo) 	{		kfifo_free(scull_kfifo_devices->tekkamankfifo);
		//kfree(scull_kfifo_devices->tekkamankfifo);
		keke = 0;
		}
		cdev_del(&scull_kfifo_devices->cdev);
		kfree(scull_kfifo_devices);
	}
#if 1
	if (tekkamanbuffer && keke) kfree(tekkamanbuffer);
#endif
	if (tekkaman) 	kfree(tekkaman);
	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, 1);
}


/*
 * Set up the char_dev structure for this device.
 */
static void scull_kfifo_setup_cdev(struct scull_kfifo *dev)
{
	int err, devno = MKDEV(scull_kfifo_major, scull_kfifo_minor );
    
	cdev_init(&dev->cdev, &scull_kfifo_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add (&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding scull_kfifo", err);
}


int scull_kfifo_init_module(void)
{
	int result;
	dev_t dev = 0;

/*
 * Get a range of minor numbers to work with, asking for a dynamic
 * major unless directed otherwise at load time.
 */
	if (scull_kfifo_major) {
		dev = MKDEV(scull_kfifo_major, scull_kfifo_minor);
		result = register_chrdev_region(dev, 1, "scull_kfifo");
	} else {
		result = alloc_chrdev_region(&dev, scull_kfifo_minor, 1,
				"scull_kfifo");
		scull_kfifo_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "scull: can't get major %d\n", scull_kfifo_major);
		return result;
	}

        /* 
	 * allocate the devices -- we can't have them static, as the number
	 * can be specified at load time
	 */
	scull_kfifo_devices = kmalloc( sizeof(struct scull_kfifo), GFP_KERNEL);
	if (!scull_kfifo_devices) {
		result = -ENOMEM;
		goto fail;  /* Make this more graceful */
	}
	memset(scull_kfifo_devices, 0, sizeof(struct scull_kfifo));

	tekkamanbuffer = kmalloc( BUFSIZE, GFP_KERNEL);
	if (!tekkamanbuffer) {
		result = -ENOMEM;
		goto fail;  /* Make this more graceful */
	}
	tekkaman = kmalloc( BUFSIZE, GFP_KERNEL);
	if (!tekkaman) {
		result = -ENOMEM;
		goto fail;  /* Make this more graceful */
	}

        /* Initialize each device. */
	init_MUTEX(&scull_kfifo_devices->sem);
	spin_lock_init (&scull_kfifo_devices->lock);
	scull_kfifo_devices->tekkamankfifo = kfifo_init(tekkamanbuffer,  BUFSIZE ,GFP_KERNEL, &scull_kfifo_devices->lock);
		init_waitqueue_head(&scull_kfifo_devices->inq);
		init_waitqueue_head(&scull_kfifo_devices->outq);	
	scull_kfifo_setup_cdev(scull_kfifo_devices);

	return 0; /* succeed */

  fail:
	scull_kfifo_cleanup_module();
	return result;
}

module_init(scull_kfifo_init_module);
module_exit(scull_kfifo_cleanup_module);

MODULE_AUTHOR("Tekkamanninja");
MODULE_LICENSE("Dual BSD/GPL");
