#include <asm-generic/uaccess.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/pid.h>
#include <linux/fs.h>
#include <linux/mm.h>

#include "../inc/kercli.h"

// Infos 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michal Rudek, Saalik Hatia");
MODULE_DESCRIPTION("UPMC-MSAR-PNL Kernel CLI");

/* Driver major number */
int d_maj;

long device_handler(struct file *f, unsigned int req_num, 
	unsigned long arg_p);

static const struct file_operations f_ops = {
  .unlocked_ioctl = device_handler,
};

static int __init init (void)
{
  d_maj = register_chrdev(0, "kercli", &f_ops);
  pr_alert("Device registered, major : %d\n", d_maj);
  return 0; 
}

static void __exit end (void)
{
  unregister_chrdev(d_maj, "krecli");
  pr_alert("kercil byebye!\n");
}

long device_handler(struct file *f, unsigned int req_num, unsigned long arg_p)
{
  return 0;
}

module_init(init);
module_exit(end);
