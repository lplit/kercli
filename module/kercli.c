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

long device_handler(struct file *f,
		    unsigned int req_num, 
		    unsigned long arg_p);

static const struct file_operations f_ops = {
  .unlocked_ioctl = device_handler,
};

/* Init routine */
static int __init init (void)
{
  d_maj = register_chrdev(0, "kercli", &f_ops);
  pr_alert("Device registered, major : %d\n", d_maj);
  return 0; 
}

/* Unload routine */
static void __exit end (void)
{
  unregister_chrdev(d_maj, "krecli");
  pr_alert("kercil byebye!\n");
}

/* Copies sysinfo structure to userspace */
static void send_mem_info(void *arg_p)
{
  struct sysinfo values;
  memset(&values, 0, sizeof(struct sysinfo));
  si_meminfo(&values);
  copy_to_user((void *)arg_p, &values, sizeof(struct mem_infos));
  pr_debug("Memory info sent\n");
}

/**
 * Request handler
 * goes like: 
 * struct mem_infos m; 
 * ioctl(fd, MEMINFO, &m);
 * print_meminfo(m);
 */
long device_handler(struct file *f,
		    unsigned int req_num,
		    unsigned long arg_p)
{


  switch (req_num) {
  case LIST:
    
    break;

  case FG:
    
    break;

  case KILL:

    break;

  case WAIT:

    break;

  case MEMINFO:
    send_mem_info((void *) arg_p);
    break;

  case MODINFO:

    break;

  default:
    pr_alert("Unknown request code : %d\n", req_num); 
    return -1; 
  }
  
  return 0;
}

module_init(init);
module_exit(end);
