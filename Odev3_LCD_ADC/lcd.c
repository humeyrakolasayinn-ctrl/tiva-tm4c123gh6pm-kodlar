#include "lcd.h"
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// --- YARDIMCI GECİKME FONKSİYONU ---
void beklet(void) {
    // LCD'nin komutları işleyebilmesi için gereken mikro saniyelik bekleme
    SysCtlDelay(50000); 
}

// --- LCD'YE KOMUT GÖNDERİMİ (4-BIT) ---
void Lcd_Cmd(unsigned char cp) {
    // RS = 0 yapılarak komut gönderileceği belirtilir
    GPIOPinWrite(LCDPORT, RS, 0x00);
    
    // ÜST 4 BİT: Komutun ilk yarısı gönderilir
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (cp & 0xF0));
    GPIOPinWrite(LCDPORT, E, E); // Enable tetikle
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0x00);
    
    // ALT 4 BİT: Komutun ikinci yarısı 4 bit sola kaydırılarak gönderilir
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (cp & 0x0F) << 4);
    GPIOPinWrite(LCDPORT, E, E); // Enable tetikle
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0x00);
    
    beklet();
}

// --- LCD'YE VERİ/KARAKTER GÖNDERİMİ ---
void Lcd_Putch(unsigned char cp) {
    // RS = 1 yapılarak veri/karakter gönderileceği belirtilir
    GPIOPinWrite(LCDPORT, RS, RS);
    
    // ÜST 4 BİT GÖNDERİMİ
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (cp & 0xF0));
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0x00);
    
    // ALT 4 BİT GÖNDERİMİ
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (cp & 0x0F) << 4);
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0x00);
    
    beklet();
}

// --- LCD BAŞLATMA (INITIALIZATION) ---
void Lcd_init(void) {
    // Pinleri çıkış (output) olarak yapılandır
    GPIOPinTypeGPIOOutput(LCDPORT, RS | E | D4 | D5 | D6 | D7);
    
    SysCtlDelay(50000);
    Lcd_Cmd(0x03); // Sistem başlatma
    Lcd_Cmd(0x02); // 4-bit modunu aktif etme komutu
    
    Lcd_Cmd(0x28); // 4-bit, 2 satır, 5x7 matris ayarı
    Lcd_Cmd(0x0C); // Ekran açık, imleç kapalı
    Lcd_Cmd(0x06); // Giriş modu ayarı (sağa doğru yazım)
    Lcd_Cmd(0x01); // Ekranı temizle
    SysCtlDelay(50000);
}

// --- EKRAN TEMİZLEME ---
void Lcd_Temizle(void) {
    Lcd_Cmd(0x01);
    SysCtlDelay(50000);
}

// --- KOORDİNAT AYARI ---
void Lcd_Goto(char y, char x) {
    if (y == 1)
        Lcd_Cmd(0x80 + (x - 1)); // 1. satır adresi
    else
        Lcd_Cmd(0xC0 + (x - 1)); // 2. satır adresi
}
