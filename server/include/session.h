#ifndef MRAT_SESSION_H
#define MRAT_SESSION_H

#include "list.h"

struct session {
        int id;
        int socket;
        int alive;
        struct list_head list;
};

int init_session(int socket);
struct session* find_session(int id);
int write_session(int id, const char *data, size_t size);
size_t read_session(int id, char *data, size_t size);
int num_alive_sessions(void);

#endif
