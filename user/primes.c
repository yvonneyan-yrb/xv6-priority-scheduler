#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Iterative pipeline stage: each process handles one prime filter
void
sieve(int left_pipe[2])
{
  int input_fd = left_pipe[0];
  close(left_pipe[1]); // Close unused write end of incoming pipe

  while (1) {
    int prime;
    // Read the first number from the left pipe; this is the current prime
    if (read(input_fd, &prime, sizeof(prime)) == 0) {
      close(input_fd);
      exit(0); // No more numbers left to sieve
    }

    // Print the discovered prime
    printf("%d\n", prime);

    int right_pipe[2];
    if (pipe(right_pipe) < 0) {
      fprintf(2, "primes: pipe failed\n");
      exit(1);
    }

    int pid = fork();
    if (pid < 0) {
      fprintf(2, "primes: fork failed\n");
      exit(1);
    }

    if (pid == 0) {
      // Child process: becomes the next stage in the pipeline
      close(input_fd);            // Close old input descriptor
      input_fd = right_pipe[0];   // Read from new pipe's read end
      close(right_pipe[1]);       // Close write end in child before looping
      continue;                   // Loop around to process next prime stage
    } else {
      // Parent process: filter out multiples of current prime
      close(right_pipe[0]); // Close read end of outgoing pipe

      int num;
      while (read(input_fd, &num, sizeof(num)) > 0) {
        if (num % prime != 0) {
          write(right_pipe[1], &num, sizeof(num));
        }
      }

      // Cleanup and wait for child process chain to finish
      close(input_fd);
      close(right_pipe[1]);
      wait(0);
      exit(0);
    }
  }
}

int
main(int argc, char *argv[])
{
  int limit = 35; // Default limit if no argument is provided

  if (argc >= 2) {
    limit = atoi(argv[1]);
  }

  if (limit > 200) {
    limit = 200; // Enforce maximum limit specified in task
  }

  int p[2];
  if (pipe(p) < 0) {
    fprintf(2, "primes: pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "primes: fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    // Start the first sieve process with initial pipeline input
    sieve(p);
    exit(0);
  } else {
    // Main generator process: feeds numbers from 2 up to 'limit'
    close(p[0]);

    for (int i = 2; i <= limit; i++) {
      write(p[1], &i, sizeof(i));
    }

    close(p[1]);
    wait(0);
    exit(0);
  }
}