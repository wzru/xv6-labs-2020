#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

char *fmtname(char *path) {
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = '\0';
  return buf;
}

void find(char *path, char *target) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  if (st.type == T_DIR) {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("find: path too long\n");
      return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0) {
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      char *name = fmtname(buf);
    //   printf("buf=%s name=%sn\n", buf, name);
      if (!strcmp(name, target)) {
        printf("%s\n", buf);
      }
      if (st.type == T_DIR && strcmp(name, ".") && strcmp(name, "..")) {
        find(buf, target);
      }
    }
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "usage: %s [path...] [target]\n", argv[0]);
  }
  if (argc == 2) {
    find(".", argv[1]);
    exit(0);
  }
  for (int i = 1; i < argc - 1; ++i) {
    find(argv[i], argv[argc - 1]);
  }
  exit(0);
}