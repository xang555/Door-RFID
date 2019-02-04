#include "rfid.h"

#ifndef DEBUG
#define DEBUG
#endif

rfid::rfid(uint8_t ss_pin,uint8_t rst_pin) {
  this->m_ss_pin = ss_pin;
  this->m_rst_pin = rst_pin;
}

void rfid::initRFID(){
 this->mrfid = MFRC522(this->m_ss_pin,this->m_rst_pin);
 SPI.begin();
 this->mrfid.PCD_Init();
}

String rfid::readRDIDCard(){

if (!this->mrfid.PICC_IsNewCardPresent()){
  #ifdef DEBUG
    Serial.println(F("PICC_IsNewCardPresent!"));
  #endif
  return "";
}

if(!this->mrfid.PICC_ReadCardSerial()){
  #ifdef DEBUG
    Serial.println(F("PICC_ReadCardSerial!"));
  #endif
  return "";
}

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = this->mrfid.PICC_GetType(this->mrfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    #ifdef DEBUG
      Serial.println(F("Your tag is not of type MIFARE Classic."));
    #endif
    return "";
  }
  String strID = "";
  for (byte i = 0; i < this->mrfid.uid.size; i++) {
    strID +=
    (this->mrfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(this->mrfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
  this->mrfid.PICC_HaltA();
  this->mrfid.PCD_StopCrypto1();
  return strID;
}
