#ifndef __UTIL_H
#define __UTIL_H

void print_hex(const void *input,size_t len);
void print_html_escaped_array(const void* input, size_t len);
int write_file(const char *fname,unsigned char *out,size_t size_out);
int read_file(const char *fname,unsigned char **out,size_t *size_out);
void reverse_memcpy(unsigned char *dst, const unsigned char *src, size_t len);
size_t count_zeros(const unsigned char *buf,size_t len);
bool count_zeros_max(const unsigned char *buf,size_t len,size_t max,size_t *zero_last);
bool count_dup_max(const unsigned char *buf,size_t len,size_t max,size_t *dup_last,unsigned char *dup_char);
unsigned char get_uint8(char* input);
unsigned short get_uint16_be(char* input);
unsigned short get_uint16_le(char* input);
unsigned int get_uint32_be(char* input);
unsigned int get_uint32_le(char* input);

#endif
