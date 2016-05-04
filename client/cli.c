#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include "kercli.h" 

#define PATH "/dev/kercli" /* Device file */

/* Used on results of MEMINFO request */
void print_meminfo(struct mem_infos mi)
{
  printf("Total usable main memory size: %llu\n", mi.totalram);
  printf("Amount of shared memory : %llu\n", mi.sharedram);
  printf("Available memory size: %llu\n", mi.freeram);
  printf("Memory used by buffers: %llu\n", mi.bufferram);
  printf("Total high memory size: %llu\n", mi.totalhigh);
  printf("Available high memory size: %llu\n", mi.freehigh);
  printf("Memory unit size in bytes: %lu\n", mi.mem_unit);
}

/* Prints help */
void print_help () {
  printf("Usage : $ cli <argument>\n");
  printf("Arguments:\n");
  printf("list - Shows runnig stuff\n");
  printf("fg <id> - Blocking till <id> completes. Recovers exit status.\n");
  printf("kill <signal> <pid> - Sends <signal> to <pid>\n");
  printf("wait <pid> [<pid> ...] - Blocking till `<pid>`"
	 "completes, recovers `pid` and exit status\n");
  printf("meminfo - Prints info about memory, similar to "
	 "/proc/meminfo using si_meminfo and si_swapinfo\n");
  printf("modinfo <name> - Prints info (author, version, "
	 "parameters, address) about kernel module <name>\n");
}


int main(int argc, char ** argv)
{
  struct mem_infos mem_info;
  struct pid_list pids;
  char mod_list[LS_SIZE]; /* `list` data */
  int fd, // File descriptor 
    ret,
    *args, 
    nb_args,
    i,
    tmp;
  fd = open(PATH, O_WRONLY);
  if (fd < 0) {
    perror("file error");
    exit(-1);
  }

  /* Get arguments */
  nb_args = argc-2;
  args = (int *) malloc(sizeof(int) * nb_args);
  if (nb_args > 0) {
    for (i=2 ; i<argc ; i++) {
      sscanf(argv[i], "%d", &tmp);
      args[i-2]=tmp;
    }
  }
			   
  /* Argument parsing */
  if (strcmp("list", argv[1]) == 0) { // LIST
    if (ioctl(fd, LIST, mod_list) == -1)
      perror("ioctl - list");
    else {
      printf("module - size - used by");
      printf("%s", mod_list);
    }
    
  } else if (strcmp("fg", argv[1]) == 0) { // FG
    // Do FG stuff here 
    
  } else if (strcmp("kill", argv[1]) == 0) { // KILL
    struct killerstruct ks = {
      .sid = args[0],
      .pid = args[1]
    };
    if (ioctl(fd, KILL, &ks)== -1)
      perror("ioctl - kill");
    
  } else if (strcmp("wait", argv[1]) == 0) { // WAIT
    pids.nb_element = nb_args;
    pids.array_pointer = args;
    ret = ioctl(fd, WAIT, &pids);
    if (ret < 0)
      perror("ioctl - wait");
    else
      printf("%d terminated!\n", pids.return_value);

  } else if (strcmp("meminfo", argv[1]) == 0) { // MEMINFO
    if (ioctl(fd, MEMINFO, &mem_info) == -1)
      perror("ioctl - meminfo");
    else
      print_meminfo(mem_info);


  } else if (strcmp("modinfo", argv[1]) == 0) { // MODINFO
    if (ioctl(fd, MODINFO, &mod_list) == -1)
      perror("ioctl - modinfo");
        
  } else {
    print_help(); 
  }

  close(fd);
  return 0; 
}
