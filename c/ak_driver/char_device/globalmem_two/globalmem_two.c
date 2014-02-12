      
/** @file globalmem_two.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief  A globalmem driver as an example of char device drivers
  * @brief  There are two same globalmems in this driver  
  * @brief  This example is to introduce the function of file->private_data
  * 
  * @author   liuboyf1
  * @date     2012-9-24
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#define GLOBALMEM_SIZE	0x1000	/*ȫ���ڴ����4K�ֽ�*/
#define MEM_CLEAR 0x1  /*��0ȫ���ڴ�*/
#define GLOBALMEM_MAJOR 249    /*Ԥ���globalmem�����豸��*/

static int globalmem_major = GLOBALMEM_MAJOR;
/*globalmem�豸�ṹ��*/
struct globalmem_dev                                     
{                                                        
  struct cdev cdev; /*cdev�ṹ��*/                       
  unsigned char mem[GLOBALMEM_SIZE]; /*ȫ���ڴ�*/        
};

struct globalmem_dev *globalmem_devp; /*�豸�ṹ��ָ��*/
/*�ļ��򿪺���*/
static int globalmem_open(struct inode *inode, struct file *filp)
{
  /*���豸�ṹ��ָ�븳ֵ���ļ�˽������ָ��*/
  struct globalmem_dev *dev;
  
  dev = container_of(inode->i_cdev,struct globalmem_dev,cdev);  
  filp->private_data = dev;  
  return 0;
}
/*�ļ��ͷź���*/
static int globalmem_release(struct inode *inode, struct file *filp)
{
  return 0;
}

/* ioctl�豸���ƺ��� */
static int globalmem_ioctl(struct inode *inodep, struct file *filp, unsigned
  int cmd, unsigned long arg)
{
  struct globalmem_dev *dev = filp->private_data;/*����豸�ṹ��ָ��*/

  switch (cmd)
  {
    case MEM_CLEAR:
      memset(dev->mem, 0, GLOBALMEM_SIZE);      
      printk(KERN_INFO "globalmem is set to zero\n");
      break;

    default:
      return  - EINVAL;
  }
  return 0;
}

/*������*/
static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size,
  loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct globalmem_dev *dev = filp->private_data; /*����豸�ṹ��ָ��*/

  /*�����ͻ�ȡ��Ч��д����*/
  if (p >= GLOBALMEM_SIZE)
    return count ?  - ENXIO: 0;
  if (count > GLOBALMEM_SIZE - p)
    count = GLOBALMEM_SIZE - p;

  /*�ں˿ռ�->�û��ռ�*/
  if (copy_to_user(buf, (void*)(dev->mem + p), count))
  {
    ret =  - EFAULT;
  }
  else
  {
    *ppos += count;
    ret = count;
    
    printk(KERN_EMERG "read %u bytes(s) from %lu\n", count, p);
  }

  return ret;
}

/*д����*/
static ssize_t globalmem_write(struct file *filp, const char __user *buf,
  size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct globalmem_dev *dev = filp->private_data; /*����豸�ṹ��ָ��*/
  
  /*�����ͻ�ȡ��Ч��д����*/
  if (p >= GLOBALMEM_SIZE)
    return count ?  - ENXIO: 0;
  if (count > GLOBALMEM_SIZE - p)
    count = GLOBALMEM_SIZE - p;
    
  /*�û��ռ�->�ں˿ռ�*/
  if (copy_from_user(dev->mem + p, buf, count))
    ret =  - EFAULT;
  else
  {
    *ppos += count;
    ret = count;
    
    printk(KERN_EMERG "written %u bytes(s) from %lu\n", count, p);
  }

  return ret;
}

/* seek�ļ���λ���� */
static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig)
{
  loff_t ret = 0;
  switch (orig)
  {
    case 0:   /*����ļ���ʼλ��ƫ��*/
      if (offset < 0)
      {
        ret =  - EINVAL;
        break;
      }
      if ((unsigned int)offset > GLOBALMEM_SIZE)
      {
        ret =  - EINVAL;
        break;
      }
      filp->f_pos = (unsigned int)offset;
      ret = filp->f_pos;
      break;
    case 1:   /*����ļ���ǰλ��ƫ��*/
      if ((filp->f_pos + offset) > GLOBALMEM_SIZE)
      {
        ret =  - EINVAL;
        break;
      }
      if ((filp->f_pos + offset) < 0)
      {
        ret =  - EINVAL;
        break;
      }
      filp->f_pos += offset;
      ret = filp->f_pos;
      break;
    default:
      ret =  - EINVAL;
      break;
  }
  return ret;
}

/*�ļ������ṹ��*/
static const struct file_operations globalmem_fops =
{
  .owner = THIS_MODULE,
  .llseek = globalmem_llseek,
  .read = globalmem_read,
  .write = globalmem_write,
  .ioctl = globalmem_ioctl,
  .open = globalmem_open,
  .release = globalmem_release,
};

/*��ʼ����ע��cdev*/
static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
  int err, devno = MKDEV(globalmem_major, index);

  cdev_init(&dev->cdev, &globalmem_fops);
  dev->cdev.owner = THIS_MODULE;
  dev->cdev.ops = &globalmem_fops;
  err = cdev_add(&dev->cdev, devno, 1);
  if (err)
    printk(KERN_EMERG "Error %d adding LED%d", err, index);
}

/*�豸����ģ����غ���*/
static int __init globalmem_init(void)
{
  int result;
  dev_t devno = MKDEV(globalmem_major, 0);  // ���32λ�������豸�� ? ��ô���ж�globalmem_majorΪ0�����

  /* �����豸��*/
  if (globalmem_major) // ���ָ�������豸��
    result = register_chrdev_region(devno, 2, "globalmem");
  else  /* ��̬�����豸�� */
  {
    result = alloc_chrdev_region(&devno, 0, 2, "globalmem");
    globalmem_major = MAJOR(devno);
  }
  if (result < 0)
  {
    return result;
  }
    
  /* ��̬����2���豸�ṹ����ڴ�*/
  globalmem_devp = kmalloc(2*sizeof(struct globalmem_dev), GFP_KERNEL);
  if (!globalmem_devp)    /*����ʧ��*/
  {
    result =  - ENOMEM;
    goto fail_malloc;
  }
  memset(globalmem_devp, 0, 2*sizeof(struct globalmem_dev));
  
  globalmem_setup_cdev(&globalmem_devp[0], 0);
  globalmem_setup_cdev(&globalmem_devp[1], 1);
  return 0;

  fail_malloc: unregister_chrdev_region(devno, 1);
  return result;
}

/*ģ��ж�غ���*/
static void __exit globalmem_exit(void)
{
  cdev_del(&(globalmem_devp[0].cdev));   
  cdev_del(&(globalmem_devp[1].cdev));   /*ע��cdev*/
  kfree(globalmem_devp);     /*�ͷ��豸�ṹ���ڴ�*/
  unregister_chrdev_region(MKDEV(globalmem_major, 0), 2); /*�ͷ��豸��*/
}

MODULE_AUTHOR("liuboyf1");
MODULE_LICENSE("Dual BSD/GPL");

module_param(globalmem_major, int, S_IRUGO);

module_init(globalmem_init);
module_exit(globalmem_exit);