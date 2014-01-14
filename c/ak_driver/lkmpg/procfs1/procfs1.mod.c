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
	{ 0x3c2c5af5, "sprintf" },
	{ 0x714242a0, "create_proc_entry" },
	{ 0xb72397d5, "printk" },
	{ 0x2bc801eb, "remove_proc_entry" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

