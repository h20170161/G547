#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 
#include<linux/uaccess.h>              
#include <linux/ioctl.h>
#include <linux/random.h>

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

#define channel0 0x40
#define channel1 0x41
#define channel2 0x42
#define command_register 0x43
#define LATCH  1193
unsigned int value, current_counter;
int i, val;
dev_t dev = 0;
static struct class *timer0_class;
static struct cdev timer0_cdev;

static int __init timer0_init(void);
static void __exit timer0_exit(void);
static int timer0_open(struct inode *inode, struct file *file);
static int timer0_release(struct inode *inode, struct file *file);
static ssize_t timer0_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t timer0_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long timer0_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int delay(void);
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = timer0_read,
        .write          = timer0_write,
        .open           = timer0_open,
        .unlocked_ioctl = timer0_ioctl,
        .release        = timer0_release,
};

static int timer0_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

static int timer0_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

static ssize_t timer0_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read Function\n");
        return 0;
}
static ssize_t timer0_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write function\n");
        return 0;
}

static int delay(void)
{
   outb_p(0x34,command_register);
   outb_p(LATCH & 0xff,channel0);
   outb_p(LATCH >> 8,channel0);
 return 0;
}
static int read_current_value(void)
{
   outb_p(0x00, command_register);
   current_counter = inb(channel0);
   current_counter = current_counter|(inb(channel0)<<8);
   return current_counter ;
} 
static long timer0_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{   
   
         switch(cmd) {
                case WR_VALUE:  i=0;
                        copy_from_user(&value ,(int32_t*) arg, sizeof(value));
                        printk(KERN_INFO "Value = %d\n", value);
                       while(i<value)
                    {
                      current_counter = read_current_value();
                     printk(KERN_INFO "current_counter_Value = %d\n", current_counter);
                     if(current_counter == 1)
                     {i++ ;
                       }
                       }  
                      break;
                case RD_VALUE:  val = read_current_value();
                        copy_to_user((int32_t*) arg, &val, sizeof(val)); 
                        break;
     
        }
        return 0;
}



static int __init timer0_init(void)
{
   

        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "timer0_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        /*Creating cdev structure*/
        cdev_init(&timer0_cdev,&fops);
        timer0_cdev.owner = THIS_MODULE;
        timer0_cdev.ops = &fops;

        /*Adding character device to the system*/
        if((cdev_add(&timer0_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }

        /*Creating struct class*/
        if((timer0_class = class_create(THIS_MODULE,"timer0_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }

        /*Creating device*/
        if((device_create(timer0_class,NULL,dev,NULL,"timer0_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
     
   return 0;

r_device:
        class_destroy(timer0_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

void __exit timer0_exit(void)
{
        device_destroy(timer0_class,dev);
        class_destroy(timer0_class);
        cdev_del(&timer0_cdev);
        unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(timer0_init);
module_exit(timer0_exit);

MODULE_LICENSE("GPL");
