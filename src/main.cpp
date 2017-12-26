#include <Arduino.h>
#include "./rfid/rfid.h"
#include "./lcd/LCD.h"
#include "config.h"
#include "Arduino.h"
#include <EDB.h>
#include <EEPROM.h>

#define TABLE_SIZE 1024 // Arduino 168 or greater
#define RECORDS_TO_CREATE 100 // records

#define RST_PIN	9      
#define SS_PIN	10 

#define NEXT_AND_PUSH_BUTTON 5
#define BACK_AND_MINUST_BUTTON 4
#define ENTER_BUTTON 3
#define EXIT_BUTTON  2
#define RELAY_SWITCH 6

#define DEBUG

const uint8_t NAP_BUTTON = 0;
const uint8_t BAM_BUTTON = 1;
const uint8_t ENT_BUTTON = 2;
const uint8_t EXT_BUTTON = 3;


rfid mrfid(SS_PIN,RST_PIN); // RFID 
LCD mlcd(16,2);

//-------------- databse ---------------//
struct Users {
  int uid;
  char uuid[20];
} users;

// The read and write handlers for using the EEPROM Library
void writer(unsigned long address, byte data)
{
  EEPROM.write(address, data);
}

byte reader(unsigned long address)
{
  return EEPROM.read(address);
}

EDB db(&writer, &reader);


bool findRfidCardByUUID(String uuid){
    for(uint8_t recno = 1 ; recno <= db.count(); recno ++){
        db.readRec(recno, EDB_REC users);
         if(strcmp(users.uuid,uuid.c_str()) == 0){
             return true;
         }   
    }
    return false;
} // find rfid by uuid

long AddRfidCard(String uuid) {
    long uid = 0;
    if(findRfidCardByUUID(uuid)) {
        return 0;
    }else {
     uid = millis();
     users.uid = uid;
     strcpy(users.uuid,uuid.c_str());
     db.appendRec(EDB_REC users);
    }
    return uid;
} // add cardd to database


/* ----------------- get Button Clcik ------------- */

bool checkButtonIsClick(uint8_t btnPin) {

    bool is_click = false;
    uint8_t ButtonPushCounter = 0;  
    uint8_t buttonState = 0;         
    uint8_t lastButtonState = 1;

    while(!is_click) {
    buttonState = digitalRead(btnPin);
    if(buttonState != lastButtonState){
         ButtonPushCounter ++ ;
    }

    lastButtonState = buttonState;

    if(ButtonPushCounter % 2 == 0){
        is_click = true;
    }
        yield();
    }
    
    return is_click;
} // check is button click


uint8_t getButtonClick() {

 uint8_t state = -1;

 if(digitalRead(NEXT_AND_PUSH_BUTTON) == LOW){

  if(checkButtonIsClick(NEXT_AND_PUSH_BUTTON)){
     state = 0;
  }  

    Serial.println("NEXT  ");
    Serial.print(state);
    Serial.println();

    return state ;

  }else if(digitalRead(BACK_AND_MINUST_BUTTON) == LOW){

  if(checkButtonIsClick(BACK_AND_MINUST_BUTTON)){
     state = 1;
  }  

   Serial.println("BACK ");
    Serial.print(state);
    Serial.println();

    return state ;

  }else if(digitalRead(ENTER_BUTTON) == LOW){
  
    if(checkButtonIsClick(ENTER_BUTTON)){
     state = 2;
    }  

    Serial.println("ENTER  ");
    Serial.print(state);
    Serial.println();

    return state ;

  }else if(digitalRead(EXIT_BUTTON) == LOW){

 if(checkButtonIsClick(EXIT_BUTTON)){
     state = 3;
  } 

   Serial.println("EXIT  ");
    Serial.print(state);
    Serial.println(); 

   return state; 
 }

 Serial.println("NO KEY");
 return -1;
}

/* ----------------- handle scan rfid card ----------------*/
void handleScanCard(uint8_t& state) {
 
    mlcd.lcd_default_screen();

    while(getButtonClick() != ENT_BUTTON) {

        Serial.println("read card and processing");

        yield();
    }

    state = 1;
} 


/* ------------------ handle authentication -------------------------*/

const uint8_t _position_cursor_default = 4;

String handleEnterPassword(uint8_t _position) {

  uint8_t num = -1;
  uint8_t col = _position_cursor_default + _position;
  Serial.println(col);
  uint8_t button_key = 0;

 while ((button_key = getButtonClick()) != ENT_BUTTON) {
   if(button_key == NAP_BUTTON) {

       if(num >= 0 && num <= 9){
        num += 1;
        num = num >= 10 ? 0:num;
       }else {
           num = 0;
       }
        String str = String(num);
         mlcd.posion(col, 1);
         mlcd.printchar(str.c_str());
         mlcd.posion(col, 1);

   }else if(button_key == BAM_BUTTON) {
       if(num > 0 && num <= 9){
          num -= 1;
       }else {
           num = 0;
       }
        String str = String(num);
        mlcd.posion(col, 1);
        mlcd.printchar(str.c_str());
        mlcd.posion(col, 1);
   }

    yield();
 }

 if(num  < 0 ) {
    mlcd.posion(col, 1);
    mlcd.printchar("0");
    mlcd.posion(col, 1);
 }   
 mlcd.posion((col + 1) , 1); // next cursor posion
 return num < 0 ? "0":String(num);   
} // handle enter password

void handleAuthentication(uint8_t& state){

    String passwd = "";
    uint8_t login_state = 0;
    bool exit_login = false;

    while(!exit_login) {

        switch(login_state) {
           case 0 :
                mlcd.lcd_login_screen();
                while(passwd.length() != FIX_ADMIN_PASSWD_LENGHT) {
                    passwd += handleEnterPassword(passwd.length());  
                    yield();
                 }
                 
                login_state = 1;

            break;
        
        case 1 :
            if(passwd == FIX_ADMIN_PASSWORD){
                state = 2;
                exit_login = true;    
            }else {
                login_state = 0;
                passwd = "";
            }
            break;
        }

    }


} // handle login 

/* ------------------ handle option menu -------------------*/
void handleOptionMenu(uint8_t& state) {

    mlcd.lcd_option_menu_screen();
    uint8_t btn_key = -1;
    while(true){
        btn_key = getButtonClick();
        if(btn_key == NAP_BUTTON){
           state = 3;  // handle add rfid card
           return;
        }else if(btn_key == BAM_BUTTON){
            state = 4; //handle delete rfid card
            return;
        }

    }

}

/*------------------- handle add rfid card -----------------*/

void handleAddRfidCard(uint8_t& state) {

    uint8_t state_step_add = 0;
    String uuid;
    long id;

    while(true) {

        switch(state_step_add) {

            case 0 :
                #ifdef DEBUG
                    Serial.println("Read rfid card ..");
                #endif
                mlcd.lcd_add_card_screen();

                while(state_step_add == 0){

                    if(getButtonClick() == EXT_BUTTON){
                     state_step_add = 2;
                     }else {
                        uuid = mrfid.readRDIDCard();
                        if(uuid != ""){
                            id = AddRfidCard(uuid);
                            state_step_add = 1; 
                         } 
                 }
                    yield();
                }

                break;
            case 1 :
                if(id < 1000) {
                    mlcd.lcd_clear();
                    mlcd.printchar("Card Already !!!.");
                }else {
                    String _id = String(id);
                    mlcd.lcd_add_finish_screen(_id.c_str(),uuid.c_str());
                }
                while(getButtonClick() != ENT_BUTTON){}; // loop untill user press enter
                state_step_add = 0;
                break;
            case 2 :
                state = 2;
                return;
        }

        yield();

    }

}

/* ------------------- handle Delete rfid card -----------------*/

void handleDeleteRfidCard(uint8_t& state) {



}

/*------------------------ setup ----------------------*/

void setup()
{
Serial.begin(9600);
pinMode(NEXT_AND_PUSH_BUTTON,INPUT);
pinMode(BACK_AND_MINUST_BUTTON,INPUT);
pinMode(ENTER_BUTTON,INPUT);
pinMode(EXIT_BUTTON,INPUT);	
pinMode(RELAY_SWITCH,OUTPUT);
mrfid.initRFID();
mlcd.begin();

#ifdef DEBUG 
     db.create(0, TABLE_SIZE, sizeof(users));
     Serial.print("Record Count: "); Serial.println(db.count());
#else
 if(db.open(0) != EDB_OK) {
    db.create(0, TABLE_SIZE, sizeof(users));
   #ifdef DEBUG 
   Serial.print("Record Count: "); Serial.println(db.count());
   #endif
 }
 #endif

}

void loop()
{

static uint8_t state = 0;

switch(state) {

case 0 :
    handleScanCard(state);
    break;
case 1 :
    handleAuthentication(state);
    break;

case 2 :
    handleOptionMenu(state);
    break;
case 3 :
    handleAddRfidCard(state);
    break;    
case 4:
    handleDeleteRfidCard(state);

}

}




