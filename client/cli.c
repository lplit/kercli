#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
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
  printf("Usage : $ %s <argument>\n", argv[0]);
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
  char mod_list[LS_SIZE]; /* `list` data */
  int fd; 
  fd = open(PATH, O_WRONLY);
  if (fd < 0) {
    perror("file error");
    exit(-1);
  }

  /* Arguments parsing */
  if (strcmp("list", argv[1]) == 0) {
    if (ioctl(fd, LIST, mod_list) == -1)
      perror("ioctl - list");
    else {
      printf("module - size - used by");
      printf("%s", mod_list);
    }
    
  } else if (strcmp("fg", argv[1]) == 0) {
    // Do FG stuff here 

  } else if (strcmp("kill", argv[1]) == 0) {
    kill(args[0], args[1]);

  } else if (strcmp("wait", argv[1]) == 0) {
    plist.nb_element = nb_args;
    plist.array_pointer = args;
    ret = ioctl(fd, WAIT_ONE, &plist);
    if (ret < 0)
      perror("ioctl : wait");
    else
      printf("%d terminated!\n", plist.return_value);

  } else if (strcmp("meminfo", argv[1]) == 0) {
    if (ioctl(fd, MEMINFO, &mem_info) == -1)
      perror("meminfo : IOCTL");
    else
      display_info(mem_info);


  } else if (strcmp("modinfo", argv[1]) == 0) {
    // Do MODINFO stuff here 
        
  } else if (strcmp("help", argv[1]) == 0) {
    print_help();

  } else {
    print_help(); 
  }

  close(fd);
  return 0; 
}
