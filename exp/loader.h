#ifndef __LOADER_H
#define __LOADER_H

#include "membuf.h"

enum {
  OS_MACOS,
  OS_LINUX
};

enum {
  ARCH_IA32,
  ARCH_X64,
  ARCH_UNIVERSAL
};

const char kLoaderPayloadOsx[] = "payload_osx_universal.dylib";
const char kLoaderPayloadLinuxIA32[] = "payload_linux_i386.so";
const char kLoaderPayloadLinuxX64[] = "payload_linux_x64.so";

void get_cupsd_conf(mem_buf* buf, int target_os, int num_temp_files);
void get_loader_payload(mem_buf* buf, int target_os, int target_arch);

#endif // __LOADER_H
