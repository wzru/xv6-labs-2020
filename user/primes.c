#include "kernel/types.h"
#include "user/user.h"

#define MAXN 35
#define BUF_SIZE 128

// int is_prime(int x) {
//   if (x <= 1)
//     return 0;
//   for (int i = 2; i < x; ++i)
//     if (x % i == 0)
//       return 0;
//   return 1;
// }

int main(int argc, char **argv) {
  int fd[MAXN][2];
  int cnt = 0, p, n;
  pipe(fd[0]);
  for (int i = 2; i <= MAXN; ++i)
    write(fd[0][1], &i, sizeof(int));
  close(fd[0][1]);
  while (read(fd[cnt][0], &p, sizeof(int))) {
    fprintf(1, "prime %d\n", p);
    pipe(fd[++cnt]);
    if (fork() == 0) {
      while (read(fd[cnt - 1][0], &n, sizeof(int)) > 0) {
        if (n % p != 0) {
          //   fprintf(1, "n=%d\n", n);
          write(fd[cnt][1], &n, sizeof(int));
        }
      }
      close(fd[cnt - 1][0]);
      close(fd[cnt][1]);
      exit(0);
    } else {
      int res;
      wait(&res);
      close(fd[cnt][1]); // important
    }
  }
  exit(0);
}
