/**
 * @file Logger.cpp
 */
#include "Logger.hpp"

#include <cstdarg>
#include <cstdio>

#include <esp_attr.h>
#include <malloc.h>
#include <SD.h>

#include "SDCard.hpp"

namespace Logging
{

    /**
     * @brief Extracts only file from path given by __FILE__.
     *
     * @param path Path of the file.
     * @return The file name.
     */
    const char *IRAM_ATTR Logger::PathToFileName(const char *path)
    {
        size_t i = 0;
        size_t pos = 0;
        char *p = (char *) path;
        while (*p)
        {
            i++;
            if (*p == '/' || *p == '\\')
            {
                pos = i;
            }
            p++;
        }
        return path + pos;
    }

    /**
     * @brief General log function that logs to serial and sd card.
     *
     * Adaptation of log_printf from esp32-hal-log.h.
     *
     * @param path Path to file on SD card.
     * @param format Format string.
     * @param ... Arguments for the format string.
     *
     * @return Length of resulting string written / printed.
     */
    int Logger::Log(const char *path, const char *format, ...)
    {

        static char loc_buf[64];
        char *temp = loc_buf;
        int len;
        va_list arg;
        va_list copy;
        va_start(arg, format);
        va_copy(copy, arg);
        len = vsnprintf(NULL, 0, format, arg);
        va_end(copy);
        if (len >= sizeof(loc_buf))
        {
            temp = (char *) malloc(len + 1);
            if (temp == NULL)
            {
                return 0;
            }
        }
        vsnprintf(temp, len + 1, format, arg);

        // Write to Serial (UART)
        printf("%s", temp);

        // Write to SD Card
        if (SDCard::initSuccessful)
        {
            SDCard::AppendFile(SD, path, temp);
        }

        va_end(arg);
        if (len >= sizeof(loc_buf))
        {
            free(temp);
        }
        return len;
    }

}
