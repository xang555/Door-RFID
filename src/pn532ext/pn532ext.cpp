
#include "pn532ext.h"
#ifndef DEBUG
#include "../DEBUG.h"
#endif

pn532ext::pn532ext(){
    _pn532i2c = PN532_I2C(Wire);
    _nfc = PN532(_pn532i2c);
}

void pn532ext::begin()
{

#ifdef DEBUG
    Serial.println("Hello!");
#endif

_nfc.begin();

#ifdef DEBUG
    uint32_t versiondata = _nfc.getFirmwareVersion();
    if (!versiondata)
    {
        Serial.print("Didn't find PN53x board");
        while (1)
            ; // halt
    }

    // Got ok data, print it out!
    Serial.print("Found chip PN5");
    Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. ");
    Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.');
    Serial.println((versiondata >> 8) & 0xFF, DEC);

#endif

_nfc.setPassiveActivationRetries(0x01);

    // configure board to read RFID tags
_nfc.SAMConfig();

#ifdef DEBUG
    Serial.println("Waiting for an ISO14443A card");
#endif
}

String pn532ext::readCardId()
{

    String _uid = "";
    boolean success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
    uint8_t uidLength;                     // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = _nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

    if (success)
    {
        #ifdef DEBUG
        Serial.println("Found a card!");
        Serial.print("UID Length: ");
        Serial.print(uidLength, DEC);
        Serial.println(" bytes");
        Serial.print("UID Value: ");
        #endif

        for (uint8_t i = 0; i < uidLength; i++)
        {
            _uid += (uid[i] < 0x10 ? "0" : "") + String(uid[i], HEX) + (i != 3 ? ":" : "");
        }
        _uid.toUpperCase();
        #ifdef DEBUG
        Serial.println(_uid);
        #endif
    }
    else
    {
// PN532 probably timed out waiting for a card
#ifdef DEBUG
        Serial.println("Timed out waiting for a card");
#endif
    }

    return _uid;

}