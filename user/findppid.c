#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "Usage: findppid <num>\n");
    exit(1);
  }

  int num = atoi(argv[1]);

  for (int i = 0; i < num; i++) {
    int pid = fork();
    if (pid < 0) {
      fprintf(2, "findppid: fork failed\n");
      exit(1);
    }
    if (pid > 0) {
      // Parent waits for its child to finish first, then prints and exits
      wait(0);
      printf("my ID is %d, my parent ID is %d\n", getpid(), getppid());
      exit(0);
    }
    // Child process continues loop to create the next generation
  }

  // Deepest child process reaches here (has no children)
  printf("my ID is %d, my parent ID is %d\n", getpid(), getppid());
  exit(0);
}