
#include "UtilityFunctions.hpp"

#include "PinConfiguration.hpp"

void UtilityFunctions::ScanI2CDevice(TwoWire *wireInstance)
{
    byte error, address;
    int nDevices;
    dlogn("Scanning...");
    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        wireInstance->beginTransmission(address);
        error = wireInstance->endTransmission();
        if (error == 0)
        {
            dlog("I2C device found at address 0x");
            if (address < 16)
            {
                dlogs("0");
            }
            dlogsn("%x", address);
            nDevices++;
        } else if (error == 4)
        {
            dlog("Unknow error at address 0x");
            if (address < 16)
            {
                dlogs("0");
            }
            dlogsn("%x", address);
        }
    }
    if (nDevices == 0)
    {
        dlogn("No I2C devices found");
    } else
    {
        dlogn("done");
    }
}

void UtilityFunctions::EnableSDSPI()
{
    digitalWrite(LoRa_CS, HIGH);
    digitalWrite(SD_CS, LOW);
}

void UtilityFunctions::EnableLoRaSPI()
{
    digitalWrite(SD_CS, HIGH);
    digitalWrite(LoRa_CS, LOW);
}

void UtilityFunctions::DisableLoRaSPI()
{
    digitalWrite(LoRa_CS, HIGH);
}

void UtilityFunctions::DisableSDSPI()
{
    digitalWrite(SD_CS, HIGH);
}
