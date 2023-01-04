#include <logging.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

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
        time_t rawtime;
        time(&rawtime);
        char timestr[1024];
        ctime_r(&rawtime, timestr);
        timestr[strlen(timestr)-1] = '\0';

        char output[1024];
        int ret = 0;
        switch (level) {
                case LOG_SEVERE:
                        snprintf(output, 1024, "%s %s: %s", timestr, severe, fmt);
                        ret = vfprintf(out_stream, output, args);
                        break;
                case LOG_WARNING:
                        snprintf(output, 1024, "%s %s: %s", timestr, warn, fmt);
                        ret = vfprintf(out_stream, output, args);
                        break;
                case LOG_INFO:
                        snprintf(output, 1024, "%s %s: %s", timestr, info, fmt);
                        ret = vfprintf(out_stream, output, args);
                        break;
                case LOG_DEBUG:
                        snprintf(output, 1024, "%s %s: %s", timestr, debug, fmt);
                        ret = vfprintf(out_stream, output, args);
                        break;
        }
        fflush(out_stream);
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

void close_logfile(void) {
        fclose(out_stream);
}
