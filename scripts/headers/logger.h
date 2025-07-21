#ifndef LOGGER_H
#define LOGGER_H

// Log level enumeration
typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } log_level_t;

// Initialize logger with file and minimum log level
void log_init(const char *filename, log_level_t min_level);
// Write a log message with specified level and format
void log_write(log_level_t level, const char *fmt, ...);
// Close the logger and file
void log_close(void);

// Convenience macros for each log level
#define LOG_DEBUG(...) log_write(LOG_DEBUG, __VA_ARGS__)
#define LOG_INFO(...)  log_write(LOG_INFO, __VA_ARGS__)
#define LOG_WARN(...)  log_write(LOG_WARN, __VA_ARGS__)
#define LOG_ERROR(...) log_write(LOG_ERROR, __VA_ARGS__)

#endif // LOGGER_H