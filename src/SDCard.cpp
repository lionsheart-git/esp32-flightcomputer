/**
 * @file SDCard.cpp
 * @brief Contains @ref SDCard class
 */
#include "SDCard.hpp"

#include <FS.h>
#include <SD.h>

#include "PinConfiguration.hpp"
#include "Logging/SystemLogger.hpp"

bool SDCard::initSuccessful = false;

//@todo Error handling and recovery if init not successful.

/**
 * @brief Initializes the SD arduino library.
 *
 * Uses the SD_SCK, SD_MISO, SD_MOSI, SD_CS pins to set the correct SPI and the SD_CS for the SD library.
 */
void SDCard::Begin()
{
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

    if (!SD.begin(SD_CS))
    {
        initSuccessful = false;
        slog_e("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        initSuccessful = false;
        slog_e("No SD card attached");
        return;
    }

    initSuccessful = true;
}

/**
 * @brief Lists files and directories of an directory.
 *
 * @param fs File system where the directory is located.
 * @param dirname Path to directory on file system.
 * @param levels Number of levels to recurse down if available.
 */
void SDCard::ListDir(FS &fs, const char *dirname, uint8_t levels)
{
    slog_d("Listing directory: %s", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        initSuccessful = false;
        slog_e("Failed to open directory %s", dirname);
        return;
    }
    if (!root.isDirectory())
    {
        initSuccessful = false;
        slog_e("Not a directory %s", dirname);
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            slog_i("  DIR : ");
            slog_i("%s", file.name());
            if (levels)
            {
                ListDir(fs, file.name(), levels - 1);
            }
        } else
        {
            slog_i("  FILE: %s   SIZE: %u", file.name(), file.size());
        }
        file = root.openNextFile();
    }
}

/**
 * @brief Creates an directory at given location.
 *
 * @param fs File system to create the directory on.
 * @param path Path to directory.
 */
void SDCard::CreateDir(fs::FS &fs, const char *path)
{
    slog_d("Creating Dir: %s", path);
    if (fs.mkdir(path))
    {
        slog_i("Dir %s created", path);
    } else
    {
        initSuccessful = false;
        slog_e("mkdir %s failed", path);
    }
}

/**
 * @brief Removes a directory at given location.
 *
 * @param fs File system to remove the directory from.
 * @param path Path on the file system to directory.
 */
void SDCard::RemoveDir(fs::FS &fs, const char *path)
{
    slog_d("Removing Dir: %s.", path);
    if (fs.rmdir(path))
    {
        slog_i("Dir %s removed", path);
    } else
    {
        initSuccessful = false;
        slog_e("rmdir %s failed", path);
    }
}

/**
 * @brief Reads bytes from a file.
 *
 * @param fs File system to read file from.
 * @param path Path on the file system to file.
 */
void SDCard::ReadFile(fs::FS &fs, const char *path)
{
    slog_d("Reading file: %s.", path);

    File file = fs.open(path);
    if (!file)
    {
        initSuccessful = false;
        slog_e("Failed to open file %s for reading.", path);
        return;
    }

    while (file.available())
    {
        //@todo Why read from file? How to redirect output?
        Serial.write(file.read());
    }
    file.close();
}

/**
 * @brief Writes a message to given file.
 *
 * If not existent file is created. If file already exists content is overwritten.
 *
 * @param fs File system to write message to.
 * @param path Path to file to write message in.
 * @param message Message to write to file.
 */
void SDCard::WriteFile(fs::FS &fs, const char *path, const char *message)
{
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        initSuccessful = false;
        slog_e("Failed to open file %s message %s for writing.", path, message);
        return;
    }
    if (file.print(message))
    {
        initSuccessful = false;
        slog_e("Write file %s message %s failed.", path, message);

    }
    file.close();
}

/**
 * @brief Appends a message to a file.
 *
 * If not existent, file is create. Otherwise message is appended to existing content.
 *
 * @param fs File system to append file on.
 * @param path Path to file to append message to.
 * @param message Message to append.
 */
void SDCard::AppendFile(fs::FS &fs, const char *path, const char *message)
{
    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        initSuccessful = false;
        slog_e("Failed to open file %s for appending %s.", path, message);
        return;
    }
    if (file.print(message))
    {
        initSuccessful = false;
        slog_e("Append %s to %s failed.", message, path);
    }
    file.close();
}

/**
 * @brief Renames a file.
 *
 * @param fs File system to rename file on.
 * @param path1 Old path to file.
 * @param path2 New path to file.
 */
void SDCard::RenameFile(fs::FS &fs, const char *path1, const char *path2)
{
    if (fs.rename(path1, path2))
    {
        slog_i("File %s renamed to %s.", path1, path2);
    } else
    {
        initSuccessful = false;
        slog_e("Rename %s to  %s failed.", path1, path2);
    }
}

/**
 * @brief Deletes a file from file system.
 *
 * @param fs File system to delete file from.
 * @param path Path to file to delete.
 */
void SDCard::DeleteFile(fs::FS &fs, const char *path)
{
    if (fs.remove(path))
    {
        slog_i("File %s deleted.", path);
    } else
    {
        initSuccessful = false;
        slog_e("Delete %s failed.", path);
    }
}

/**
 * @brief Tests the IO performance for a file.
 *
 * @param fs File system to test IO on.
 * @param path Path to test file.
 */
void SDCard::TestFileIO(fs::FS &fs, const char *path)
{
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file)
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        slog_i("%u bytes read for %u ms.", flen, end);
        file.close();
    } else
    {
        initSuccessful = false;
        slog_e("Failed to open file %s for reading.", path);
    }


    file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        initSuccessful = false;
        slog_e("Failed to open file %s for writing.", path);
        return;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++)
    {
        file.write(buf, 512);
    }
    end = millis() - start;
    slog_i("%u bytes written for %u ms.", 2048 * 512, end);
    file.close();
}
