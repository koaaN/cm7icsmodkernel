/*****************************************************************************
 Copyright(c) 2009 FCI Inc. All Rights Reserved
 
 File name : fc8100_i2c.c
 
 Description : fc8100 host interface
 
 History : 
 ----------------------------------------------------------------------
 2009/09/29 	bruce		initial
*******************************************************************************/
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/delay.h>

#include "fci_types.h"
#include "fci_oal.h"
#include "fc8100_i2c.h"

#define FC8100_I2C_ADDR 0x77
#define I2C_M_RD 1
#define I2C_M_WR 0
#define I2C_MAX_SEND_LENGTH 300
#define  CONFIG_LATIN_S1_FROYO


//Derek: Modify the I2C driver for S1 Froyo:2010.07.28
static struct i2c_driver fc8100_i2c_driver;
static struct i2c_client *fc8100_i2c_client = NULL;


int fc8100_init_ = 0;

struct fc8100_state{
	struct i2c_client	*client;	
};
struct fc8100_state *fc8100_state;


static u8 bb_reg[32] = {0xe7, };
static u8 rf_reg[32] = {0xaa, };


int i2c_init()
{
	//Derek: Modify the I2C driver for S1 Froyo:2010.07.28
	int res;

	printk("fc8100_i2c_init ENTER...\n");
	fc8100_i2c_client = kzalloc(sizeof(struct i2c_client), GFP_KERNEL);
	
	if(fc8100_i2c_client== NULL) {
		printk("fc8100_i2c_client NULL...\n");
		return -ENOMEM;
	}

	if (res=i2c_add_driver(&fc8100_i2c_driver))
		pr_err("%s: Can't add fc8100 i2c drv\n", __func__);
	else
		pr_info("%s: Added fc8100 i2c drv\n", __func__);

	return res;
}


static int i2c_deinit()
{
	printk("fc8100_i2c_drv_deinit ENTER...\n");

	i2c_del_driver(&fc8100_i2c_driver);

	return 0;
}


static int i2c_bb_read(HANDLE hDevice, u16 addr, u8 *data, u16 length) 
{
	int res;
	struct i2c_msg rmsg[2];
	unsigned char i2c_data[1];

	rmsg[0].addr = fc8100_i2c_client->addr;
	rmsg[0].flags = I2C_M_WR;
	rmsg[0].len = 1;
	rmsg[0].buf = i2c_data;
	i2c_data[0] = addr & 0xff;
	
	rmsg[1].addr = fc8100_i2c_client->addr;
	rmsg[1].flags = I2C_M_RD;
	rmsg[1].len = length;
	rmsg[1].buf = data;
	res = i2c_transfer(fc8100_i2c_client->adapter, &rmsg, 2);

	return 0;
}

static int i2c_bb_write(HANDLE hDevice, u16 addr, u8 *data, u16 length)
{
	int res;
	struct i2c_msg wmsg;
	unsigned char i2c_data[I2C_MAX_SEND_LENGTH];

	if(length+1>I2C_MAX_SEND_LENGTH)
	{
		printk(".......error", __FUNCTION__);
		return -ENODEV;
	}
	wmsg.addr = fc8100_i2c_client->addr;
	wmsg.flags = I2C_M_WR;
	wmsg.len = length + 1;
	wmsg.buf = i2c_data;

	i2c_data[0] = addr & 0xff;
	memcpy(&i2c_data[1], data, length);

	res = i2c_transfer(fc8100_i2c_client->adapter, &wmsg, 1);

	return 0;


}

static int i2c_rf_read(HANDLE hDevice, u8 addr, u8 *data, u8 length) 
{
	int res;
	struct i2c_msg rmsg[3];
	unsigned char i2c_rp_data[2];
	unsigned char i2c_data[1];

	printk("i2c_rf_read ENTER  addr : %x, length : %d\n", addr, length);
	rmsg[0].addr = fc8100_i2c_client->addr;
	rmsg[0].flags = I2C_M_WR;
	rmsg[0].len = 2;
	rmsg[0].buf = i2c_rp_data;
	i2c_rp_data[0] = 0x02;
	i2c_rp_data[1] = 0x01;

	rmsg[1].addr = fc8100_i2c_client->addr;
	rmsg[1].flags = I2C_M_WR;
	rmsg[1].len = 1;
	rmsg[1].buf = i2c_data;
	i2c_data[0] = addr & 0xff;

	rmsg[2].addr = fc8100_i2c_client->addr;
	rmsg[2].flags = I2C_M_RD;
	rmsg[2].len = length;
	rmsg[2].buf = data;

	res = i2c_transfer(fc8100_i2c_client->adapter, &rmsg, 3);
	printk("i2c_rf_read data : %x\n", data[0]);
	return 0;

}

static int i2c_rf_write(HANDLE hDevice, u8 addr, u8 *data, u8 length)
{

	int res;
	struct i2c_msg wmsg[2];
	unsigned char i2c_rp_data[2];
	unsigned char i2c_data[I2C_MAX_SEND_LENGTH];

	printk("i2c_rf_write ENTER  addr : %x, data : %x, length : %d\n", addr, data[0], length);
	if(length+1>I2C_MAX_SEND_LENGTH)
	{
		printk(".......error", __FUNCTION__);
		return -ENODEV;
	}

	wmsg[0].addr = fc8100_i2c_client->addr;
	wmsg[0].flags = I2C_M_WR;
	wmsg[0].len = 2;
	wmsg[0].buf = i2c_rp_data;
	i2c_rp_data[0] = 0x02;
	i2c_rp_data[1] = 0x01;
	
	wmsg[1].addr = fc8100_i2c_client->addr;
	wmsg[1].flags = I2C_M_WR;
	wmsg[1].len = length+1;
	wmsg[1].buf = i2c_data;
	i2c_data[0] = addr & 0xff;
	memcpy(&i2c_data[1], data, length);

	res = i2c_transfer(fc8100_i2c_client->adapter, &wmsg, 2);
	printk("i2c_transfer ENTER	addr : %x, res : %x\n", addr, res);

	return 0;

}

int fc8100_i2c_init(HANDLE hDevice, u16 param1, u16 param2)
{

	int res;

	res = i2c_init();

	return res;

}

int fc8100_i2c_byteread(HANDLE hDevice, u16 addr, u8 *data)
{
	int res = BBM_NOK;

	//PRINTF(0, "fc8100_i2c_byteread 0x%x\n", addr);
	res =  i2c_bb_read(hDevice, addr, (u8 *)data, 1);

	return res;  
}

int fc8100_i2c_wordread(HANDLE hDevice, u16 addr, u16 *data)
{
	int res = BBM_NOK;

	//PRINTF(0, "fc8100_i2c_wordread 0x%x\n", addr);
	res =  i2c_bb_read(hDevice, addr, (u8 *)data, 2);

	return res;  
}

int fc8100_i2c_longread(HANDLE hDevice, u16 addr, u32 *data)
{
	int res = BBM_NOK;

	//PRINTF(0, "fc8100_i2c_longread 0x%x\n", addr);
	res =  i2c_bb_read(hDevice, addr, (u8 *)data, 4);

	return res;  
}

int fc8100_i2c_bulkread(HANDLE hDevice, u16 addr, u8 *data, u16 size)
{
	int res = BBM_NOK;

	//PRINTF(0, "fc8100_i2c_bulkread 0x%x\n", addr);
	res =  i2c_bb_read(hDevice, addr, (u8 *)data, size);

	return BBM_OK;  
}

int fc8100_i2c_bytewrite(HANDLE hDevice, u16 addr, u8 data)
{
	int res = BBM_NOK;
	
	res = i2c_bb_write(hDevice, addr, (u8 *)&data, 1);
	
	return res;
}

int fc8100_i2c_wordwrite(HANDLE hDevice, u16 addr, u16 data)
{
	int res = BBM_NOK;
	
	res = i2c_bb_write(hDevice, addr, (u8 *)&data, 2);
	
	return res;
}

int fc8100_i2c_longwrite(HANDLE hDevice, u16 addr, u32 data)
{
	int res = BBM_NOK;

	res = i2c_bb_write(hDevice, addr, (u8 *)&data, 4);

	return res;
}

int fc8100_i2c_bulkwrite(HANDLE hDevice, u16 addr, u8 *data, u16 size)
{
	int res = BBM_NOK;

	res = i2c_bb_write(hDevice, addr, (u8 *)&data, size);

	return res;  
}

int fc8100_i2c_dataread(HANDLE hDevice, u16 addr, u8 *data, u16 size)
{
	int res = BBM_NOK;

	return res;  
}

int fc8100_rf_bulkread(HANDLE hDevice, u8 addr, u8 *data, u8 size)
{
	int res = BBM_NOK;

	res = i2c_rf_read(hDevice, addr, (u8 *)data, size);

	return res;  
}

int fc8100_rf_bulkwrite(HANDLE hDevice, u8 addr, u8 *data, u8 size)
{
	int res = BBM_NOK;

	res = i2c_rf_write(hDevice, addr, (u8 *)data, size);

	return res;  
}

int fc8100_i2c_deinit(HANDLE hDevice)
{
	int res = BBM_NOK;

	res = i2c_deinit(hDevice);

	return res;  
}

int fc8100_spi_init(HANDLE hDevice, u16 param1, u16 param2)
{

	return BBM_OK;
}

int fc8100_spi_dataread(HANDLE hDevice, u16 addr, u8 * pBuf, u16 nLength)
{
	return BBM_OK;
}


//Derek: Modify the I2C driver for S1 Froyo:2010.07.28


static int fc8100_i2c_remove(struct i2c_client *client)
{
	struct fc8100_state *fc8100 = i2c_get_clientdata(client);

	kfree(fc8100);
	return 0;
}

static int fc8100_i2c_probe(struct i2c_client *client,  const struct i2c_device_id *id)
{
	struct fc8100_state *fc8100;

	fc8100 = kzalloc(sizeof(struct fc8100_state), GFP_KERNEL);
	if (fc8100 == NULL) {		
		printk("failed to allocate memory \n");
		return -ENOMEM;
	}
	
	fc8100->client = client;
	i2c_set_clientdata(client, fc8100);
	
	/* rest of the initialisation goes here. */
	
	printk("fc8100 I2C attach success!!!\n");

	fc8100_i2c_client = client;

	fc8100_init_ = 1;
	
	return 0;
}

static const struct i2c_device_id fc8100_device_id[] = {
	{"fc8100", 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, fc8100_device_id);


static struct i2c_driver fc8100_i2c_driver = {
	.driver = {
		.name = "fc8100",
		.owner = THIS_MODULE,
	},
	.probe	= fc8100_i2c_probe,
	.remove	= fc8100_i2c_remove,
	.id_table	= fc8100_device_id,
};
//Derek: Modify the I2C driver for S1 Froyo:2010.07.28



