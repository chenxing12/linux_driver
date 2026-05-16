#include <linux/init.h>
#include <linux/module.h>

static int debug_level = 0;

static int __init hello_init(void)
{
    printk(KERN_ERR "hello, kernel world error level message \n");

    if (debug_level >= 1) {
        printk(KERN_INFO "hello, kernel world (info level) basic init \n");
    }

    if (debug_level >= 2) {
        printk(KERN_DEBUG "hello, kernel world. (Debug level: deatiled state info)");
    }

    if (debug_level >= 3) {
        printk(KERN_DEBUG "hello, kernel world (verbose level) \n");
    }

    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "goodbye, kernel world \n");
}

module_init(hello_init);
module_exit(hello_exit);
module_param(debug_level, int, 0644);

MODULE_PARM_DESC(debug_level, "debug level (0=error, 1=info, 2=debug, 3=verbose)");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("cx");
MODULE_DESCRIPTION("a sample driver module");