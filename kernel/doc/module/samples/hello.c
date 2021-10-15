#include <linux/kernel.h>
#include <linux/module.h>

static int __init linux_hello_init(void)
{
	printk("hello world!\n");
	return 0;
}

static void __exit linux_hello_exit(void)
{
	printk("bye hello world!\n");
}

module_init(linux_hello_init);
module_exit(linux_hello_exit);

MODULE_LICENSE("GPL"); /*必不可少的声明*/
