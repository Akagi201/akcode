      
/** @file hello_dev.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    "Hello, world!" minimal kernel module - /dev version
  * 
  * @author   liuboyf1
  * @date     2012-9-28
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("liuboyf1");
MODULE_DESCRIPTION("\"Hello, world!\" minimal module");
MODULE_VERSION("dev");
MODULE_SUPPORTED_DEVICE("hello_dev")

/*
 * hello_read is the function called when a process calls read() on
 * /dev/hello.  It writes "Hello, world!" to the buffer passed in the
 * read() call.
 */
static ssize_t hello_read(struct file * file, char * buf, size_t count, loff_t *ppos)
{
	char *hello_str = "Hello, world!\n";
	int len = strlen(hello_str); /* Don't include the null byte. */
	/*
	 * We only support reading the whole string at once.
	 */
	if (count < len)
	{
		return -EINVAL;
	}
	/*
	 * If file position is non-zero, then assume the string has
	 * been read and indicate there is no more data to be read.
	 */
	if (*ppos != 0)
	{
		return 0;
	}
	/*
	 * Besides copying the string to the user provided buffer,
	 * this function also checks that the user has permission to
	 * write to the buffer, that it is mapped, etc.
	 */
	if (copy_to_user(buf, hello_str, len))
	{
		return -EINVAL;
	}
	/*
	 * Tell the user how much data we wrote.
	 */
	*ppos = len;

	return len;
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations hello_fops = {
	.owner		= THIS_MODULE,
	.read		= hello_read,
};

static struct miscdevice hello_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/hello.
	 */
	"hello_dev",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&hello_fops
};

static int __init hello_init(void)
{
	int ret;

	/*
	 * Create the "hello" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/hello device using
	 * the default rules.
	 */
	ret = misc_register(&hello_dev);
	if (ret)
	{
		printk(KERN_EMERG "Unable to register \"Hello, world!\" misc device\n");
	}

	return ret;
}

static void __exit hello_exit(void)
{
	misc_deregister(&hello_dev);
}

module_init(hello_init);
module_exit(hello_exit);


