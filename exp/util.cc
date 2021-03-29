#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "util.h"

void print_hex(const void *input,size_t len) {
  const char *ptr = (const char *)input;
  char tmp[256];
  char lastline[1024];
  char line[1024];
  char txt_buf[17];
  int i;
  int first_dup = 1;

  memset(txt_buf,0,sizeof(txt_buf));
  memset(line,0,sizeof(line));
  memset(lastline,0,sizeof(lastline));

  for(i = 0;i < len;i++) {

    if(isprint(ptr[i])) {
      txt_buf[(i % 16)] = ptr[i];
    } else {
      txt_buf[(i % 16)] = '.';
    }

    snprintf(tmp,sizeof(tmp),"%2.2x",ptr[i] & 0xff);
    strcat(line,tmp);

    if((i % 16) == 15) {
      snprintf(tmp,sizeof(tmp),"\t%s\n",txt_buf);
      strcat(line,tmp);
      memset(txt_buf, 0, sizeof(txt_buf));
      if(strcmp(line,lastline) || (i + 1) >= len) {
        fprintf(stdout,"0x%08x: %s",i & 0xfffffff0,line);
        if((i + 1) >= len) {
          fprintf(stdout,"0x%08x:\n\n",(i + 1) & 0xfffffff0);
        }
        memcpy(lastline,line,sizeof(line));
        first_dup = 1;
      } else if(first_dup != 0) {
        fprintf(stdout,"*\n");
        first_dup = 0;
      }
      memset(line,0,sizeof(line));
    } else if (i == (len - 1)) {
      snprintf(tmp,sizeof(tmp)," %*c\t%s\n",(i % 16) > 7 ? (15 - (i % 16)) * 3 : (15 - (i % 16)) * 3 + 2,' ',txt_buf);
      strcat(line,tmp);
      fprintf(stdout,"0x%08x: %s",i & 0xfffffff0,line);
      fprintf(stdout,"0x%08x:\n\n",i + 1);
      memset(line,0,sizeof(line));
    } else if ((i % 16) == 7) {
      strcat(line,"  ");
    } else {
      strcat(line," ");
    }
  }

  return;
}

void print_html_escaped_array(const void* input, size_t len) {
  const unsigned char* cp = (const unsigned char*)input;
  size_t i;
  for (i = 0; i < len; i++) {
    fprintf(stdout, "%%%02x", cp[i]);
  }
}

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

int read_file(const char *fname,unsigned char **out,size_t *size_out) {
  int fd;
  size_t fsize;
  struct stat statbuf;
  unsigned char *data;

  if(stat(fname,&statbuf)) {
    fprintf(stderr,"Could not stat filename '%s'\n",fname);
    exit(-1);
  }

  fsize = statbuf.st_size;

  data = (unsigned char *)malloc(fsize);

  if(!data) {
    fprintf(stderr,"Could not allocate %zd bytes for file input\n",fsize);
    exit(-1);
  }

  fd = open(fname,O_RDONLY);

  if(fd < 0) {
    fprintf(stderr,"Error opening file '%s'\n",fname);
    free(data);
    return -1;
  }

  if(read(fd,data,fsize) != fsize) {
    fprintf(stderr,"Error reading %zd bytes from file '%s'\n",fsize,fname);
    free(data);
    close(fd);
    return -1;
  }

  close(fd);
  *size_out = fsize;
  *out = data;
  return 0;
}

void reverse_memcpy(unsigned char *dst, const unsigned char *src, size_t len) {
  for(size_t i = 0;i < len;i++) {
    dst[i] = src[len - i - 1];
  }
}

size_t count_zeros(const unsigned char *buf,size_t len) {
  size_t count = 0;

  for(size_t i = 0;i < len;i++) {
    if(buf[i] == 0)
      count++;
  }

  return count;
}

bool count_zeros_max(const unsigned char *buf,size_t len,size_t max,size_t *zero_last) {
  size_t count = 0;

  if(len < max) {
    return true;
  }

  for(int i = len - 1;i >= 0;i--) {
    if(buf[i] == 0) {
      if(count == 0) {
        if(zero_last)
          *zero_last = i;
      }
      count++;
    }

    if(count >= max) {
      return false;
    }
  }

  return true;
}

bool count_dup_max(const unsigned char *buf,size_t len,size_t max,size_t *dup_last,unsigned char *dup_char) {
  size_t count[256];
  size_t last_instance[256];
  unsigned char x;

  if(len < max)
    return true;

  memset(&count[0],0,sizeof(count));
  memset(&last_instance[0],0,sizeof(last_instance));

  for(int i = len - 1;i >= 0;i--) {
    x = buf[i];

    if(count[x] == 0) {
      last_instance[x] = i;
    }
    count[x]++;

    if(count[x] >= max) {
      if(dup_last)
        *dup_last = last_instance[x];
      if(dup_char)
        *dup_char = x;
      return false;
    }
  }

  return true;
}

unsigned char get_uint8(char* input) {
  unsigned char ret = (input[0] & 0xff);
  return ret;
}

unsigned short get_uint16_be(char* input) {
  unsigned short ret = 0;
  for (int i = 0; i < 2; i++) {
    ret <<= 8;
    ret |= (input[i] & 0xff);
  }
  return ret;
}

unsigned short get_uint16_le(char* input) {
  unsigned short ret = 0;
  for (int i = 2; i > 0; i--) {
    ret <<= 8;
    ret |= (input[i - 1] & 0xff);
  }
  return ret;
}

unsigned int get_uint32_be(char* input) {
  unsigned int ret = 0;
  for (int i = 0; i < 4; i++) {
    ret <<= 8;
    ret |= (input[i] & 0xff);
  }
  return ret;
}

unsigned int get_uint32_le(char* input) {
  unsigned short ret = 0;
  for (int i = 4; i > 0; i--) {
    ret <<= 8;
    ret |= (input[i - 1] & 0xff);
  }
  return ret;
}
