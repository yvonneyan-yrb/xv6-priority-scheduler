#include "kernel/types.h"
#include "user/user.h"
#include "user/uoa.h"

long calc_primes(long max) {
  long count = 0;
  for (long i = 2; i < max; i++) {
    int is_prime = 1;
    for (long j = 2; j * j <= i; j++) {
      if (i % j == 0) {
        is_prime = 0;
        break;
      }
    }
    if (is_prime)
      count++;
  }
  return count;
}

int main(int argc, char *argv[]) {
  int level = 1;
  long limit = 1000000;
  if (argc == 2) {
    level = atoi(argv[1]);
  }
  limit *= level;
  //printf("Starting heavy integer computation at level %d...\n", level);
  volatile long primes_found = calc_primes(limit);
  UNUSED(primes_found);

  //printf("Finished computation. Primes found: %d\n", (int)primes_found);
  exit(0);
}
