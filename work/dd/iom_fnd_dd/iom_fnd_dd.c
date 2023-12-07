/* iom_fnd_dd.c */
/* PWD = ~/bbbb/work/dd/iom_fnd_dd */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h> //outb()
#include <linux/uaccess.h> //copy_from_user()
#define DEV_NAME "/dev/iom_fnd"	
#include "../rpi4_gpio_control.c"
#include "../rpi4_bcm2711_gpio_registers.h"
#define iom_fnd_dd_MAJOR_NUM 0
#define IOREMAP_SIZE 0x40 //led 공간할당, GPFSEL0 ~ GPFSEL1
//#define UNUSED_CODE

MODULE_LICENSE("GPL");
MODULE_INFO(intree, "Y");
MODULE_DESCRIPTION("iom_fnd_dd module");
MODULE_AUTHOR("JJLEE");

//#define LED_PIN_NUM 21
int fnd_pin[] = {5, 6, 13, 19, 26, 16, 20}; //a, b, c, d, e, f, g
int fnd_hex_val[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};
unsigned int major_num;
static int gpio_fnd_device_usage = 0; //state if using fnd
//static int gpio_fnd_device_usage = 0;//hidden for outside
static unsigned int *ioremapped_gpio_addr;


int iom_fnd_dd_open (struct inode *, struct file *);
int iom_fnd_dd_release (struct inode *, struct file *);
ssize_t iom_fnd_dd_write (struct file *, const char __user *, size_t, loff_t *);
int iom_fnd_dd_flush (struct file *, fl_owner_t id);
#ifdef UNUSED_CODE
ssize_t iom_fnd_dd_read (struct file *, char __user *, size_t, loff_t *);
long iom_fnd_dd_ioctl (struct file *, unsigned int, unsigned long);
#endif

struct file_operations iom_fnd_dd_fops = {
	.owner = THIS_MODULE,
	.open = iom_fnd_dd_open,
	.release = iom_fnd_dd_release,
	//.read = iom_fnd_dd_read,
	.write = iom_fnd_dd_write,
	//.unlocked_ioctl = iom_fnd_dd_ioctl,
};

int iom_fnd_dd_open (struct inode *inode, struct file *filp)
{
	//if someone is alreay use it, must detect other's access.
	//set the count of availabel device access number...
	if(gpio_fnd_device_usage != 0){
			  return -EBUSY; //print error message
	}
	//control theh number of user using the resource... 
	gpio_fnd_device_usage = 1; 
	printk("iom_fnd_dd_open(): open %s device!!!\n", DEV_NAME);
	return 0;
}

int iom_fnd_dd_release (struct inode *inode, struct file *filp)
{

	int pinNo;
	for(pinNo = 0; pinNo < sizeof(fnd_pin)/sizeof(fnd_pin[0]); pinNo++){	
		//turn off the LED
		gpio_set(fnd_pin[pinNo], SET_GPIO);
	}

	//control the num of user
	gpio_fnd_device_usage = 0;
	printk("iom_fnd_dd_release(): close %s device!!!\n", DEV_NAME);
	
	return 0;
}

#ifdef UNUSED_CODE
ssize_t iom_fnd_dd_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return 0;
}
#endif

ssize_t iom_fnd_dd_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	
	unsigned char value;
	const char *dtmp = buf;
	int x;
	if(copy_from_user(&value, dtmp, count)) return -EFAULT;


	//bit operation...
	if(value >= 0 && value <= 9){
		//clear all FND pins
		//Turn all segments off before writing new FND data
		for(x = 0; x < sizeof(fnd_pin)/sizeof(fnd_pin[0]); x++){
			gpio_set(fnd_pin[x], SET_GPIO);
		}
		for(int x = 0; x < sizeof(fnd_pin)/sizeof(fnd_pin[0]); x++){
			if((fnd_hex_val[value] & (0x1 << x)) == 0) gpio_clr(fnd_pin[x], SET_GPIO); //active-LOW
		}
	}

	printk("iom_fnd_dd_write() function\n");
	return count;
}

#ifdef UNUSED_CODE
long iom_fnd_dd_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("Access iom_fnd_dd_ioctl() function\n");
	return 0;
}
#endif

// This is the starting point of the kernel module by insmod 
static int __init iom_fnd_dd_init(void)
{
	int x;
	//called when setup... set pin as output, memory mapping... 
	major_num = register_chrdev(major_num, DEV_NAME, &iom_fnd_dd_fops);
	if (major_num < 0) {
		printk(KERN_WARNING"%s: can't get or assign major number %d\n", DEV_NAME, major_num);
		return major_num;
	}
	ioremapped_gpio_addr = ioremap(GPIO_BASE, IOREMAP_SIZE); //base부터 size만큼 뭘 해? 초기화?
	for(x = 0; x < sizeof(fnd_pin); x++) gpio_select(fnd_pin[x], GPIO_OUTPUT); //setting gpio direction
	for(x = 0; x < sizeof(fnd_pin); x++) gpio_set(fnd_pin[x], SET_GPIO); //init off(active low)
	
	printk("Success to load the device %s. Major number is %d", DEV_NAME, major_num);

	return 0;
}

// This is the exit point of the kernel module by rmmod
static void __exit iom_fnd_dd_exit(void)
{
	int x;
	//default mode of gpio is input, to prevent the crash of other output.... so we need to set gpio as input. 
	for(x = 0; x < sizeof(fnd_pin); x++) gpio_select(fnd_pin[x], GPIO_INPUT);
	
	iounmap(ioremapped_gpio_addr);	

	unregister_chrdev(major_num, DEV_NAME);
	printk("Unload the device %s…\n", DEV_NAME);
}

module_init(iom_fnd_dd_init);
module_exit(iom_fnd_dd_exit);
