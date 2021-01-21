#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char buf[512], *p = buf, *xargv[MAXARG] = {0};
  int xargc = argc - 1;
  for (int i = 1; i < argc; ++i)
    xargv[i - 1] = argv[i];
  while (read(0, p, sizeof(char)) > 0) {
    if (*p == '\n') {
      *p = 0;
      xargv[xargc] = malloc(p - buf + 1);
      memcpy(xargv[xargc++], buf, p - buf + 1);
      p = buf;
      if (fork() == 0) {
        exec(xargv[0], xargv);
        exit(0);
      } else {
        int res;
        wait(&res);
        xargc = argc - 1;
      }
    } else
      ++p;
  }
  if (p != buf) {
    xargv[xargc] = malloc(p - buf + 1);
    memcpy(xargv[argc++], buf, p - buf + 1);
    p = buf;
    if (fork() == 0) {
      exec(xargv[0], xargv);
      exit(0);
    } else {
      int res;
      wait(&res);
    }
  }
  exit(0);
}