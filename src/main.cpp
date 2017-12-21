#include <Arduino.h>
#include "rfid.h"

#define SS_PIN 10
#define RST_PIN 9

rfid mrfid(SS_PIN,RST_PIN);

void setup(){
    Serial.begin(9600);
    mrfid.initRFID();
}

void loop(){

String card = mrfid.readRDIDCard();
if(card == "") return;
Serial.println(card);

}