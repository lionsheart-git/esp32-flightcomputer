/**
 * @file SystemLogger.hpp
 * @brief Logger for logging system messages.
 */
#ifndef FLIGHTCOMPUTER_SYSTEMLOGGER_HPP
#define FLIGHTCOMPUTER_SYSTEMLOGGER_HPP

//@todo Figure out how to redirect log messages from eps32-hal-log.h.

#include "Logger.hpp"
/**
 * @def S_LOG_LEVEL_NONE
 * @brief Catchall for everything that does not fit in an other log level.
 */
#define S_LOG_LEVEL_NONE       (0)

/**
 * @def S_LOG_LEVEL_ERROR
 * @brief Log level for errors.
 */
#define S_LOG_LEVEL_ERROR      (1)

/**
 * @def S_LOG_LEVEL_WARN
 * @brief Log level for warnings.
 */
#define S_LOG_LEVEL_WARN       (2)

/**
 * @def S_LOG_LEVEL_INFO
 * @brief Log level for info messages.
 */
#define S_LOG_LEVEL_INFO       (3)

/**
 * @def S_LOG_LEVEL_DEBUG
 * @brief Log level for debug messages.
 */
#define S_LOG_LEVEL_DEBUG      (4)

/**
 * @def S_LOG_LEVEL_VERBOSE
 * @brief Log level for message more insignificant than debug.
 */
#define S_LOG_LEVEL_VERBOSE    (5)

/**
 * @def CONFIG_S_LOG_DEFAULT_LEVEL
 * @brief Default log level.
 */
#ifndef CONFIG_S_LOG_DEFAULT_LEVEL
#define CONFIG_S_LOG_DEFAULT_LEVEL S_LOG_LEVEL_VERBOSE
#endif

/**
 * @def S_CORE_DEBUG_LEVEL
 * @brief Sets the debug level if given.
 */
#ifndef S_CORE_DEBUG_LEVEL
#define S_LOG_LEVEL CONFIG_S_LOG_DEFAULT_LEVEL
#else
#define S_LOG_LEVEL S_CORE_DEBUG_LEVEL
#endif

/**
 * @def S_LOG_PATH
 * @brief Sets the default path for system logger on sd card.
 */
#ifndef S_LOG_PATH
#define S_LOG_PATH "/systemlog.log"
#endif

/**
 * @def SHORT_S_LOG_FORMAT(letter, format)
 * @brief Short log format.
 *
 * Just appends a "\\r\\n" to format string.
 *
 * @param letter Does not do anything in this context.
 * @param format Format string for varargs.
 */
#ifndef SHORT_S_LOG_FORMAT//(letter, format)
#define SHORT_S_LOG_FORMAT(letter, format) format "\r\n"
#endif

/**
 * @def S_LOG_FORMAT
 * @brief Normal / long log format.
 *
 * Generates log output like "[letter][fileName:line] function(): format \\r\\n".
 * Example: [I][main.cpp:113] main(): Info message with number 3 \\r\\n
 *
 * @param letter Letter indicating log level E = error, W = warning, I = info, D = debug, V = verbose.
 * @param format Format string for varargs.
 */
#ifndef S_LOG_FORMAT//(letter, format)
#define S_LOG_FORMAT(letter, format) "[" #letter "][%s:%u] %s(): " format "\r\n", Logging::Logger::PathToFileName(__FILE__), __LINE__, __FUNCTION__
#endif

/**
 * @def slog_v(format, ...)
 * @brief Logs message on verbose level.
 *
 * @param format Format string for varargs to print to console..
 * @param ... Variable argument list.
 */
#if S_LOG_LEVEL >= S_LOG_LEVEL_VERBOSE
#define slog_v(format, ...) Logging::Logger::Log(S_LOG_PATH, S_LOG_FORMAT(V, format), ##__VA_ARGS__)
#else
#define slog_v(format, ...)
#endif

/**
 * @def slog_d(format, ...)
 * @brief Logs message on debug level.
 *
 * @copydetails slog_v(format, ...)
 */
#if S_LOG_LEVEL >= S_LOG_LEVEL_DEBUG
#define slog_d(format, ...) Logging::Logger::Log(S_LOG_PATH, S_LOG_FORMAT(D, format), ##__VA_ARGS__)
#else
#define slog_d(format, ...)
#endif

/**
 * @def slog_i(format, ...)
 * @brief Logs message on info level.
 *
 * @copydetails slog_v(format, ...)
 */
#if S_LOG_LEVEL >= S_LOG_LEVEL_INFO
#define slog_i(format, ...) Logging::Logger::Log(S_LOG_PATH, S_LOG_FORMAT(I, format), ##__VA_ARGS__)
#else
#define slog_i(format, ...)
#endif

/**
 * @def slog_w (format, ...)
 * @brief Logs message on warning level.
 *
 * @copydetails slog_v(format, ...)
 */
#if S_LOG_LEVEL >= S_LOG_LEVEL_WARN
#define slog_w(format, ...) Logging::Logger::Log(S_LOG_PATH, S_LOG_FORMAT(W, format), ##__VA_ARGS__)
#else
#define slog_w(format, ...)
#endif

/**
 * @def slog_e (format, ...)
 * @brief Logs message on error level.
 *
 * @copydetails slog_v(format, ...)
 */
#if S_LOG_LEVEL >= S_LOG_LEVEL_ERROR
#define slog_e(format, ...) Logging::Logger::Log(S_LOG_PATH, S_LOG_FORMAT(E, format), ##__VA_ARGS__)
#else
#define slog_e(format, ...)
#endif

/**
 * @def slog_n (format, ...)
 * @brief Logs message on none level.
 *
 * @copydetails slog_v(format, ...)
 */
#if S_LOG_LEVEL >= S_LOG_LEVEL_NONE
#define slog_n(format, ...) Logging::Logger::Log(S_LOG_PATH, S_LOG_FORMAT(N, format), ##__VA_ARGS__)
#else
#define slog_n(format, ...)
#endif

#endif //FLIGHTCOMPUTER_SYSTEMLOGGER_HPP
