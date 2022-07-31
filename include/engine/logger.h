#pragma once

#define LOG_WARN_ENABLED  1
#define LOG_INFO_ENABLED  1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

typedef enum LogLevel {
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO  = 2,
    LOG_LEVEL_WARN  = 3,
    LOG_LEVEL_ERROR = 4,
    LOG_LEVEL_FATAL = 5
} log_level;

void log_msg(log_level level, const char *message, ...);

#define LOGF(message, ...) log_msg(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
#define LOGE(message, ...) log_msg(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);

#if LOG_WARN_ENABLED
#define LOGW(message, ...) log_msg(LOG_LEVEL_WARN , message, ##__VA_ARGS__);
#else
#define LOGW(message, ...)
#endif

#if LOG_INFO_ENABLED
#define LOGI(message, ...) log_msg(LOG_LEVEL_INFO , message, ##__VA_ARGS__);
#else
#define LOGI(message, ...)
#endif

#if LOG_DEBUG_ENABLED
#define LOGD(message, ...) log_msg(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define LOGD(message, ...)
#endif

#if LOG_TRACE_ENABLED
#define LOGT(message, ...) log_msg(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define LOGT(message, ...)
#endif

