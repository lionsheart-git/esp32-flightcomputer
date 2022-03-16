/**
 * @file Logger.hpp
 */
#ifndef FLIGHTCOMPUTER_LOGGER_HPP
#define FLIGHTCOMPUTER_LOGGER_HPP

/**
 * @namespace Logging
 * @brief Collection of all classes related to Logging.
 *
 * Currently only Logger lives here because other loggers are macros only.
 */
namespace Logging
{

    /**
     * @class Logger
     * @brief General logging functions.
     *
     * Used by all other loggers to write there specific formatted output to serial and sd card.
     */
    class Logger
    {
    public:
        static int Log(const char *path, const char *format, ...);
        static char const *PathToFileName(const char *path);
    };

}

#endif //FLIGHTCOMPUTER_LOGGER_HPP
