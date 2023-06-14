#ifndef MRAT_SESSION_H
#define MRAT_SESSION_H

#include <mini-rat.h>
#include <list.h>

struct session {
        int id;
        int socket;
        int alive;
        struct list_head list;
};

int init_session(int socket);
struct session* find_session(int id);
ssize_t write_session(int id, char *buffer, size_t sz);
ssize_t read_session(int id, char *buffer, size_t sz);
void kill_session(int id);
int num_alive_sessions(void);

#endif
