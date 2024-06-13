#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/kobject.h>
#include <linux/kdev_t.h>
#include <linux/list.h>
#include "address_map_arm.h"

#define DATA_A_BASE 0x80
#define DATA_B_BASE 0x70
#define WRREG_BASE 0xc0
#define WRFULL_BASE 0xb0
#define SCREEN_BASE 0xa0
#define RESET_PULSE_COUNTER_BASE 0x90

#define DEVICE_NAME "colenda_driver"
#define MIN_BUFFER_SIZE 8

MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("G03");
MODULE_DESCRIPTION("Driver do CoLenda para o kit de desenvolvimento DE1-SoC");

static dev_t dev;
static struct cdev *kernel_cdev;

volatile int *DATA_A_ptr;
volatile int *DATA_B_ptr;
volatile int *WRREG_ptr;
volatile int *WRFULL_ptr;

static int gpu_decode_instruction(char *buffer);
static ssize_t gpu_write(struct file *filp, const char *buff, size_t len, loff_t *off);
static int gpu_open(struct inode *device_file, struct file *instance);
static int gpu_release(struct inode *device_file, struct file *instance);
static int __init gpu_driver_init(void);
static void __exit gpu_driver_exit(void);

static struct file_operations fops = {
	.owner = THIS_MODULE,
   	.write = gpu_write,
	.open = gpu_open,
	.release = gpu_release
};

/**
 * @brief Decodifica uma instrução de um buffer.
 *
 * Esta função recebe um buffer contendo uma instrução e a decodifica em dois valores de dados de 32 bits, DATA_A_ptr
 e DATA_B_ptr.
 * A instrução é assumida estar em um formato específico, onde os últimos 4 bytes representam DATA_A_ptr
 e os primeiros 4 bytes representam DATA_B_ptr.
 *
 * @param buffer Um ponteiro para o buffer contendo a instrução.
 *
 * @return Esta função não retorna nenhum valor.
 *
 * @note A função assume que o buffer contém pelo menos 8 bytes de dados.
 *
 * @see device_write
 */
static int gpu_decode_instruction(char *buffer)
{
    int i;
    int j;
    int aux = 0;
		
    for (i = 7, j = 0; i >= 4; i--, j++) {
        aux |= (int) ((unsigned char) buffer[i]) << (j * 8);
    }

    *DATA_A_ptr = aux;
    
    aux = 0;

    for (i = 3, j = 0; i >= 0; i--, j++) {
        aux |= (int) ((unsigned char) buffer[i]) << (j * 8);
    }
    
    *DATA_B_ptr = aux;
    
    printk(KERN_INFO "Successfully transferred data to DATA_A and DATA_B\n");
    printk(KERN_INFO "DATA_A value: %d\n", (*DATA_A_ptr));
    printk(KERN_INFO "DATA_B value: %d\n", (*DATA_B_ptr));
    
    return 0;
}

static ssize_t gpu_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    char buffer[MIN_BUFFER_SIZE];

    ssize_t size = min(sizeof(buffer), len);

    if (len < MIN_BUFFER_SIZE) {
        printk(KERN_ALERT "Buffer underflow\n");
        return -EINVAL;
    }
    
    *off = 0;
    *WRREG_ptr = 0;

    if(copy_from_user(buffer, buff, len)){
    	return -1;
    }
    
    if(size == 0) {
    	return 0;
    }

    printk(KERN_INFO "Received buffer\n");

    while ((*WRFULL_ptr)) {}

    gpu_decode_instruction(buffer);

    *WRREG_ptr = 1;
    *WRREG_ptr = 0;

    return size;
}

static int gpu_open(struct inode *device_file, struct file *instance) {
	printk(KERN_INFO "%s: OPEN\n", DEVICE_NAME);
	return 0;
}

static int gpu_release(struct inode *device_file, struct file *instance) {
	printk(KERN_INFO "%s: CLOSE\n", DEVICE_NAME);
	return 0;
}

static int __init gpu_driver_init(void)
{
	void *LW_virtual;
	int result;
	int major;
	
    result = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    
    if (result < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", result);
        return result;
    }
    
    major = MAJOR(dev);
    dev = MKDEV(major, 0);
    
    kernel_cdev = cdev_alloc();

    kernel_cdev->ops = &fops;
    kernel_cdev->owner = THIS_MODULE;
    
    result = cdev_add(kernel_cdev, dev, 1);
    
    if (result < 0) {
        cdev_del(kernel_cdev);
        unregister_chrdev_region(dev, 1);
    	printk(KERN_INFO "Unable to add cdev\n");
    	return result;
    }

    LW_virtual = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

    DATA_A_ptr = (int *) (LW_virtual + DATA_A_BASE);
    DATA_B_ptr = (int *) (LW_virtual + DATA_B_BASE);
    WRREG_ptr = (int *) (LW_virtual + WRREG_BASE);
    WRFULL_ptr = (int *) (LW_virtual + WRFULL_BASE);

    printk(KERN_INFO "I was assigned major number %d. Create a dev file with 'mknod /dev/%s c %d 0'.\n", major, DEVICE_NAME, major);
    return 0;
}

static void __exit gpu_driver_exit(void)
{
    cdev_del(kernel_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "GPU driver module unloaded.\n");
}

module_init(gpu_driver_init);
module_exit(gpu_driver_exit);
