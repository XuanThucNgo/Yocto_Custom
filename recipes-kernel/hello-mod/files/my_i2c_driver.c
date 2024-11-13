#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

//Macros for driver name, class name and device name
#define DRIVER_NAME "dc_driver"
#define DRIVER_CLASS "dc_driver"

//General description for driver
MODULE_LICENSE("GPL");
MODULE_AUTHOR("NgoXuanThuc");
MODULE_DESCRIPTION("I2C driver for control DC motor");

// Define static variables for the i2c client, driver class, driver device, and major number
static struct i2c_client *dcDriver_client;
typedef struct {
  dev_t dev_num;
  struct class *m_class;
  struct cdev m_cdev;
} b_dev;

static b_dev mdev;

//Define an enum for module's register
typedef enum SwerveParamRegister{
  DEV_REG_SPEED_KP = 1,
  DEV_REG_SPEED_KI,
  DEV_REG_SPEED_KD,

  DEV_REG_ANGLE_KP,
  DEV_REG_ANGLE_KI,
  DEV_REG_ANGLE_KD,

  DEV_REG_TARGET_SPEED,
  DEV_REG_TARGET_ANGLE,
  DEV_REG_MODE,
}SwerveParamRegister;

//Define cmd for in out control 
#define SWERVE_MODULE_MAGIC_WORD 's'
#define SWERVE_IOCTL_SEND_TARGET_SPEED _IOW(SWERVE_MODULE_MAGIC_WORD,DEV_REG_TARGET_SPEED,u8)
#define SWERVE_IOCTL_SEND_TARGET_ANGLE _IOW(SWERVE_MODULE_MAGIC_WORD,DEV_REG_TARGET_ANGLE,u8)
#define SWERVE_IOCTL_SET_MODE _IOW(SWERVE_MODULE_MAGIC_WORD,DEV_REG_MODE,u8)
#define SWERVE_IOCTL_READ_CURRENT_ANGLE _IOR(SWERVE_MODULE_MAGIC_WORD,17,int)

//Define i2c device id list
static struct i2c_device_id my_ids[] = {
  {"dc_driver", 0},
  {},
};
MODULE_DEVICE_TABLE(i2c, my_ids);

//Function for read data from module's register DEV_REG_TARGET_ANGLE
static int dcDriver_read(struct i2c_client *client)
{
  u8 buf[1];

  if (i2c_smbus_read_i2c_block_data(client, DEV_REG_TARGET_ANGLE, sizeof(buf), buf) < 0) {
    printk(KERN_ERR "Failed to read Target Angle data\n");
    return -EIO;
  }

  return buf[0];
}

//Function for read data from module's register
static u8 dcDriver_received_data(struct i2c_client *client, u8 reg)
{
  u8 value;
  value = i2c_smbus_read_byte_data(client, reg);
  printk("Value read: %u from reg: %u",value, reg);
  return value;
}

//Function for send data to module's register
static int dcDriver_send_data(struct i2c_client *client, u8 data, u8 reg)
{
  if(i2c_smbus_write_byte_data(client, reg, data) < 0){
    printk("Failed to send data");
    return -EIO;
  }

  return 1;
}

//In out control for user interface
static long dcDriver_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
  int data;
  u8 value;
  int receiveData;
  switch (cmd) {
    case SWERVE_IOCTL_SEND_TARGET_SPEED:
      if(copy_from_user(&value,(u8 __user *)arg,sizeof(value))){
        printk("Data write from user error");
      }
      //printk("Target speed: %u", value);
      data = dcDriver_send_data(dcDriver_client, value, DEV_REG_TARGET_SPEED);
      break;
    case SWERVE_IOCTL_SEND_TARGET_ANGLE:
      if(copy_from_user(&value,(u8 __user *)arg,sizeof(value))){
        printk("Data write from user error");
      }
      //printk("Target angle: %u", value);
      data = dcDriver_send_data(dcDriver_client, value, DEV_REG_TARGET_ANGLE);
      break;
    case SWERVE_IOCTL_SET_MODE:
      if(copy_from_user(&value,(u8 __user *)arg,sizeof(value))){
        printk("Data write from user error");
      }
      printk("Mode set: %u", value);
      data = dcDriver_send_data(dcDriver_client, value, DEV_REG_MODE);
      break;
    case SWERVE_IOCTL_READ_CURRENT_ANGLE:
      receiveData = dcDriver_read(dcDriver_client);
      if (copy_to_user((int __user *)arg, &receiveData, sizeof(receiveData))) {
        return -EFAULT;
      }
      printk("Receive data angle: %u", receiveData);
      break;
    default:
      return -EINVAL;
  }
  return 0;
}    

//Open function for file operations
static int dcDriver_open(struct inode *inodep, struct file *filep)
{
  printk(KERN_INFO "dc_driver device opened\n");
  return 0;
}

//Release function for file operations
static int dcDriver_release(struct inode *inodep, struct file *filep)
{
  printk(KERN_INFO "dc_driver device closed\n");
  return 0;
}

//Define file operations struct for driver
static struct file_operations fops = {
  .open = dcDriver_open,
  .unlocked_ioctl = dcDriver_ioctl,
  .release = dcDriver_release,
};

//Define probe function for driver
static int my_probe(struct i2c_client *client, const struct i2c_device_id *id)
{	
  dcDriver_client = client;
  printk("dc_driver - Now I am in the probe function!\n");

  if(alloc_chrdev_region(&mdev.dev_num, 0, 1, DRIVER_NAME)) {
    pr_err("encoder - Error: Cannot make number device\n");
    return -1;
  }
  pr_info("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", mdev.dev_num >> 20, mdev.dev_num & 0xfffff);
  if((mdev.m_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
    pr_err("encoder - Error: Cannot create class\n");
    goto ClassError;
  }
  if(device_create(mdev.m_class, NULL, mdev.dev_num, NULL, DRIVER_NAME) == NULL) {
    pr_err("encoder - Error: Cannot create device\n");
    goto FileError;
  }
  cdev_init(&mdev.m_cdev, &fops);
  if(cdev_add(&mdev.m_cdev, mdev.dev_num, 1) < 0) {
    pr_err("encoder - Error: Cannot add device\n");
    goto AddError;
  }
  printk(KERN_INFO "dc_driver installed\n");
  return 0;
AddError:
  device_destroy(mdev.m_class, mdev.dev_num);
FileError:
  class_destroy(mdev.m_class);
ClassError:
  unregister_chrdev_region(mdev.dev_num, 1);
  return -1;
}

//Define remove function for driver
static int my_remove(struct i2c_client *client)
{
  cdev_del(&mdev.m_cdev);
  device_destroy(mdev.m_class, mdev.dev_num);
  class_destroy(mdev.m_class);
  unregister_chrdev_region(mdev.dev_num, 1);
  printk("my_i2c_driver - Removing device\n");

  return 0;
}

//Define i2c driver struct
static struct i2c_driver my_driver= {
  .probe = my_probe,
  .remove = my_remove,
  .id_table = my_ids,
  .driver = {
    .name = DRIVER_NAME,
  }
};

//Create i2c driver module
module_i2c_driver(my_driver);
