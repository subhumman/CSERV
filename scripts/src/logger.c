#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// Static variables for log file and current log level
static FILE *log_file = NULL;
static log_level_t current_level = LOG_INFO;

// String representations of log levels
static const char *level_str[] = { "DEBUG", "INFO", "WARN", "ERROR" };

// Initialize logger: open file and set minimum log level
void log_init(const char *filename, log_level_t min_level) {
    if (log_file && log_file != stdout) {
        fclose(log_file);
    }
    log_file = fopen(filename, "a");
    if (!log_file) log_file = stdout;
    current_level = min_level;
}

// Write a log message if level is sufficient
void log_write(log_level_t level, const char *fmt, ...) {
    if (level < current_level) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timebuf[20];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", t);

    fprintf(log_file, "[%s] %s: ", timebuf, level_str[level]);
    va_list args;
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    va_end(args);
    fprintf(log_file, "\n");
    fflush(log_file);
}

// Close the log file if needed
void log_close(void) {
    if (log_file && log_file != stdout) fclose(log_file);
    log_file = NULL;
}