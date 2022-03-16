#include <Arduino.h>

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <sys/time.h>

#include "heltec.h"

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

#include "PinConfiguration.hpp"
#include "SDCard.hpp"

#include "Sensors/GNSSSensor.hpp"
#include "Sensors/SensorData.hpp"

static const int RXPin = 36, TXPin = 37;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


void setup()
{
    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
    Heltec.display->flipScreenVertically();
    Heltec.display->setFont(ArialMT_Plain_10);

    // Init SD Card on correct SPI port.
    SDCard::Begin();
//
    GNSSSensor gnss;
    Serial.println(SensorData::Satellites());

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

static void
showDateTime(TinyGPSDate &d, TinyGPSTime &t, OLEDDISPLAY_TEXT_ALIGNMENT alignment, const uint8_t *font, int posX,
             int posY)
{
    char sd[20];
    if (d.isValid())
    {
        snprintf(sd, sizeof(sd), "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    } else
    {
        snprintf(sd, sizeof(sd), "INV date");
    }

    char st[9];
    if (t.isValid())
    {
        snprintf(st, sizeof(st), "%02d:%02d:%02d", t.hour(), t.minute(), t.second());
    } else
    {
        snprintf(st, sizeof(st), "INV time");
    }

    strncat(sd, st, sizeof(sd) - strlen(sd) - 1);

    Heltec.display->setTextAlignment(alignment);
    Heltec.display->setFont(font);
    Heltec.display->drawString(posX, posY, sd);

    smartDelay(0);
}

static void showFloat(float val, bool valid, char const *prefix, int prec, OLEDDISPLAY_TEXT_ALIGNMENT alignment,
                      const uint8_t *font, int posX, int posY)
{
    char s[32];
    if (valid)
    {
        snprintf(s, sizeof(s), "%s %.*f", prefix, prec, val);
    } else
    {
        snprintf(s, sizeof(s), "%s INV", prefix);
    }

    Heltec.display->setTextAlignment(alignment);
    Heltec.display->setFont(font);
    Heltec.display->drawString(posX, posY, s);

    smartDelay(0);
}

static void
showInt(int val, bool valid, char const *prefix, OLEDDISPLAY_TEXT_ALIGNMENT alignment, const uint8_t *font, int posX,
        int posY)
{
    char s[32];
    if (valid)
    {
        snprintf(s, sizeof(s), "%s %i", prefix, val);
    } else
    {
        snprintf(s, sizeof(s), "%s INV", prefix);
    }
    Heltec.display->setTextAlignment(alignment);
    Heltec.display->setFont(font);
    Heltec.display->drawString(posX, posY, s);
    smartDelay(0);
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
    Heltec.display->clear();

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        slog_e("No GPS data received: check wiring");
        Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
        Heltec.display->drawStringMaxWidth(64, 16, 128, "No GPS data received: check wiring");
    } else
    {
        dlog("%c", ' ');
        //Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
        //Heltec.display->setFont(ArialMT_Plain_10);
        //Heltec.display->drawString(0, 0, "Sat: 5");
        showInt(gps.satellites.value(), gps.satellites.isValid(), "Sat:", TEXT_ALIGN_LEFT, ArialMT_Plain_10, 0, 0);

        // Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
        // Heltec.display->drawString(52, 0, "Fix: 589");
        // showInt(gps_.sentencesWithFix(), true, "Fix:", TEXT_ALIGN_CENTER, ArialMT_Plain_10, 52, 0);

        // Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
        // Heltec.display->drawString(128, 0, "HDOP: 1.7");
        showFloat(gps.hdop.hdop(), gps.hdop.isValid(), "HDOP:", 1, TEXT_ALIGN_RIGHT, ArialMT_Plain_10, 128, 0);

        // Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
        // Heltec.display->drawString(64, 10, "00/00/2000 00:00:00");
        // Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);

        showDateTime(gps.date, gps.time, TEXT_ALIGN_CENTER, ArialMT_Plain_10, 64, 10);

        // Heltec.display->drawString(0, 20, "Long: 52.282654");
        showFloat(gps.location.lng(), gps.location.isValid(), "Long:", 6, TEXT_ALIGN_LEFT, ArialMT_Plain_10, 0, 20);

        // Heltec.display->drawString(0, 30, "Lat: 10.526393");
        showFloat(gps.location.lat(), gps.location.isValid(), "Lat:", 6, TEXT_ALIGN_LEFT, ArialMT_Plain_10, 0, 30);

        // Heltec.display->drawString(0, 40, "Alt: 960");
        showFloat(gps.altitude.meters(), gps.altitude.isValid(), "Alt:", 2, TEXT_ALIGN_LEFT, ArialMT_Plain_10, 0, 40);

        showFloat(gps.speed.mps(), gps.speed.isValid(), "m/s:", 2, TEXT_ALIGN_CENTER, ArialMT_Plain_10, 70, 40);

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

    // write the buffer to the display
    Heltec.display->display();

    smartDelay(100);
}
