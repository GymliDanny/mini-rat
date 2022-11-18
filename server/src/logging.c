#include <logging.h>
#include <stdio.h>
#include <pthread.h>

static const char *severe = "SEVERE";
static const char *warn = "WARN";
static const char *info = "INFO";
static const char *debug = "DEBUG";

static FILE *out_stream;

int init_logging(FILE *out_file) {
        // TODO: maybe make this a bit more thread safe
        out_stream = out_file;
        return 0;
}

int vlog_msg(int level, const char *fmt, va_list args) {
        char output[1024];
        int ret = 0;
        switch (level) {
                case LOG_SEVERE:
                        snprintf(output, 1024, "%s: %s", severe, fmt);
                        ret = vfprintf(out_stream, output, args);
                        break;
                case LOG_WARNING:
                        snprintf(output, 1024, "%s: %s", warn, fmt);
                        ret = vfprintf(out_stream, output, args);
                        break;
                case LOG_INFO:
                        snprintf(output, 1024, "%s: %s", info, fmt);
                        ret = vfprintf(out_stream, output, args);
                        break;
                case LOG_DEBUG:
                        snprintf(output, 1024, "%s: %s", debug, fmt);
                        ret = vfprintf(out_stream, output, args);
                        break;
        }
        return ret;
}

int log_msg(int level, const char *fmt, ...) {
        va_list args;
        int done;

        va_start(args, fmt);
        done = vlog_msg(level, fmt, args);
        va_end(args);

        return done;
}
