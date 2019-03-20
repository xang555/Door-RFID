  #include <Wire.h>
  #include <PN532_I2C.h>
  #include <PN532.h>
  #include <NfcAdapter.h>

/*
use i2c for communication
*/

class pn532ext
{
  
public:
    pn532ext();
    String readCardId(void);
    void begin(void);
private:
    PN532_I2C _pn532i2c;
    PN532 _nfc;
};


