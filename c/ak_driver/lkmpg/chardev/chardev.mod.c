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
	{ 0xe8b8056d, "__register_chrdev" },
	{ 0xc3aaf0a9, "__put_user_1" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0xacdeb154, "__tracepoint_module_get" },
	{ 0x280f9f14, "__per_cpu_offset" },
	{ 0x9629486a, "per_cpu__cpu_number" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xabde244e, "module_put" },
	{ 0xb72397d5, "printk" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

