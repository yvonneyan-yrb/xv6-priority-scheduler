#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

/* modified stressfs.c that cleans itself up after completion. */

int
main(int argc, char *argv[])
{
  int fd, i;
  char *base_path = "iobound";
  char path[32];
  char data[512];

  strcpy(path, base_path);
  
  if (argc == 2) {
    if (strlen(argv[1]) >= 32) {
      printf("uoa_iobound: filename arg too long!\n");
      exit(1);
    }
    strcpy(path, argv[1]);
  }

  //printf("uoa_iobound starting\n");
  memset(data, 'a', sizeof(data));

  //printf("write %d\n", i);

  fd = open(path, O_CREATE | O_RDWR);
  if (fd < 0) {
    printf("iobound: cannot open %s\n", path);
    exit(1);
  }

  for (i = 0; i < 100; i++) {
    write(fd, data, sizeof(data));
  }
  close(fd);

  //printf("read\n");

  fd = open(path, O_RDONLY);

  for (i = 0; i < 100; i++) {
    read(fd, data, sizeof(data));
  }
  close(fd);

  unlink(path);

  exit(0);
}
