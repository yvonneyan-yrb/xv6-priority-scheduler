#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2]; // Parent <-> Child 1 pipes
  int p3[2], p4[2]; // Parent <-> Child 2 pipes
  char buf[8];

  // Create pipes for both child processes
  if (pipe(p1) < 0 || pipe(p2) < 0 || pipe(p3) < 0 || pipe(p4) < 0) {
    fprintf(2, "pingpong: pipe failed\n");
    exit(1);
  }

  // Fork Child 1
  int pid1 = fork();
  if (pid1 < 0) {
    fprintf(2, "pingpong: fork 1 failed\n");
    exit(1);
  }

  if (pid1 == 0) {
    // --- Child 1 ---
    close(p3[0]); close(p3[1]); // Close unused Child 2 pipes
    close(p4[0]); close(p4[1]);
    close(p1[1]); // Close write end of P->C1
    close(p2[0]); // Close read end of C1->P

    if (read(p1[0], buf, 4) > 0) {
      printf("%d: pong\n", getpid());
      write(p2[1], "ACK", 3);
    }

    close(p1[0]);
    close(p2[1]);
    exit(0);
  }

  // Fork Child 2
  int pid2 = fork();
  if (pid2 < 0) {
    fprintf(2, "pingpong: fork 2 failed\n");
    exit(1);
  }

  if (pid2 == 0) {
    // --- Child 2 ---
    close(p1[0]); close(p1[1]); // Close unused Child 1 pipes
    close(p2[0]); close(p2[1]);
    close(p3[1]); // Close write end of P->C2
    close(p4[0]); // Close read end of C2->P

    if (read(p3[0], buf, 4) > 0) {
      printf("%d: pong\n", getpid());
      write(p4[1], "ACK", 3);
    }

    close(p3[0]);
    close(p4[1]);
    exit(0);
  }

  // --- Parent Process ---
  close(p1[0]); // Close read end of P->C1
  close(p2[1]); // Close write end of C1->P
  close(p3[0]); // Close read end of P->C2
  close(p4[1]); // Close write end of C2->P

  // 1. Send "ping" to both child processes
  write(p1[1], "ping", 4);
  write(p3[1], "ping", 4);

  // 2. Wait for both "ACK" messages
  read(p2[0], buf, 3);
  read(p4[0], buf, 3);

  close(p1[1]);
  close(p2[0]);
  close(p3[1]);
  close(p4[0]);

  // 3. Ensure all child processes have exited
  wait(0);
  wait(0);

  exit(0);
}