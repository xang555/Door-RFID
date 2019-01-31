#include "LCD.h"

LCD::LCD(uint8_t cols , uint8_t rows,uint8_t addrs){
    this->mlcd = LiquidCrystal_I2C(addrs,cols,rows);
}

void LCD::begin(){
    this->mlcd.begin();
    this->mlcd.backlight();
}

void LCD::lcd_default_screen(){
    this->mlcd.clear();
    this->mlcd.setCursor(4,0);
    this->mlcd.print("LaoItDev");
    this->mlcd.setCursor(0,1);
    this->mlcd.print("Setting  <Enter>");
    this->mlcd.cursor_off();
}

void LCD::lcd_login_screen() {
     this->mlcd.clear();
    this->mlcd.setCursor(1,0);
    this->mlcd.print("Enter Password");
    this->mlcd.setCursor(4,1);
    this->mlcd.cursor_on();
}

void LCD::lcd_option_menu_screen(){
    this->mlcd.clear();
    this->mlcd.setCursor(0,0);
    this->mlcd.print("Add Card <UP>");
    this->mlcd.setCursor(0,1);
    this->mlcd.print("Delete Card <Dn>");
    this->mlcd.cursor_off();
}

void LCD::lcd_add_card_screen(){
    this->mlcd.clear();
    this->mlcd.setCursor(0,0);
    this->mlcd.print("Waiting ... card");
    this->mlcd.setCursor(5,1);
    this->mlcd.print("...");
    this->mlcd.cursor_off();
}

void LCD::lcd_add_finish_screen(const char uid[],const char uuid[]){
    this->mlcd.clear();
    this->mlcd.setCursor(0,0);
    this->mlcd.print("uid:");
    this->mlcd.setCursor(4,0);
    this->mlcd.printstr(uid);
    this->mlcd.setCursor(0,1);
    this->mlcd.print("uuid:");
    this->mlcd.setCursor(5,1);
    this->mlcd.printstr(uuid);
    this->mlcd.cursor_off();
}

void LCD::lcd_delete_card_option_screen(){
    this->mlcd.clear();
    this->mlcd.setCursor(2,0);
    this->mlcd.print("by Card <Up>");
    this->mlcd.setCursor(2,1);
    this->mlcd.print("by Uid <Dn>");
    this->mlcd.cursor_off();
}

void LCD::lcd_delete_by_card_screen(){
    this->mlcd.clear();
    this->mlcd.setCursor(0,0);
    this->mlcd.print("Waiting ... Card");
    this->mlcd.setCursor(6,1);
    this->mlcd.print(" ... ");
    this->mlcd.cursor_off();
}

void LCD::lcd_delete_by_UID_screen(){
    this->mlcd.clear();
    this->mlcd.setCursor(4,0);
    this->mlcd.print("Enter UID");
    this->mlcd.setCursor(4,1);
    this->mlcd.cursor_on();
}

void LCD::lcd_delete_finish_screen(){
    this->mlcd.clear();
    this->mlcd.setCursor(1,0);
    this->mlcd.print("Delete Success");
    this->mlcd.cursor_off();
}

void LCD::lcd_delete_fail_screen(){
    this->mlcd.clear();
    this->mlcd.setCursor(2,0);
    this->mlcd.print("Delete Fail");
    this->mlcd.cursor_off();
}

void LCD::printchar(const char ch[]){
    this->mlcd.printstr(ch);
}

void LCD::posion(uint8_t col , uint8_t row){
    this->mlcd.setCursor(col,row);
}

void LCD::lcd_clear(){
    this->mlcd.clear();
}

void LCD::lcd_clear_database_screen(){
    this->mlcd.clear();
    this->mlcd.setCursor(0,0);
    this->mlcd.print("Clear data <Ent>");
    this->mlcd.cursor_off();
}

void LCD::lcd_clear_database_success(){
    this->mlcd.clear();
    this->mlcd.setCursor(1,0);
    this->mlcd.print("Clear success!");
    this->mlcd.cursor_off();
}