#ifndef __NETWORK_H
#define __NETWORK_H

#include "membuf.h"

int network_write(int fd,char *cp,size_t len);
mem_buf *network_read_to_buf(int fd);
unsigned int resolve(char *host);

#endif
