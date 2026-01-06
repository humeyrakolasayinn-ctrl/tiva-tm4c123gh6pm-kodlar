#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

// --- LCD GECİKMELERİ İÇİN YARDIMCI FONKSİYON ---
void beklet(void) {
  
    SysCtlDelay(50000); 
}

// --- LCD'YE KOMUT GÖNDERME FONKSİYONU ---
void Lcd_Cmd(unsigned char cp) {
    // RS = 0 (Komut Modu)
    GPIOPinWrite(LCDPORT, RS, 0x00);
    
  
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (cp & 0xF0));
 
    GPIOPinWrite(LCDPORT, E, E); 
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0x00);
    
 
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (cp & 0x0F) << 4);
   
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0x00);
    
    beklet(); 
}

// --- LCD'YE KARAKTER GÖNDERME FONKSİYONU ---
void Lcd_Putch(unsigned char cp) {
    // RS = 1 (Veri/Karakter Modu)
    GPIOPinWrite(LCDPORT, RS, RS);
    
    // YÜKSEK 4-BİT GÖNDERİMİ
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (cp & 0xF0));
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0x00);
    
    // DÜŞÜK 4-BİT GÖNDERİMİ
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (cp & 0x0F) << 4);
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0x00);
    
    beklet(); 
}

// --- LCD BAŞLATMA FONKSİYONU ---
void Lcd_init(void) {
    
    GPIOPinTypeGPIOOutput(LCDPORT, RS | E | D4 | D5 | D6 | D7);
    
  
    SysCtlDelay(50000);
    Lcd_Cmd(0x03); 
    Lcd_Cmd(0x02);
    
    Lcd_Cmd(0x28); 
    Lcd_Cmd(0x0C); 
    Lcd_Cmd(0x06); 
    Lcd_Cmd(0x01); 
    SysCtlDelay(50000);
}

// --- LCD EKRANI TEMİZLEME FONKSİYONU ---
void Lcd_Temizle(void) {
    Lcd_Cmd(0x01); 
    SysCtlDelay(50000);
}

// --- İMLECİ BELİRLİ KOORDİNATA TAŞIMA ---
void Lcd_Goto(char y, char x) {
    if (y == 1)
       
        Lcd_Cmd(0x80 + (x - 1));
    else
       
        Lcd_Cmd(0xC0 + (x - 1));
}
