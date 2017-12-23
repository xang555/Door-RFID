#include "LCD.h"

LCD::LCD(uint8_t cols , uint8_t rows,uint8_t addrs){
    this->mlcd = LiquidCrystal_I2C(addrs,cols,rows);
}

void LCD::lcd_default_screen(){
    this->mlcd.begin();
    this->mlcd.backlight();
    this->mlcd.setCursor(4,0);
    this->mlcd.print("LaoItDev");
    this->mlcd.setCursor(0,1);
    this->mlcd.print("Setting  <Enter>");
}

void LCD::lcd_login_screen() {
    this->mlcd.begin();
    this->mlcd.backlight();
    this->mlcd.setCursor(1,0);
    this->mlcd.print("Enter Password");
    this->mlcd.setCursor(4,1);
    this->mlcd.cursor_on();
}

void LCD::lcd_option_menu_screen(){
    this->mlcd.begin();
    this->mlcd.backlight();
    this->mlcd.setCursor(0,0);
    this->mlcd.print("Add Card <UP>");
    this->mlcd.setCursor(0,1);
    this->mlcd.print("Delete Card <Dn>");
}

void LCD::printchar(const char ch[]){
    this->mlcd.printstr(ch);
}

void LCD::posion(uint8_t col , uint8_t row){
    this->mlcd.setCursor(col,row);
}