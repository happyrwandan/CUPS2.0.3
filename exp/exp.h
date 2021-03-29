#ifndef __EXP_H
#define __EXP_H

#include "membuf.h"

typedef struct expctx {
  int host_resolved;
  char *host;
  unsigned int ip;

  int fd;

  char *printer_name;

  int port;

  char *admin_conf_location_string;
  size_t admin_conf_leak_count;

  char *admin_location_string;
  size_t admin_leak_count;

  mem_buf* cups_config_content;
  mem_buf* loader_payload;

  int target_os;
  int target_arch;

  size_t num_temp_files;
} expctx;

#endif
