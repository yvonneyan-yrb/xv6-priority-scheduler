#ifndef _PINFO_H_
#define _PINFO_H_

#include "param.h"

struct pinfo {
  int inuse[NPROC];      // whether this slot in process table is in use (1 or 0)
  int pid[NPROC];        // PID of each process
  int priority[NPROC];   // current priority level (1 or 2)
  int ticks[NPROC][2];   // ticks[i][0] = ticks at prio 1, ticks[i][1] = ticks at prio 2
};

#endif