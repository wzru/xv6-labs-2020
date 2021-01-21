#include "kernel/types.h"
#include "user/user.h"

#define MAXN 35

void sieve(int input) {
  int n, p, fd[2];
  if (read(input, &p, sizeof(int)) > 0) {
    fprintf(1, "prime %d\n", p);
    pipe(fd);
    if (fork() == 0) {
      close(fd[1]);
      sieve(fd[0]);
    } else {
      close(fd[0]);
      while (read(input, &n, sizeof(int)) > 0) {
        if (n % p != 0)
          write(fd[1], &n, sizeof(int));
      }
      close(fd[1]);
      close(input);
      int res;
      wait(&res);
    }
  }
}

int main(int argc, char **argv) {
  int fd[2];
  pipe(fd);
  for (int i = 2; i <= MAXN; ++i)
    write(fd[1], &i, sizeof(int));
  close(fd[1]);
  sieve(fd[0]);
  exit(0);
}
