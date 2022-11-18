#ifndef MRAT_LOGGING_H
#define MRAT_LOGGING_H

#include <stdio.h>
#include <stdarg.h>

enum LOG_LEVEL {
        LOG_SEVERE,
        LOG_WARNING,
        LOG_INFO,
        LOG_DEBUG,
};

int init_logging(FILE *out_file);
int vlog_msg(int level, const char *fmt, va_list args);
int log_msg(int level, const char *fmt, ...);

#endif
