/**
 * @file DataLogger.hpp
 * @brief Compiler macros for logging data.
 */

#ifndef FLIGHTCOMPUTER_DATALOGGER_HPP
#define FLIGHTCOMPUTER_DATALOGGER_HPP

#include "Logger.hpp"

/**
 * @def D_LOG_PATH
 * @brief Path to the datalog file on the sd card.
 */
#ifndef D_LOG_PATH
#define D_LOG_PATH "/datalog.log"
#endif

/**
 * @def SHORT_D_LOG_FORMAT
 * @brief Logging format without time and newline.
 */
#ifndef SHORT_D_LOG_FORMAT//(letter, format)
#define SHORT_D_LOG_FORMAT(format) format
#endif

/**
 * @def SHORT_D_LOGN_FORMAT
 * @brief Logging format without time.
 */
#ifndef SHORT_D_LOGN_FORMAT//(letter, format)
#define SHORT_D_LOGN_FORMAT(format) format "\r\n"
#endif

//@todo Implemented using of rtc time
/**
 * @def D_LOG_FORMAT
 * @brief Logging format with time but without newline.
 *
 * Currently system time is used since the project has no Real Time Clock
 */
#ifndef D_LOG_FORMAT//(letter, format)
#define D_LOG_FORMAT(format) "[%ld] " format, millis()
#endif

/**
 * @def D_LOGN_FORMAT
 * @brief Logging format with time and newline.
 *
 * Currently system time is used since the project has no Real Time Clock
 */
#ifndef D_LOGN_FORMAT//(letter, format)
#define D_LOGN_FORMAT(format) "[%ld] " #format "\r\n", millis()
#endif

/**
 * @def dlog(format, ...)
 * @brief Data logging with time but without newline.
 *
 * @param format Format string.
 * @param ... Arguments for format string.
 *
 * @ref Logging::Logger::Log
 */
#ifndef dlog
#define dlog(format, ...) Logging::Logger::Log(D_LOG_PATH, D_LOG_FORMAT(format), ##__VA_ARGS__)
#endif

/**
 * @def dlogn(format, ...)
 * @brief Data logging with time and newline.
 *
 * @param format Format string.
 * @param ... Arguments for format string.
 *
 * @ref Logging::Logger::Log
 */
#ifndef dlogn
#define dlogn(format, ...) Logging::Logger::Log(D_LOG_PATH, D_LOGN_FORMAT(format), ##__VA_ARGS__)
#endif

/**
 * @def dlogs(format, ...)
 * @brief Data logging without time and newline.
 *
 * @param format Format string.
 * @param ... Arguments for format string.
 *
 * @ref Logging::Logger::Log
 */
#ifndef dlogs
#define dlogs(format, ...) Logging::Logger::Log(D_LOG_PATH, SHORT_D_LOG_FORMAT(format), ##__VA_ARGS__)
#endif

/**
 * @def dlogsn(format, ...)
 * @brief Data logging without time but with newline.
 *
 * @param format Format string.
 * @param ... Arguments for format string.
 *
 * @ref Logging::Logger::Log
 */
#ifndef dlogsn
#define dlogsn(format, ...) Logging::Logger::Log(D_LOG_PATH, SHORT_D_LOGN_FORMAT(format), ##__VA_ARGS__)
#endif

#endif //FLIGHTCOMPUTER_DATALOGGER_HPP
