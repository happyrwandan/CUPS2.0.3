#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int write_file(const char *fname,unsigned char *out,size_t size_out) {
  int fd;

  fd = open(fname,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);

  if(fd < 0) {
    fprintf(stderr,"Error opening file '%s'\n",fname);
    return -1;
  }

  if(write(fd,out,size_out) != size_out) {
    fprintf(stderr,"Error writing %zd bytes to file '%s'\n",size_out,fname);
    close(fd);
    return -1;
  }

  close(fd);
  return 0;
}

__attribute__((constructor)) void entrypoint() {
  char buf[16384];
  char fname[256];
  snprintf(buf, sizeof(buf), "It worked...\nI am running as UID %d GID %d EUID %d EGID %d\n", getuid(), getgid(), geteuid(), getegid());
  snprintf(fname, sizeof(fname), "/private/tmp/debug%d.txt", getpid());
  write_file(fname, buf, strlen(buf));
}
