// vchar_driver.c - 虚拟字符设备驱动骨架
#include "asm-generic/ioctl.h"
#include "linux/export.h"
#include "linux/kern_levels.h"
#include "linux/printk.h"
#include "linux/types.h"
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h> // for copy_to_user/copy_from_user
#include <linux/string.h>  // for strlen

#include "vchar_driver.h"

#define DEVICE_NAME "vchar"
#define BUFFER_SIZE 100

static int major_num = 0;
static struct cdev vchar_cdev;
static char kernel_buffer[BUFFER_SIZE] = "hello from kernel";
static int device_size = 100;

static int vchar_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "vchar device open \n");
    return 0;
}

static ssize_t vchar_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos) {
    if (count > BUFFER_SIZE -1) {
        count = BUFFER_SIZE - 1;
    }

    if (copy_from_user(kernel_buffer, buf, count)) {
        printk("copy_from_user failed \n");
        return -EFAULT;
    }

    kernel_buffer[count] = '\0';

    printk("write to kernel message %s \n", kernel_buffer);
    return count;
}

static ssize_t vchar_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos) {
    size_t len = strlen(kernel_buffer);
    if (*ppos >= len)
        return 0;

    if (count > len - *ppos)
        count = len - *ppos;

    if (copy_to_user(buf, kernel_buffer + *ppos, count))
        return -EFAULT;
    *ppos += count;

    return count;
}

static int vchar_release(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "vchar device close \n");
    return 0;
}

static loff_t vchar_llseek(struct file *file, loff_t offset, int whence) {
    loff_t new_pos;

    switch (whence) {
        case SEEK_SET:
            new_pos = offset; // read from start
            break;
        case SEEK_CUR:
            new_pos = file->f_pos + offset;
            break;
        case SEEK_END:
            new_pos = BUFFER_SIZE + offset;
            break;
        default:
            return -EINVAL;
    }

    if (new_pos < 0)
        return -EINVAL;

    file->f_pos = new_pos;

    return new_pos;
}

static long vchar_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    int err = 0;
    int tmp;

    if (_IOC_TYPE(cmd) != VCHAR_IOC_MAGIC) return -ENOTTY;
    if (_IOC_NR(cmd) > VCHAR_IOC_MAXNR) return -ENOTTY;

    switch (cmd) {
        case VCHAR_IOC_RESET:
            printk("vchar resetting device \n");
            break;
        case VCHAR_IOC_SET_SIZE:
            if (copy_from_user(&tmp, (int __user *)arg, sizeof(int))) {
                return -EFAULT;
            }
            printk("setting size to %d \n", tmp);
            device_size = tmp;
            break;
        case VCHAR_IOC_GET_SIZE:
            if (copy_to_user((int __user *)arg, &device_size, sizeof(int))) {
                return -EFAULT;
            }
            printk("getting size %d \n", device_size);
            break;
        
        default:
            return -ENOTTY;
    }

    return err;
}

static struct file_operations vchar_fops = {
    .owner = THIS_MODULE,
    .open = vchar_open,
    .read = vchar_read,
    .write = vchar_write,
    .llseek = vchar_llseek,
    .unlocked_ioctl = vchar_ioctl,
    .release = vchar_release,
};

static int __init vchar_init(void) {
    dev_t dev_num;
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "Failed to allocate chrdev region \n");
        return ret;
    }
    major_num = MAJOR(dev_num);
    cdev_init(&vchar_cdev, &vchar_fops);
    vchar_cdev.owner = THIS_MODULE;
    ret = cdev_add(&vchar_cdev, dev_num, 1);

    if (ret) {
        printk(KERN_ERR "Failed to add cdev \n");
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }
    printk( "vchar driver load sucessfully major_num %d \n", major_num);
    return 0;
}

static void __exit vchar_exit(void) {
    dev_t dev_num = MKDEV(major_num, 0);
    cdev_del(&vchar_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "vchar driver unloaded.\n");
}

module_init(vchar_init);
module_exit(vchar_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A minimal virtual character device driver");