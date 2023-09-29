#ifndef MRAT_SERVER_H
#define MRAT_SERVER_H

#include <mini-rat.h>

void* control_listener(void *port);
void* control_worker(void *sock_desc);

int ping_pong(int sock);

void* listener(void *port);
void* worker(void *sock_desc);

#endif
