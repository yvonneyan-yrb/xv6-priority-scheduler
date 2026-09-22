#include "kernel/types.h"
#include "user/user.h"
#include "user/uoa.h"

int
exectime(int argc, char *argv[])
{
  int pid;
  int start_ticks = 0, end_ticks = 0;

  if (argc < 1) {
    fprintf(2, "usage: uoa_exectime command [args...]\n");
    return -1;
  }

  start_ticks = uptime();
  //fprintf(1, "uptime: %d\n", start_ticks);

  pid = fork();

  if (pid < 0) {
    fprintf(2, "uoa_exectime: fork failed\n");
    return -1;
  } else if (pid == 0) {
    if (exec(argv[0], argv + 0) < 0) {
      fprintf(2, "uoa_exectime: exec %s failed\n", argv[1]);
      return -1;
    }
  } else {
    wait(0);

    end_ticks = uptime();
    //fprintf(1, "uptime: %d\n", end_ticks);
  }

  return end_ticks - start_ticks;
}

int
execwaittime(int argc, char *argv[])
{
  int pid;
  uint runnable_ticks = 0, sleeping_ticks = 0;

  pid = fork();

  if (pid < 0) {
    fprintf(2, "execwaittime: fork failed\n");
    return -1;
  } else if (pid == 0) {
    /* child */
    if (exec(argv[0], argv) < 0) {
      fprintf(2, "execwaittime: exec %s failed\n", argv[0]);
      return -1;
    }
  } else {
    /* parent */
    int child_pid = waitx(0, &runnable_ticks, &sleeping_ticks);

    if (child_pid < 0) {
      fprintf(2, "execwaittime: waitx failed\n");
      return -1;
    }
  }
  return runnable_ticks + sleeping_ticks;
}

int
execpwaittime(int argc, int priority, char *argv[])
{
  int pid;
  uint runnable_ticks = 0, sleeping_ticks = 0;

  pid = fork();

  if (pid < 0) {
    fprintf(2, "execpwaittime: fork failed\n");
    return -1;
  } else if (pid == 0) {
    if (execp(argv[0], priority, argv) < 0) {
      fprintf(2, "execpwaittime: exec %s p %d failed\n", argv[0], priority);
      return 01;
    }
  } else {
    int child_pid = waitx(0, &runnable_ticks, &sleeping_ticks);

    if (child_pid < 0) {
      fprintf(2, "execpwaittime: waitx failed\n");
      return -1;
    }
  }
  return runnable_ticks + sleeping_ticks;
}
