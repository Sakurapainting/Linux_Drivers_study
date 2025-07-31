#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h> 
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/slab.h>

#define GPIOLED_CNT     1
#define GPIOLED_NAME    "gpioled"

struct gpioled_dev{
    struct cdev cdev;      // 字符设备结构体
    struct class *class;   // 设备类
    struct device *device; // 设备
    struct device_node *nd; // 设备树节点
    dev_t devid;
    int major;
    int minor;
};

struct gpioled_dev gpioled;

static int gpioled_open(struct inode *inode, struct file *file) {
    file->private_data = &gpioled; // 将设备结构体指针存储在文件私有数据中
    return 0;
}

static int gpioled_release(struct inode *inode, struct file *file) {
    // struct gpioled_dev *dev = (struct gpioled_dev *)file->private_data;
    return 0;
}

static ssize_t gpioled_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    // struct gpioled_dev *dev = (struct gpioled_dev *)file->private_data;

    return 0;
}

static const struct file_operations gpioled_fops = {
    .owner = THIS_MODULE,
    .open = gpioled_open, // 打开设备的函数
    .write = gpioled_write, // 写入设备的函数    
    .release = gpioled_release, // 释放设备的函数
}; 



static int __init gpioled_init(void){
    int ret = 0;
    // device id
    gpioled.major = 0;
    if(gpioled.major){
        gpioled.devid = MKDEV(gpioled.major, 0);
        register_chrdev_region(gpioled.devid, GPIOLED_CNT, GPIOLED_NAME);
    }
    else{
        alloc_chrdev_region(&gpioled.devid, 0, GPIOLED_CNT, GPIOLED_NAME);
        gpioled.major = MAJOR(gpioled.devid);
        gpioled.minor = MINOR(gpioled.devid);
    }
    if(ret < 0) {
        goto fail_devid;
    }
    printk("gpioled major=%d, minor=%d\n", gpioled.major, gpioled.minor);

    // cdev
    gpioled.cdev.owner = THIS_MODULE;
    cdev_init(&gpioled.cdev, &gpioled_fops);
    ret = cdev_add(&gpioled.cdev, gpioled.devid, GPIOLED_CNT);
    if(ret < 0){
        goto fail_cdev;
    }

    // class
    gpioled.class = class_create(THIS_MODULE, GPIOLED_NAME);
    if(IS_ERR(gpioled.class)){
        ret = PTR_ERR(gpioled.class);
        goto fail_class;
    }
    
    // device
    gpioled.device = device_create(gpioled.class, NULL, gpioled.devid, NULL, GPIOLED_NAME);
    if(IS_ERR(gpioled.device)){
        ret = PTR_ERR(gpioled.device);
        goto fail_device;
    }

    return 0;

fail_device:
    class_destroy(gpioled.class);
fail_class:
    cdev_del(&gpioled.cdev); // 删除字符设备
fail_cdev:
    unregister_chrdev_region(gpioled.devid, GPIOLED_CNT);
fail_devid:
    return ret;
}

static void __exit gpioled_exit(void){

    device_destroy(gpioled.class, gpioled.devid); // 销毁设备
    class_destroy(gpioled.class); // 销毁设备类
    cdev_del(&gpioled.cdev); // 删除字符设备
    unregister_chrdev_region(gpioled.devid, GPIOLED_CNT); // 注销设备号
    printk("gpioled exit\n");
}



module_init(gpioled_init);
module_exit(gpioled_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SakoroYou");
