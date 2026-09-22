#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/uoa.h"

int
main(int argc, char *argv[])
{
  int time;
  if ((time = exectime(argc-1, argv+1)) < 0) {
    exit(1);
  }
  printf("Ticks taken: %d\n", time);
  exit(0);
}
