#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xae141548, "module_layout" },
	{ 0x6980fe91, "param_get_int" },
	{ 0xff964b25, "param_set_int" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x38787e33, "cdev_add" },
	{ 0xf432cc41, "cdev_init" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0xf4c9dacc, "kmem_cache_alloc" },
	{ 0xfc2358d1, "kmalloc_caches" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0xf2a644fb, "copy_from_user" },
	{ 0xb72397d5, "printk" },
	{ 0x3f1899f1, "up" },
	{ 0xfc4f55f3, "down_interruptible" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x37a0cba, "kfree" },
	{ 0x2152d374, "cdev_del" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

