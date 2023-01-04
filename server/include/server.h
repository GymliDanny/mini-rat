#ifndef MRAT_SERVER_H
#define MRAT_SERVER_H

#include <stdint.h>

void* control_listener(void *port);
void* control_worker(void *sock_desc);
void* listener(void *port);
void* worker(void *sock_desc);

#endif
