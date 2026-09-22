#define UNUSED(x) ((void)(x))

/* syscalls */
int getwaittime(int pid, uint *runnable, uint *sleeping);
int waitx(int *status, uint *runnable, uint *sleeping);
int execp(const char* path, int priority, char** argv);

/* uoalib.c */
int exectime(int, char **);
int execwaittime(int, char**);
int execpwaittime(int, int, char**);
