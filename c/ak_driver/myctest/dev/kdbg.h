/*	@file kdbg.h
 *	@note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *	@brief 定义内核调试宏
 *	@author qinhuatao
 *	@date 2011/10/11
 *	@note
 *
 */

#ifndef __KDBG_H_
#define __KDBG_H_

///<如果定义了DEBUG
#define dev_dbg(format, arg...)		\
	do {printk(KERN_ALERT format , ## arg);}while(0)
#define dbg_prt_line \
	//do{printk(KERN_ALERT "dbg line----[%s]:[%d]\n", __func__, __LINE__);}while(0)


#define dev_err(format, arg...)		\
	do {printk(KERN_ERR format , ## arg);}while(0)
#define dev_info(format, arg...)		\
	do {printk(KERN_INFO format , ## arg);}while(0)
#define dev_warn(format, arg...)		\
	do {printk(KERN_WARNING format , ## arg);}while(0)
#define dev_notice(format, arg...)		\
	do {printk(KERN_NOTICE, format , ## arg);}while(0)



#endif 
