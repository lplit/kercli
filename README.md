# kercli

UPMC-Paris6 - M1 SAR 2015-2016 - PNL-4I402 Project

## Implementation progress
 - [x] `list` 
 - [ ] `fg <id >`
 - [x] `kill <signal> <pid>` 
 - [x] `wait <pid> [<pid> ...]`
 - [x] `meminfo`
 - [x] `modinfo <name>`


## Description 
Project will implement a character driver module for Linux kernel, and a client allowing interaction

## Client

Client is invoked : 
` $ client <option> `

| Options | Description |
| ------- | ----------- |
| `list` | Shows runnig stuff |
| `fg <id>` | Blocking till `<id>` completes. Recovers exit status. | 
| `kill <signal> <pid>` | Sends `<signal>` to `<pid>` using `kill_pid` kernel method | 
| `wait <pid> [<pid> ...]` | Blocking till `<pid>` completes, recovers `pid` and exit status | 
| `meminfo` | Prints info about memory, similar to `/proc/meminfo` using `si_meminfo` and `si_swapinfo` |
| `modinfo <name>` | Prints info (author, version, parameters, address) about kernel module `<name>` |

## Driver

All driver requests are passed via `ioctl` function.

```c
void meminfo ()
{
  infos i;
  if (ioctl(fd, MEMINFO, &i) == -1)
   perror("meminfo : IOCTL");
  printf "Memory info" + i.stuff
}
```

| Request code | Description | Client calls by |
| ------------ | ----------- | ---------- |
| `LSRCMS` | **L**i**S**t **R**unning **C**o**M**mand**S** | `list` |
| `FG` | Awaits end of com`<id>` returning its return code. Blocking. | `fg <id>` |
| `KILL` | Sends signal via `kill_pid` | `kill <signal> <pid>` | 
| `WAIT` | Wait for a process |  `wait <pid> [<pid> ...]` |
| `MEMINFO` | Get memory info | `meminfo` |
| `MODINFO` | Get module info | `modinfo <name>` |

## Install
### Compiling 
Before compiling please do provide path to 4.2.3 kernel in `/module/Makefile` under `KERNELDIR`.
Then, simply run : 

 `$ make`

### Inserting module 

 `$ insmod kercli.ko`

### Removing module 

 `$ rmmode kercli`


## License

This project is licensed under the MIT License.
