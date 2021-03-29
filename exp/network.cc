#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

#include <netdb.h>

#include "network.h"
#include "membuf.h"

int network_write(int fd,char *cp,size_t len) {
  int n;

  while(len > 0) {

    n = write(fd,cp,len);

    if(n < 0)
      return -1;

    len -= n;
    cp += n;
  }

  return 0;
}

mem_buf *network_read_to_buf(int fd) {
  struct fd_set rset;
  struct fd_set eset;
  char tmp[1024];
  mem_buf *b = mem_buf_new();
  struct timeval timeout;
  int n;

  while(1) {
    FD_ZERO(&rset);
    FD_SET(fd,&rset);

    FD_ZERO(&eset);
    FD_SET(fd,&eset);

    memset(&timeout,0,sizeof(timeout));
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    // fprintf(stdout, "Press enter to select()\n");
    // getchar();

    n = select(fd + 1,&rset,NULL,&eset,&timeout);

    if(n < 0) {
      fprintf(stderr,"Error from select\n");
      mem_buf_free(b);
      return NULL;
    }

    if(n == 0) {
      fprintf(stdout,"Select timeout\n");
      break;
    }

    if(FD_ISSET(fd,&rset)) {
      // fprintf(stdout,"Read fdset is set\n");

      n = read(fd,tmp,sizeof(tmp));

      if(n < 0) {
        mem_buf_free(b);
        return NULL;
      }

      if(n == 0) {
        fprintf(stdout,"EOF on socket, server probably hung up already\n");
        fprintf(stdout,"Got %zd bytes total as response from server\n\n",b->used);
        break;
      }

      mem_buf_append(b,tmp,n);

    } else if(FD_ISSET(fd,&eset)) {
      fprintf(stderr,"ERROR fdset is set, error on socket or other error\n");
      if(b->used > 0) {
        fprintf(stdout,"Got %zd bytes total as response from server\n\n",b->used);
        break;
      } else {
        fprintf(stderr,"No data from server, erroring out\n\n");
        mem_buf_free(b);
        return NULL;
      }
    }

  }

  mem_buf_append_char(b,0);

  return b;
}

unsigned int resolve(char *host) {
  unsigned int ret;
  struct hostent *hostent;

  ret = inet_addr(host);

  if(ret != INADDR_NONE)
    return ret;

  hostent = gethostbyname(host);

  if(hostent == NULL) {
    return INADDR_NONE;
  }

  memcpy(&ret,hostent->h_addr,sizeof(ret));
  return ret;
}
