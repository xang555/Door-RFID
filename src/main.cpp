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
#define RELAY_SWITCH 8
#define ESP_O1_COMMAND 6
#define EXIT_INSIDE_BUTTON 7

//#define DEBUG

const uint8_t NAP_BUTTON = 0;
const uint8_t BAM_BUTTON = 1;
const uint8_t ENT_BUTTON = 2;
const uint8_t EXT_BUTTON = 3;


rfid mrfid(SS_PIN,RST_PIN); // RFID 
LCD mlcd(16,2);

// ----------- buzzer -----------------//
void tone_on(){
  analogWrite(A1,500);
  delay(50);
  analogWrite(A1,0);
}

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
    for(int recno = 1 ; recno <= db.count(); recno ++){
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
     randomSeed(analogRead(A0));   
     uid = random(1000,9999);
     users.uid = uid;
     strcpy(users.uuid,uuid.c_str());
     db.appendRec(EDB_REC users);
    }
    return uid;
} // add cardd to database

bool deleteByCardId(String uuid)
{
  for(int recno = 1 ; recno <= db.count(); recno ++){
        db.readRec(recno, EDB_REC users);
         if(strcmp(users.uuid,uuid.c_str()) == 0){
           EDB_Status status = db.deleteRec(recno);
           if(status == EDB_OK) return true;
         }   
    }
    return false;  
} // delete by uuid

bool deleteByUid(int uid)
{
  for(int recno = 1 ; recno <= db.count(); recno ++){
        db.readRec(recno, EDB_REC users);
         if(users.uid == uid){
           EDB_Status status  =  db.deleteRec(recno);
             if(status == EDB_OK){
                 return true;
             }
         }   
    }  
    return false;
} // delete by uid

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
    #ifdef DEBUG
    Serial.println("NEXT  ");
    Serial.print(state);
    Serial.println();
    #endif

    return state ;

  }else if(digitalRead(BACK_AND_MINUST_BUTTON) == LOW){

  if(checkButtonIsClick(BACK_AND_MINUST_BUTTON)){
     state = 1;
  }  

    #ifdef DEBUG
   Serial.println("BACK ");
    Serial.print(state);
    Serial.println();
    #endif

    return state ;

  }else if(digitalRead(ENTER_BUTTON) == LOW){
  
    if(checkButtonIsClick(ENTER_BUTTON)){
     state = 2;
    }  

    #ifdef DEBUG
    Serial.println("ENTER  ");
    Serial.print(state);
    Serial.println();
    #endif

    return state ;

  }else if(digitalRead(EXIT_BUTTON) == LOW){

    if(checkButtonIsClick(EXIT_BUTTON)){
     state = 3;
     } 

    #ifdef DEBUG
    Serial.println("EXIT  ");
    Serial.print(state);
    Serial.println(); 
    #endif

   return state; 
 }

 #ifdef DEBUG
  Serial.println("NO KEY");
 #endif

 return -1;
}

/* handle menual open button click and reivce command from esp01 */
 bool is_esp_open_door(){
     if(digitalRead(ESP_O1_COMMAND) == HIGH){
         return true;
     }
    return false;
 } //esp send command open door   

bool is_exit_inside_click() {
    if(digitalRead(EXIT_INSIDE_BUTTON) == LOW) {
        return true;
    }
    return false;
}

/* ----------------- handle scan rfid card ----------------*/
void handleScanCard(uint8_t& state) {
 
    mlcd.lcd_default_screen();

    while(getButtonClick() != ENT_BUTTON) {
        #ifdef DEBUG
        Serial.println("read card and processing");
        #endif
        
        if(is_exit_inside_click()){
             #ifdef DEBUG
              Serial.println("open door inside");
              delay(2000);
              #endif
              tone_on(); // tone 
              digitalWrite(RELAY_SWITCH,HIGH);
              delay(3000);
              digitalWrite(RELAY_SWITCH,LOW);
        }

        if(is_esp_open_door()){ 
             #ifdef DEBUG
              Serial.println("ESP send command for open the door");
              delay(2000);
              #endif
              tone_on(); // tone 
              digitalWrite(RELAY_SWITCH,HIGH);
              delay(3000);
              digitalWrite(RELAY_SWITCH,LOW);
        }

        String uuid = mrfid.readRDIDCard(); // read card
        if(uuid != ""){
          if(findRfidCardByUUID(uuid)){
              #ifdef DEBUG
              Serial.println("Access ok");
              delay(2000);
              #endif
              tone_on(); // tone 
              digitalWrite(RELAY_SWITCH,HIGH);
              delay(3000);
              digitalWrite(RELAY_SWITCH,LOW);
          }else {
              #ifdef DEBUG
              Serial.println("Access fail");
              delay(2000);
              #endif
              tone_on(); //tone
              delay(100);
              tone_on(); // tone
          }

        }

        yield();
    }

    state = 1;
} 

/*------------------- handle enter number ---------------------*/
const uint8_t _position_cursor_default = 4;

String handleEnterNumber(uint8_t& state,uint8_t _position) {

  int num = -1;
  uint8_t col = _position_cursor_default + _position;
  #ifdef DEBUG
  Serial.println(col);
  #endif
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
   }else if(button_key == EXT_BUTTON){
       state = state - 1;
       return "";
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

/* ------------------ handle authentication -------------------------*/

void handleAuthentication(uint8_t& state){

    String passwd = "";
    uint8_t login_state = 0;
    bool exit_login = false;

    while(!exit_login) {

        switch(login_state) {
           case 0 :
                mlcd.lcd_login_screen();
                while(passwd.length() != FIX_ADMIN_PASSWD_LENGHT) {
                    if(state == 0) {
                        return;
                    }
                    passwd += handleEnterNumber(state,passwd.length());  
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
        }else if(btn_key == EXT_BUTTON) {
            state = 0;
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
                            tone_on(); //tone
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
    mlcd.lcd_delete_card_option_screen();
    uint8_t btn_key = -1;
    while(true) {
        btn_key = getButtonClick();
        if(btn_key == NAP_BUTTON) { // delete by Card
            state = 5;
            return;
        }else if(btn_key == BAM_BUTTON) { // deelte by uid
            state = 6;
            return;
        }else if(btn_key == EXT_BUTTON){ // return
            state = 2;
            return;
        }
    }
}


void handleDeleteRfidByCardID(uint8_t& state) {

    uint8_t delete_step_state = 0;

    while(true){

      switch(delete_step_state) {

        case 0 :
                mlcd.lcd_delete_by_card_screen();

                while(delete_step_state == 0){
                  if(getButtonClick() == EXT_BUTTON){
                        delete_step_state = -1;
                  }else {
                    String uuid = mrfid.readRDIDCard();
                    if(uuid != ""){
                        tone_on(); //tone
                        if(deleteByCardId(uuid)){
                            delete_step_state = 1;
                        }else  {
                            delete_step_state = 2;
                        }    
                    }
                }

                } //while

            break;
        
        case 1 :
            mlcd.lcd_delete_finish_screen();
            delay(3000);
            delete_step_state = 0;
            break;

        case 2 :
            mlcd.lcd_delete_fail_screen();
            delay(3000);
            delete_step_state = 0;
            break;

        default :
            state = 4;
            return;            
        }

    }

} // hsndle delete by card id


void handleDeleteRfidByUID(uint8_t& state) {
    String uid = "";
    uint8_t state_del_by_uid = 0;

    while(true) {

     switch(state_del_by_uid) {  

        case 0 :
            mlcd.lcd_delete_by_UID_screen();
            while(uid.length() != 4){
                if(state == 5){ // if state is 5 user is exit 
                    state = 4;
                    state_del_by_uid = -1;
                    return;
                }
                uid += handleEnterNumber(state,uid.length());
                yield();
            }
            state_del_by_uid = 1;

            break;
        
        case 1 :
         if(uid.length() <= 0) {
             state_del_by_uid = 0;
             return;
         }    
         if(deleteByUid(uid.toInt())){
             mlcd.lcd_delete_finish_screen();
             delay(3000);
            state_del_by_uid = -1;
         }else {
             mlcd.lcd_delete_fail_screen();
             delay(3000);
             state_del_by_uid = 0;
         }   

            uid = "";

            break;

        default :
            state = 4;
            return;
     } 

    }

} // handle delete by uid


/*------------------------ setup ----------------------*/

void setup()
{
#ifdef DEBUG    
Serial.begin(9600);
#endif
pinMode(NEXT_AND_PUSH_BUTTON,INPUT);
pinMode(BACK_AND_MINUST_BUTTON,INPUT);
pinMode(ENTER_BUTTON,INPUT);
pinMode(EXIT_BUTTON,INPUT);	
pinMode(RELAY_SWITCH,OUTPUT);
pinMode(ESP_O1_COMMAND,INPUT);
pinMode(EXIT_INSIDE_BUTTON,INPUT);
digitalWrite(RELAY_SWITCH,LOW);
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
    break;
case 5 :
    handleDeleteRfidByCardID(state);
    break;
case 6 :
    handleDeleteRfidByUID(state);
    break;

}

}




