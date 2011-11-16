#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/miscdevice.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/delay.h>

#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
#include <mach/gpio-aries.h>

#include "fc8100.h"
#include "bbm.h"
#include "fci_oal.h"

#include "fc8100_i2c.h"

#define FC8100_NAME		"dmb"


int dmb_open (struct inode *inode, struct file *filp);
int dmb_ioctl (struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
int dmb_release (struct inode *inode, struct file *filp);


/* GPIO Setting */
void dmb_hw_setting(void);
void dmb_hw_init(void);
void dmb_hw_deinit(void);

static struct file_operations dmb_fops = 
{
	.owner		= THIS_MODULE,
	.ioctl		= dmb_ioctl,
	.open		= dmb_open,
	.release	= dmb_release,
};

static struct miscdevice fc8100_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = FC8100_NAME,
    .fops = &dmb_fops,
};

int dmb_open (struct inode *inode, struct file *filp)
{
	DMB_OPEN_INFO_T *hOpen = NULL;

	PRINTF(0, "dmb open HANDLE : 0x%x\n", hOpen);

	hOpen = (DMB_OPEN_INFO_T *)kmalloc(sizeof(DMB_OPEN_INFO_T), GFP_KERNEL);

	filp->private_data = hOpen;

	return 0;
}

int dmb_release (struct inode *inode, struct file *filp)
{
	DMB_OPEN_INFO_T *hOpen;

	PRINTF(0, "dmb release \n");

	return 0;
}

int dmb_ioctl (struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	s32 res = BBM_NOK;

	ioctl_info info;

	int	err, size;

	if(_IOC_TYPE(cmd) != IOCTL_MAGIC) return -EINVAL;
	if(_IOC_NR(cmd) >= IOCTL_MAXNR) return -EINVAL;

	size = _IOC_SIZE(cmd);

#if 0
	if(size) {
		err = 0;
		if(_IOC_DIR(cmd) & _IOC_READ)
			err = verify_area(VERIFY_WRITE, (void *) arg, size);
		else if(_IOC_DIR(cmd) & _IOC_WRITE)
			err = verify_area(VERIFY_READ, (void *) arg, size);
		if(err) return err;
	}
#endif

	switch(cmd) 
	{
		case IOCTL_DMB_RESET:
			res = BBM_RESET(0);
			break;
		case IOCTL_DMB_INIT:
			res = BBM_INIT(0);
			break;
		case IOCTL_DMB_BYTE_READ:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_BYTE_READ(0, (u16)info.buff[0], (u8 *)(&info.buff[1]));
			copy_to_user((void *)arg, (void *)&info, size);
			break;
		case IOCTL_DMB_WORD_READ:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_WORD_READ(0, (u16)info.buff[0], (u16 *)(&info.buff[1]));
			copy_to_user((void *)arg, (void *)&info, size);
			break;
		case IOCTL_DMB_LONG_READ:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_LONG_READ(0, (u16)info.buff[0], (u32 *)(&info.buff[1]));
			copy_to_user((void *)arg, (void *)&info, size);
			break;
		case IOCTL_DMB_BULK_READ:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_BULK_READ(0, (u16)info.buff[0], (u8 *)(&info.buff[2]), info.buff[1]);
			copy_to_user((void *)arg, (void *)&info, size);
			break;
		case IOCTL_DMB_BYTE_WRITE:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_BYTE_WRITE(0, (u16)info.buff[0], (u8)info.buff[1]);
			break;
		case IOCTL_DMB_WORD_WRITE:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_WORD_WRITE(0, (u16)info.buff[0], (u16)info.buff[1]);
			break;
		case IOCTL_DMB_LONG_WRITE:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_LONG_WRITE(0, (u16)info.buff[0], (u32)info.buff[1]);
			break;
		case IOCTL_DMB_BULK_WRITE:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_BULK_WRITE(0, (u16)info.buff[0], (u8 *)(&info.buff[2]), info.buff[1]);
			break;
		case IOCTL_DMB_TUNER_SELECT:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_TUNER_SELECT(0, (u32)info.buff[0], 0);
			break;
		case IOCTL_DMB_TUNER_READ:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_TUNER_READ(0, (u8)info.buff[0], (u8)info.buff[1],  (u8 *)(&info.buff[3]), (u8)info.buff[2]);
			copy_to_user((void *)arg, (void *)&info, size);
			break;
		case IOCTL_DMB_TUNER_WRITE:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_TUNER_WRITE(0, (u8)info.buff[0], (u8)info.buff[1], (u8 *)(&info.buff[3]), (u8)info.buff[2]);
			break;
		case IOCTL_DMB_TUNER_SET_FREQ:
			copy_from_user((void *)&info, (void *)arg, size);
			res = BBM_TUNER_SET_FREQ(0, (u8)info.buff[0]);
			break;
		case IOCTL_DMB_POWER_ON:
			PRINTF(0, "DMB_POWER_ON enter..\n");
			dmb_hw_init();
			break;
		case IOCTL_DMB_POWER_OFF:
			PRINTF(0, "DMB_POWER_OFF enter..\n");
			dmb_hw_deinit();
			break;
		default:
			PRINTF(0, "dmb_ioctl : Undefined ioctl command\n");
			res = BBM_NOK;
			break;
	}
	return res;
}


void dmb_hw_setting(void)
{
	gpio_request(GPIO_ISDBT_PWR_EN,"ISDBT_EN");
	udelay(50);
	gpio_direction_output(GPIO_ISDBT_PWR_EN,1); //!!!

	gpio_request(GPIO_ISDBT_RST,"ISDBT_RST");
	udelay(50);
	gpio_direction_output(GPIO_ISDBT_RST,1); //!!!	
}


void dmb_hw_init(void)
{
	gpio_set_value(GPIO_ISDBT_PWR_EN, 1);  // ISDBT EN Enable
	mdelay(1);
	PRINTF(0, "GPIO_ISDBT_PWR_EN..\n");
	gpio_set_value(GPIO_ISDBT_RST, 1);	// ISDBT RST Enable
	msleep(1);
	PRINTF(0, "GPIO_ISDBT_RST...\n");

}

void dmb_hw_deinit(void)
{
	gpio_set_value(GPIO_ISDBT_RST, 0);  // ISDBT EN Disable
	mdelay(1);
	gpio_set_value(GPIO_ISDBT_PWR_EN, 0);	// ISDBT RST Disable
	msleep(1);
}


int dmb_init(void)
{
	int result;

	PRINTF(0, "dmb dmb_init \n");

	dmb_hw_setting();
	dmb_hw_init();

    /*misc device registration*/
	result = misc_register(&fc8100_misc_device);
	printk("[DMB:DEREK] MISC_REGISTER result = %d\n", result);


	
	if(result < 0)
		return result;

 	//Derek: 
	//fc8100_i2c_init_();
    //    i2c_init();
	BBM_HOSTIF_SELECT(0, BBM_I2C);

	dmb_hw_deinit();

	return 0;
}

void dmb_exit(void)
{
	PRINTF(0, "dmb dmb_exit \n");

	BBM_HOSTIF_DESELECT(0);
	dmb_hw_deinit();

/*misc device deregistration*/
misc_deregister(&fc8100_misc_device);
}

module_init(dmb_init);
module_exit(dmb_exit);

MODULE_LICENSE("Dual BSD/GPL");

