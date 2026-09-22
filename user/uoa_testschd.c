#include "kernel/types.h"
#include "user/user.h"
#include "user/uoa.h"

int
do_cpubound(int i, int priority)
{
  int time;
  char *args[] = {"uoa_cpubound", "3", 0};
  time = execpwaittime(2, priority, args);
  printf("cpu p=%d: %d\n", priority, time);
  return time;
}

int
do_iobound(int i, int priority)
{
  int time;
  char num[] = "0iobound";
  num[0] += i;
  char *args[] = {"uoa_iobound", num, 0};
  time = execpwaittime(2, priority, args);
  printf("io p=%d: %d\n", priority, time);
  return time;
}

int
main(int argc, char *argv[])
{
  int num_processes = 5;
  int cpu_pipe[2];
  int io_pipe[2];
  int i;

  if (pipe(cpu_pipe) < 0 || pipe(io_pipe) < 0) {
    printf("Pipe creation failed.\n");
    exit(1);
  }

  for (i = 0; i < num_processes; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("Fork failed.\n");
      exit(1);
    }
    else if (pid == 0) {
      int io_pid = fork();
      if (io_pid == 0) {
        int time = do_iobound(i, 13);
        write(io_pipe[1], &time, sizeof(time));
        exit(0);
      } else {
        int time = do_cpubound(i, 12);
        write(cpu_pipe[1], &time, sizeof(time));
        wait(0);
        exit(0);
      }
    }
  }

  close(cpu_pipe[1]);
  close(io_pipe[1]);

  while (wait(0) != -1)
    ;

  int time;
  int cpu_sum = 0;
  int io_sum = 0;

  for (i = 0; i < num_processes; i++) {
    read(cpu_pipe[0], &time, sizeof(time));
    cpu_sum += time;
  }

  for (i = 0; i < num_processes; i++) {
    read(io_pipe[0], &time, sizeof(time));
    io_sum += time;
  }

  printf("Average CPU bound wait time: %d\n", cpu_sum / num_processes);
  printf("Average IO bound wait time: %d\n", io_sum / num_processes);
  
  exit(0);
}
