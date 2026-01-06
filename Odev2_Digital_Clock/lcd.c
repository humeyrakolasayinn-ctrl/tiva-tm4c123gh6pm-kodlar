#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "Lcd.h"

// 40 MHz için ~1 ms bekleme (3 çevrim = 1 döngü)
#define DELAY_MS_1 SysCtlDelay(40000 / 3)
// 40 MHz için ~2 ms bekleme
#define DELAY_MS_2 SysCtlDelay(80000 / 3)
// Uzun başlangıç beklemesi
#define DELAY_MS_100 SysCtlDelay(4000000 / 3)

// E pinine kısa bir darbe üretiyoruz
void Lcd_EnablePulse(void) {
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(200);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(200);
}

void Lcd_Komut(uint8_t cmd) {

    uint8_t highNibble = cmd & 0xF0;          // Yüksek 4 bit
    uint8_t lowNibble  = (cmd & 0x0F) << 4;   // Düşük 4 bit (D7-D4 pin konumuna kaydırıldı)

    GPIOPinWrite(LCDPORT, RS, 0x00); // RS = 0 (komut modu)


    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, highNibble);
    Lcd_EnablePulse();


    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, lowNibble);
    Lcd_EnablePulse();

    DELAY_MS_1;
}

void Lcd_Putch(uint8_t data) {
    // 4-bit mantık HATASI GİDERİLDİ
    uint8_t highNibble = data & 0xF0;
    uint8_t lowNibble  = (data & 0x0F) << 4;

    GPIOPinWrite(LCDPORT, RS, RS); // RS = 1 (veri modu)

    // Yüksek Nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, highNibble);
    Lcd_EnablePulse();

    // Düşük Nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, lowNibble);
    Lcd_EnablePulse();

    SysCtlDelay(200); // Kısa bekleme
}

void Lcd_init(void) {
    // Port aktif etme ve GPIO ayarları main.c'ye taşındı,
    // ancak pin ayarları bu fonksiyonda doğru yapıldı.

    // GPIO Pinlerini Output olarak ayarla (RW eklendi)
    GPIOPinTypeGPIOOutput(LCDPORT, RS | E | RW | D4 | D5 | D6 | D7);
    GPIOPinWrite(LCDPORT, RW, 0x00); // RW = 0 (Yazma) olarak ayarlandı

    // Uzun başlangıç beklemesi
    DELAY_MS_100;

    // --- Başlangıç Sekansı (4-bit mod) ---
    GPIOPinWrite(LCDPORT, RS | E, 0x00);

    // Sadece yüksek 4 bit gönderilir
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, 0x30); Lcd_EnablePulse();
    DELAY_MS_2;
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, 0x30); Lcd_EnablePulse();
    DELAY_MS_2;
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, 0x30); Lcd_EnablePulse();
    DELAY_MS_2;
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, 0x20); Lcd_EnablePulse(); // 4-bit modu başlat
    DELAY_MS_2;


    Lcd_Komut(0x28); // 4-bit, 2 satır, 5x8 font
    Lcd_Komut(0x0C); // Ekran açık, imleç kapalı
    Lcd_Komut(0x06); // Otomatik imleç sağa kaydırma
    Lcd_Temizle();   // Ekranı temizleme
}

void Lcd_Temizle(void) {
    Lcd_Komut(0x01);
    DELAY_MS_2;
}

void Lcd_Goto(char row, char col) {
    uint8_t addr;
    // Satır 1=0x80, Satır 2=0xC0. Sütun 1'den başlasın diye col-1 yaptık.
    if (row == 1) addr = 0x80 + (col - 1);
    else          addr = 0xC0 + (col - 1);
    Lcd_Komut(addr);
}

void Lcd_Puts(char *str) {
    while(*str) {
    Lcd_Putch(*str++);
    }
}
