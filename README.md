# kercli

PNL-4I402 Project

## Description 
Project will implement a character driver module for Linux kernel, to offer the following: 

| Command | Description |
| ------- | ----------- |
| `list` | Shows runnig stuff |
| `fg <id>` | Blocking till `<id>` completes. Recovers exit status. | 
| `kill <signal> <pid>` | Sends `<signal>` to `<pid>` using `kill_pid` kernel method | 
| `wait <pid> [<pid> ...]` | Blocking till `<pid>` completes, recovers `pid` and exit status | 
| `meminfo` | Prints info about memory, similar to `/proc/meminfo` using `si_meminfo` and `si_swapinfo` |
| `modinfo <name>` | Prints info (author, version, parameters, address) about kernel module `<name>` |

## Implementation progress
 - [ ] `list` 
 - [ ] `fg <id >`
 - [ ] `kill <signal> <pid>` 
 - [ ] `wait <pid> [<pid> ...]`
 - [ ] `meminfo`
 - [ ] `modinfo <name>`

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisities

What things you need to install the software and how to install them

```
Give examples
```

## Install

### Patching kernel

### Inserting module 

### Interacting

### Removing module 

## Built With

* Dropwizard - Bla bla bla
* Maven - Maybe
* Atom - ergaerga

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc
