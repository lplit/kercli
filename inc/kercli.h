#ifndef KERCLI_H
#define KERCLI_H

#define LOLO 1 

struct mem_infos {
	long long uptime;				/* Seconds since boot */
	unsigned long long loads[3];	/* 1, 5, and 15 minute load averages */
	unsigned long long totalram;	/* Total usable main memory size */
	unsigned long long freeram;		/* Available memory size */
	unsigned long long sharedram;	/* Amount of shared memory */
	unsigned long long bufferram;	/* Memory used by buffers */
	unsigned long long totalswap;	/* Total swap space size */
	unsigned long long freeswap;	/* swap space still available */
	unsigned short procs;           /* Number of current processes */
	unsigned short pad;	        	/* Explicit padding for m68k */
	unsigned long long totalhigh;	/* Total high memory size */
	unsigned long long freehigh;	/* Available high memory size */
	unsigned long mem_unit;	        /* Memory unit size in bytes */
};


#endif 