#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/pid.h>
#include <linux/fs.h>
#include <linux/mm.h>

#include "kercli.h"


// Infos 
MODULE_LICENCE("GPL");
MODULE_AUTHOR("Michal Rudek, Saalik Hatia");
MODULE_DESCRIPTION("UPMC-MSAR-PNL Kernel CLI");