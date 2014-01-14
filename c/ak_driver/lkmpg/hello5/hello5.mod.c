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
	{ 0xb224fbe2, "param_get_short" },
	{ 0x4333eadb, "param_set_short" },
	{ 0x6980fe91, "param_get_int" },
	{ 0xff964b25, "param_set_int" },
	{ 0x8bd5b603, "param_get_long" },
	{ 0x3457cb68, "param_set_long" },
	{ 0x41344088, "param_get_charp" },
	{ 0x6ad065f4, "param_set_charp" },
	{ 0x43ab66c3, "param_array_get" },
	{ 0x45947727, "param_array_set" },
	{ 0xb72397d5, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

