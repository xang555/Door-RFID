#include <Arduino.h>
#include "./rfid/rfid.h"
#include "./lcd/LCD.h"
#include "config.h"

#define RST_PIN	9      
#define SS_PIN	10 

#define NEXT_AND_PUSH_BUTTON 5
#define BACK_AND_MINUST_BUTTON 4
#define ENTER_BUTTON 3
#define EXIT_BUTTON  2
#define RELAY_SWITCH 6

const uint8_t NAP_BUTTON = 0;
const uint8_t BAM_BUTTON = 1;
const uint8_t ENT_BUTTON = 2;
const uint8_t EXT_BUTTON = 3;


rfid mrfid(SS_PIN,RST_PIN); // RFID 
LCD mlcd(16,2);

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
                while(passwd.length() != 6) {
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
    while(1){}

}


void setup()
{
    Serial.begin(9600);
pinMode(NEXT_AND_PUSH_BUTTON,INPUT);
pinMode(BACK_AND_MINUST_BUTTON,INPUT);
pinMode(ENTER_BUTTON,INPUT);
pinMode(EXIT_BUTTON,INPUT);	
pinMode(RELAY_SWITCH,OUTPUT);
mrfid.initRFID();
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

}

}




