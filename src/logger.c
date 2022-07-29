#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define MAX_LOG_LENGTH 4096

void log_msg(log_level level, const char *message, ...)
{
    char log_msg[MAX_LOG_LENGTH];
    memset(log_msg, 0, sizeof(log_msg));

    // Prefix with log level
    const char *level_strings[6] = {
        "[TRACE]: ",
        "[DEBUG]: ",
        "[INFO ]: ",
        "[WARN ]: ",
        "[ERROR]: ",
        "[FATAL]: ",
    };
    const char *prefix = level_strings[level];
    int prefix_length = strlen(prefix);
    strncpy(log_msg, prefix, prefix_length);

    // Append message
    va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(
            log_msg + prefix_length,
            MAX_LOG_LENGTH - prefix_length,
            message,
            arg_ptr
    );
    va_end(arg_ptr);

    // Print log message
    fprintf(stderr, "%s\n", log_msg);
}

