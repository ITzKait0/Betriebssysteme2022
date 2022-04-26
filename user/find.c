#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path) {
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ) return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}

void find(char *name, char *path) {

  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(strstr(name,fmtname(path)) == 1){
        printf("1  %s\n", path);
    }
    break;

  case T_DIR:
    if(strstr(name,fmtname(path)) == 1){
        printf("2  %s\n", path);
    }
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if (strcmp(de.name, "..") == 0 || strcmp(de.name, ".") == 0 || strlen(de.name) == 0) {
        continue;
      }
      if(de.inum == 0)
        continue;
      strcpy(p,de.name);
      find(name,buf);
    }
    break;
  }
  close(fd);
  return;
}

int main(int argc, char *argv[]) {
  if (argc >= 2) {
    for(int i = 1;i<argc;i++){
      find(argv[i], ".");  
    }
    
  }
  exit(0);
}
