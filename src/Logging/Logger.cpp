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
     * At the moment only works up to 255 bytes. Adaptation of log_printf from esp32-hal-log.h.
     *
     * @param path Path to file on SD card.
     * @param format Format string.
     * @param ... Arguments for the format string.
     *
     * @return Length of resulting string written / printed.
     */
    int Logger::Log(const char *path, const char *format, ...)
    {
        static char loc_buf[256];
        char *temp = loc_buf;
        int len = 255;
        va_list args;
        va_start(args, format);
//        len = vsnprintf(NULL, 0, format, arg);
//        va_end(arg);
//
//        printf("len: %d", len);
//
//        if (len >= sizeof(loc_buf))
//        {
//            printf("Creating new array.\n");
//            temp = (char *) malloc(len + 1);
//            if (temp == NULL)
//            {
//                return 0;
//            }
//        }
//
//        va_start(arg, format);
        vsnprintf(temp, len + 1, format, args);
        va_end(args);

        // Write to Serial (UART)
        printf("%s", temp);

        // Write to SD Card
        if (SDCard::initSuccessful)
        {
            SDCard::AppendFile(SD, path, temp);
        }

        if (len >= sizeof(loc_buf))
        {
            free(temp);
        }
        return len;
    }

}
