#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#include "membuf.h"

mem_buf *mem_buf_new() {
  mem_buf *ret = (mem_buf *)calloc(1,sizeof(*ret));

  if(!ret)
    return NULL;

  ret->buf = (char *)calloc(1,DEF_SIZE);

  if(!ret->buf) {
    free(ret);
    return NULL;
  }

  ret->len = DEF_SIZE;
  ret->used = 0;

  return ret;
}

int mem_buf_append(mem_buf *b,const char *cp,size_t len) {

  size_t tmplen;
  char *tmpbuf;

  if(len > 0x80000000)
    return -1;

  if(b->used + len >= b->len) {
    tmplen = b->len + len + 1;
    tmpbuf = (char *)realloc(b->buf,tmplen);

    if(!tmpbuf)
      return -1;

    memset(tmpbuf + b->used,0,tmplen - b->used);

    b->buf = tmpbuf;
    b->len = tmplen;
  }

  memcpy(b->buf + b->used,cp,len);
  b->used += len;
  return 0;
}

int mem_buf_clear(mem_buf *b) {
  memset(b->buf,b->used,0);
  b->used = 0;
  return 0;
}

void mem_buf_free(mem_buf *b) {

  if(b) {
    if(b->buf)
      free(b->buf);

    free(b);
  }

  return;
}

int mem_buf_append_buf(mem_buf *dst,mem_buf *src) {
  return mem_buf_append(dst,src->buf,src->used);
}

int mem_buf_append_char(mem_buf *b,char c) {
  return mem_buf_append(b,&c,1);
}

int mem_buf_append_short_lendian(mem_buf *b,unsigned short s) {
  char x[2];

  x[0] = s & 0xff;
  x[1] = (s >> 8) & 0xff;

  return mem_buf_append(b,x,2);
}

int mem_buf_append_short_bendian(mem_buf *b,unsigned short s) {
  char x[2];

  x[0] = (s >> 8) & 0xff;
  x[1] = s & 0xff;

  return mem_buf_append(b,x,2);
}

int mem_buf_append_int_lendian(mem_buf *b,unsigned int i) {
  char x[4];

  x[0] = i & 0xff;
  x[1] = (i >> 8) & 0xff;
  x[2] = (i >> 16) & 0xff;
  x[3] = (i >> 24) & 0xff;

  return mem_buf_append(b,x,4);
}

int mem_buf_append_int_bendian(mem_buf *b,unsigned int i) {
  char x[4];

  x[0] = (i >> 24) & 0xff;
  x[1] = (i >> 16) & 0xff;
  x[2] = (i >> 8) & 0xff;
  x[3] = i & 0xff;

  return mem_buf_append(b,x,4);
}

int mem_buf_append_repeated_char(mem_buf *b,char c,size_t count) {
  int ret;
  char *tmp = (char *)malloc(count);

  if(!tmp) {
    return -1;
  }

  memset(tmp,c,count);

  ret = mem_buf_append(b,tmp,count);

  free(tmp);

  return ret;
}

int mem_buf_append_string(mem_buf *b,const char *s,int null_term) {

  size_t len = strlen(s);

  if(null_term) {
    return mem_buf_append(b,s,len + 1);
  }

  return mem_buf_append(b,s,len);
}

int mem_buf_read_file(mem_buf *b, const char* fname) {
  unsigned char* buf = NULL;
  size_t len = 0;

  if (read_file(fname, &buf, &len)) {
    return -1;
  }

  int retval = mem_buf_append(b, reinterpret_cast<const char*>(buf), len);
  free(buf);

  return retval;
}
