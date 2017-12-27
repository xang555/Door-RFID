#include <SPI.h>
#include <MFRC522.h>
 
class rfid {

    public:
        rfid(uint8_t ss_pin,uint8_t rst_pin);
        String readRDIDCard();
        void initRFID();
    private:
        uint8_t m_ss_pin;
        uint8_t m_rst_pin;
        MFRC522 mrfid;
};