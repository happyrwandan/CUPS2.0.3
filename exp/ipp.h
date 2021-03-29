#ifndef __IPP_H
#define __IPP_H

#include "membuf.h"
#include "http.h"
#include "exp.h"

enum {
  GENERIC_REQUEST,
  GET_PRINTERNAME,
  LEAK_ADMINCONF,
  LEAK_ADMIN,
  UPDATE_CONFIG,
  ADD_PRINTER,
  SEED_LIBRARY
};

void ipp_append_attribute_string(mem_buf *buf, int tag_type, char* tag_name, char *tag_value);
void ipp_append_attribute_integer(mem_buf *buf, int tag_type, char* tag_name, int tag_value);
void ipp_append_attribute_boolean(mem_buf* buf, int tag_type, char* tag_name, int value);
void ipp_start_attributes_group(mem_buf *buf, char group_tag);
int build_ipp_request_leakref(expctx* ctx, uritest* u, mem_buf *buf, char* leak_string, size_t leak_count);
int build_ipp_request_get_printer_name(expctx* ctx, uritest* u, mem_buf *buf);
int build_ipp_request_clear_admin_conf(expctx* ctx, uritest* u, mem_buf *buf);
int build_ipp_request_clear_admin(expctx* ctx, uritest* u, mem_buf *buf);
int build_ipp_request_addprinter(expctx* ctx, uritest* u, mem_buf *buf);
int parse_ipp_response_getprinter(expctx* ctx, uritest* u, char* body, size_t len);
int parse_ipp_response(expctx* ctx, uritest* u, char* body, size_t len, int type);

#endif // __IPP_H
