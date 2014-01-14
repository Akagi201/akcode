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
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x5735e9de, "struct_module" },
	{ 0xe4b8c251, "proc_root" },
	{ 0x37a0cba, "kfree" },
	{ 0xb9223523, "cdev_del" },
	{ 0x4302d0eb, "free_pages" },
	{ 0x32aa3679, "remove_proc_entry" },
	{ 0x4ba89b2e, "malloc_sizes" },
	{ 0x9826e5b6, "create_proc_entry" },
	{ 0x110ff64d, "cdev_add" },
	{ 0x3aaa3207, "cdev_init" },
	{ 0x381f6ad5, "init_waitqueue_head" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0xe521e876, "kmem_cache_alloc" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x1d26aa98, "sprintf" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0xffd5a395, "default_wake_function" },
	{ 0x1000e51, "schedule" },
	{ 0x98082893, "__copy_to_user" },
	{ 0xd3777299, "remove_wait_queue" },
	{ 0xafd441b4, "__wake_up" },
	{ 0xbfb13ede, "add_wait_queue" },
	{ 0xdd132261, "printk" },
	{ 0x1042cbb5, "__up_wakeup" },
	{ 0x7a7ef853, "__down_failed" },
	{ 0x89b301d4, "param_get_int" },
	{ 0x98bd6f46, "param_set_int" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

