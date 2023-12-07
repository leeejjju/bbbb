/* iom_led_dd.c */
/* PWD = ~/bbbb/work/dd/iom_led_dd */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h> //outb()
#include <linux/uaccess.h> //copy_from_user()
#define DEV_NAME "/dev/iom_led"	
#include "../rpi4_gpio_control.c"
#include "../rpi4_bcm2711_gpio_registers.h"

#define iom_led_dd_MAJOR_NUM 0
#define LED_PIN_NUM 21
#define IOREMAP_SIZE 0x40 //led 공간할당, GPFSEL0 ~ GPFSEL1
//#define UNUSED_CODE

MODULE_LICENSE("GPL");
MODULE_INFO(intree, "Y");
MODULE_DESCRIPTION("iom_led_dd module");
MODULE_AUTHOR("JJLEE");

int iom_led_dd_open (struct inode *, struct file *);
int iom_led_dd_release (struct inode *, struct file *);
ssize_t iom_led_dd_write (struct file *, const char __user *, size_t, loff_t *);
int iom_led_dd_flush (struct file *, fl_owner_t id);
#ifdef UNUSED_CODE
ssize_t iom_led_dd_read (struct file *, char __user *, size_t, loff_t *);
long iom_led_dd_ioctl (struct file *, unsigned int, unsigned long);
#endif

struct file_operations iom_led_dd_fops = {
	.owner = THIS_MODULE,
	.open = iom_led_dd_open,
	.release = iom_led_dd_release,
	//.read = iom_led_dd_read,
	.write = iom_led_dd_write,
	//.unlocked_ioctl = iom_led_dd_ioctl,
};

unsigned int major_num;
static int gpio_pin21_usage = 0;//hidden for outside
static unsigned int *ioremapped_gpio_addr;

int iom_led_dd_open (struct inode *inode, struct file *filp)
{
	//if someone is alreay use it, must detect other's access.
	//set the count of availabel device access number...
	if(gpio_pin21_usage != 0){
			  return -EBUSY; //print error message
	}
	//control theh number of user using the resource... 
	gpio_pin21_usage = 1; 
	printk("iom_led_dd_open(): open %s device!!!\n", DEV_NAME);
	return 0;
}

int iom_led_dd_release (struct inode *inode, struct file *filp)
{
	printk("iom_led_dd_release(): close %s device!!!\n", DEV_NAME);
	//control the num of user
	gpio_pin21_usage = 0;
	//turn off the LED
	gpio_clr(LED_PIN_NUM, GPIO_CLR);
	return 0;
}

#ifdef UNUSED_CODE
ssize_t iom_led_dd_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return 0;
}
#endif

ssize_t iom_led_dd_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	
	unsigned char value;
	const char *dtmp = buf;
		
	if(copy_from_user(&value, dtmp, count)) return -EFAULT;

	if(value) gpio_set(LED_PIN_NUM, GPIO_SET);
	else 		 gpio_clr(LED_PIN_NUM, GPIO_CLR);

		
		  
		  
		  
	printk("iom_led_dd_write() function\n");
	return count;
}

#ifdef UNUSED_CODE
long iom_led_dd_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("Access iom_led_dd_ioctl() function\n");
	return 0;
}
#endif

// This is the starting point of the kernel module by insmod 
static int __init iom_led_dd_init(void)
{
	//called when setup... set pin as output, memory mapping... 
	major_num = register_chrdev(major_num, DEV_NAME, &iom_led_dd_fops);
	if (major_num < 0) {
		printk(KERN_WARNING"%s: can't get or assign major number %d\n", DEV_NAME, major_num);
		return major_num;
	}
	ioremapped_gpio_addr = ioremap(GPIO_BASE, IOREMAP_SIZE); //base부터 size만큼 뭘 해? 초기화?
	gpio_select(LED_PIN_NUM, GPIO_OUTPUT); //setting gpio direction
	gpio_clr(LED_PIN_NUM, GPIO_SET); //off led through clear, because led is active high
	
	printk("Success to load the device %s. Major number is %d", DEV_NAME, major_num);

	return 0;
}

// This is the exit point of the kernel module by rmmod
static void __exit iom_led_dd_exit(void)
{

	//default mode of gpio is input, to prevent the crash of other output.... so we need to set gpio as input. 
	gpio_select(LED_PIN_NUM, GPIO_INPUT);
	
	iounmap(ioremapped_gpio_addr);	

	unregister_chrdev(major_num, DEV_NAME);
	printk("Unload the device %s…\n", DEV_NAME);
}

module_init(iom_led_dd_init);
module_exit(iom_led_dd_exit);
