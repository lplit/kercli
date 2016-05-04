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

struct wait_struct {
  struct delayed_work w_delayed;
  struct task_struct **w_pids;
  int w_is_finished;
  int w_size;
};

DECLARE_WAIT_QUEUE_HEAD(waiter);

/**
 * METHODS 
 */

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

/* Used by wait */
static void wait_checker(struct work_struct *w)
{
  struct delayed_work * w_del;
  struct wait_struct * w_stru;
  int i;

  w_del = to_delayed_work(w);
  w_stru = container_of(w_del, struct wait_struct, w_delayed);

  wake_up_interruptible(&waiter);

  for (i = 0; i < w_stru->w_size; i++) {
    if (!pid_alive(w_stru->w_pids[i])) {
      w_stru->w_is_finished = i;
      wake_up_interruptible(&waiter);
      return;
    }
  }

  /* On relance la boucle */
  queue_delayed_work(syndicate, &(wOs->w_dw), TICKS_DELAY);
}
}

/* Blocks, wait for first process to finish */
/**
 * TODO: 
 * [x] 1. Get array size 
 * [x] 2. Get array 
 * [x] 3. Init a delayed work queue 
 * [x] 4. Check each PID (need a label for brute exit?)
 * [x] 5. Start delayed work with recovered PIDs
 * [ ] 6. Wait for event from worker
 * [x] 7. Cleanup
 */
static long wait(void *arg)
{
  struct pid_list p_list;
  struct pid *pid; 
  struct wait_struct * wt_s;
  int i, *array;

  // 1
  copy_from_user(&p_list, arg, sizeof(struct pid_list));
  array = kmalloc_array(p_list.nb_element, sizeof(int), GFP_KERNEL);
  if (array==NULL) {
    pr_alert("wait - allocation");
    return -1;
  }

  // 2
  copy_from_user(array, p_list.array_pointer, sizeof(int) * p_list.nb_element);

  // 3
  wt_s=kmalloc(sizeof(struct wait_struct), GFP_KERNEL);
  wt_s->w_pids=kmalloc(sizeof(struct task_struct *) * p_list.nb_element,
		       GFP_KERNEL);
  wt_s->w_is_finished=-1;
  INIT_DELAYED_WORK(&(wt_s->w_delayed), wait_checker);

  // 4
  wt_s->w_size = p_list.nb_element;
  for (i=0 ; i < p_list.nb_element ; i++) {
    if ((p = find_get_pid(array[i]))==0) {
      pr_err("wait - find_get_pid");
      goto err; 
    }
    if ((wt_s->w_pids[i]=get_pid_task(p, PIDTYPE_PID))==0) {
      pr_err("wait - get_pid_task");
      goto err;
    }
    put_pid(p);
  }

  // 5
  if (queue_delayed_work(syndicate, &(wt_s->w_delayed), 100) == 0) {
    pr_alert("wait - queue\n");
    goto err; 
  }

  // 6
  wait_even_interruptible(waiter, wt_s->w_is_finished != -1);

  // 7
  for(i=0 ; i<p_list.nb_element ; i++) {
    put_task_struct(wt_s->w_pids[i]);
    p_list->return_value = array[wt_s->w_is_finished];
    copy_to_user(arg, &p_list, sizeof(struct pid_list));
  }
  
  kfree(array);
  kfree(wt_s->w_pids);
  kfree(wt_s);
  return 0;
 err:
  kfree(array);
  kfree(wt_s->w_pids);
  kfree(wt_s);
  return -1;
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

/* Returns few infos about module */ 
static void find_mod(void *arg_p)
{
  struct module *m; 
  int buff_remaining, references;
  char buffer[256], * ret;
  
  buff_remaining = LS_SIZE;
  ret = kzalloc(LS_SIZE, GFP_KERNEL);
  
  /* built-in function */
  m=find_module((char*) arg_p);

  references=atomic_read(&(m->mkobj.kobj.kref.refcount));
  buff_remaining -= scnprintf(buffer, 128, "%s %u %d\n",
			      m->name, m->core_size, references);
  
  strncat(ret, buffer, buff_remaining);
  copy_to_user((void *)arg_p, ret, LS_SIZE);
  kfree(ret);
  
}

/* Sends all the running mods infos to userspace */
static void send_mods_list(void *arg_p)
{
  struct module *m; 
  int buff_remaining, references;
  char buffer[256], * ret;
  
  buff_remaining = LS_SIZE;
  ret = kzalloc(LS_SIZE, GFP_KERNEL);
  
  /* Get the first one */
  m=THIS_MODULE;
  references=atomic_read(&(m->mkobj.kobj.kref.refcount));
  buff_remaining -= scnprintf(buffer, 128, "%s %u %d\n",
			      m->name, m->core_size, references);

  /* Concat to the return */
  strncat(ret, buffer, buff_remaining);

  /* Iterate over rest */
  list_for_each_entry(m, &(THIS_MODULE->list), list) {
    references=atomic_read(&(m->mkobj.kobj.kref.refcount));
    /* If running, concat */
    if (m->state == 0) {
      buff_remaining -= scnprintf(buffer, 128, "%s %u %d\n",
				  m->name, m->core_size, references);
      strncat(ret, buffer, buff_remaining);
    }
  }
  copy_to_user((void *)arg_p, ret, LS_SIZE);
  kfree(ret); 
}

/** Sends a signal to a pid
 * from killerstruct structure */
static void signal_send(void * arg_p)
{
  struct pid *pid; 
  struct killerstruct ks;
  copy_from_user(&ks, arg_p, sizeof(struct killerstruct));

  if ((pid = find_get_pid(ks.pid)))
    kill_pid(pid, ks.sid, 1);
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
    send_mods_list((void *) arg_p);
    break;

  case FG:
    pr_debug("FG is not implemented\n");
    break;

  case KILL:
    signal_send((void *) arg_p); 
    break;

  case WAIT:
    
    break;

  case MEMINFO:
    send_mem_info((void *) arg_p);
    break;

  case MODINFO:
    find_mod((void *) arg_p);
    break;

  default:
    pr_alert("Unknown request code : %d\n", req_num); 
    return -1; 
  }
  
  return 0;
}

module_init(init);
module_exit(end);
