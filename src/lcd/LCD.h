#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#ifndef __CONFIG_PROGRAM__
#include "../config.h"
#endif

#ifdef __3F__        
#define ___addrs___     0x3F
#else
#define ___addrs___     0x2F
#endif

class LCD {

public :
    LCD(uint8_t cols , uint8_t rows,uint8_t addrs = ___addrs___ ); // addrs =0x27 and addrs =0x3F
    void begin();
    void lcd_default_screen();
    void lcd_login_screen();
    void lcd_option_menu_screen();
    void lcd_add_card_screen();
    void lcd_add_finish_screen(const char[],const char[]);
    void lcd_delete_card_option_screen();
    void lcd_delete_by_card_screen();
    void lcd_delete_by_UID_screen();
    void lcd_delete_finish_screen();
    void lcd_delete_fail_screen();
    void lcd_clear_database_screen();
    void lcd_clear_database_success();
    void printchar(const char[]);
    void posion(uint8_t col , uint8_t row);
    void lcd_clear();

private :
    LiquidCrystal_I2C mlcd;
    uint8_t mlcd_addr;
    uint8_t mlcd_row;
    uint8_t mlcd_col;
    
};