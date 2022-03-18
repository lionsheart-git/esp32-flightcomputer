#include <Arduino.h>

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <sys/time.h>

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

#include "PinConfiguration.hpp"
#include "SDCard.hpp"

#include "Sensors/SensorData.hpp"

static const int RXPin = 35, TXPin = 32;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


void setup()
{
    Serial.begin(115200);

    // Init SD Card on correct SPI port.
    SDCard::Begin();

    ss.begin(GPSBaud);

    slog_i("%s %s %s", "Testing TinyGPS++ library v.", TinyGPSPlus::libraryVersion(), "by Mikal Hart");
    Serial.println();
    dlogn("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
    dlogn(
            "           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
    dlogn(
            "----------------------------------------------------------------------------------------------------------------------------------------");
}

// This custom version of delay() ensures that the gps_ object
// is being "fed".
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (ss.available())
            gps.encode(ss.read());
    } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
    if (!valid)
    {
        while (len-- > 1)
            dlogs("%c", '*');
        dlogs("%c", ' ');
    } else
    {
        dlogs("%f", val);
        int vi = abs((int) val);
        int flen = prec + (val < 0.0 ? 2 : 1); // . and -
        flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
        for (int i = flen; i < len; ++i)
            dlogs("%c", ' ');
    }
    smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
    char sz[32] = "*****************";
    if (valid)
        sprintf(sz, "%ld", val);
    sz[len] = 0;
    for (int i = strlen(sz); i < len; ++i)
        sz[i] = ' ';
    if (len > 0)
        sz[len - 1] = ' ';
    dlogs("%s", sz);
    smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
    if (!d.isValid())
    {
        dlogs("%s", "********** ");
    } else
    {
        char sz[32];
        sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
        dlogs("%s", sz);
    }

    if (!t.isValid())
    {
        dlogs("%s", "******** ");
    } else
    {
        char sz[32];
        sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
        dlogs("%s", sz);
    }

    printInt(d.age(), d.isValid(), 5);
    smartDelay(0);
}

static void printStr(const char *str, int len)
{
    size_t slen = strlen(str);
    for (int i = 0; i < len; ++i)
        dlogs("%c", i < slen ? str[i] : ' ');
    smartDelay(0);
}


void loop()
{
    // clear the display

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        slog_e("No GPS data received: check wiring");
    } else
    {
        static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

        printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
        printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
        printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
        printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
        printInt(gps.location.age(), gps.location.isValid(), 5);
        printDateTime(gps.date, gps.time);
        printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
        printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
        printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
        printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);

        unsigned long distanceKmToLondon =
                (unsigned long) TinyGPSPlus::distanceBetween(
                        gps.location.lat(),
                        gps.location.lng(),
                        LONDON_LAT,
                        LONDON_LON) / 1000;
        printInt(distanceKmToLondon, gps.location.isValid(), 9);

        double courseToLondon =
                TinyGPSPlus::courseTo(
                        gps.location.lat(),
                        gps.location.lng(),
                        LONDON_LAT,
                        LONDON_LON);

        printFloat(courseToLondon, gps.location.isValid(), 7, 2);

        const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);

        printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);

        printInt(gps.charsProcessed(), true, 6);
        printInt(gps.sentencesWithFix(), true, 10);
        printInt(gps.failedChecksum(), true, 9);
    }
    dlogsn("");

    smartDelay(100);
}
