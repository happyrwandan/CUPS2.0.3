#ifndef __MEMBUF_H
#define __MEMBUF_H

#define DEF_SIZE 1024

typedef struct mem_buf {
  char *buf;
  size_t len;
  size_t used;
} mem_buf;


mem_buf *mem_buf_new();
int mem_buf_append(mem_buf *b,const char *cp,size_t len);
int mem_buf_clear(mem_buf *b);
void mem_buf_free(mem_buf *b);
int mem_buf_append_buf(mem_buf *dst,mem_buf *src);
int mem_buf_append_char(mem_buf *b,char c);
int mem_buf_append_short_lendian(mem_buf *b,unsigned short s);
int mem_buf_append_short_bendian(mem_buf *b,unsigned short s);
int mem_buf_append_int_lendian(mem_buf *b,unsigned int i);
int mem_buf_append_int_bendian(mem_buf *b,unsigned int i);
int mem_buf_append_repeated_char(mem_buf *b,char c,size_t count);
int mem_buf_append_string(mem_buf *b, const char *s,int null_term);
int mem_buf_read_file(mem_buf *b, const char* fname);

#endif
