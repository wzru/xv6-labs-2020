#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
  int p[2][2];
  char buf[1];
  pipe(p[0]);
  pipe(p[1]);
  if (fork()) {
    write(p[0][1], "i", 1);
    read(p[1][0], buf, 1);
    fprintf(1, "%d: received p%cng\n", getpid(), buf[0]);
  } else {
    read(p[0][0], buf, 1);
    fprintf(1, "%d: received p%cng\n", getpid(), buf[0]);
    write(p[1][1], "o", 1);
  }
  exit(0);
}
