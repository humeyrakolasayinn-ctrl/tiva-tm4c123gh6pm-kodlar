#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "Lcd.h"



// E pinine kısa bir darbe üretir
static void Lcd_EnablePulse(void) {
    GPIOPinWrite(LCDPORT, E, E);   // E = 1
    SysCtlDelay(2000);             // kısa bekleme (~1 ms)
    GPIOPinWrite(LCDPORT, E, 0);   // E = 0
    SysCtlDelay(2000);
}

// 4 bit veri gönderir (yüksek veya düşük nibble)
static void Lcd_Send4Bits(uint8_t data) {
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, data & (D4 | D5 | D6 | D7));
    Lcd_EnablePulse();
}

void Lcd_Komut(uint8_t cmd) {
    uint8_t highNibble = cmd & 0xF0;
    uint8_t lowNibble  = (cmd << 4) & 0xF0;

    GPIOPinWrite(LCDPORT, RS, 0x00); // RS = 0 (komut modu)

    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, highNibble);
    Lcd_EnablePulse();

    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, lowNibble);
    Lcd_EnablePulse();

    SysCtlDelay(50000);
}

void Lcd_Putch(uint8_t data) {
    uint8_t highNibble = data & 0xF0;
    uint8_t lowNibble  = (data << 4) & 0xF0;

    GPIOPinWrite(LCDPORT, RS, RS); // RS = 1 (veri modu)

    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, highNibble);
    Lcd_EnablePulse();

    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, lowNibble);
    Lcd_EnablePulse();

    SysCtlDelay(50000);
}

void Lcd_init(void) {

    SysCtlPeripheralEnable(LCDPORTENABLE);
    while(!SysCtlPeripheralReady(LCDPORTENABLE));

    GPIOPinTypeGPIOOutput(LCDPORT, RS | E | D4 | D5 | D6 | D7);
    SysCtlDelay(50000);

    // RW pinini GND'ye bağladığını unutma!

    // Başlangıç sekansı (4-bit mod)
    GPIOPinWrite(LCDPORT, RS | E | D4 | D5 | D6 | D7, 0x00);
    SysCtlDelay(150000);

    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, 0x30); Lcd_EnablePulse();
    SysCtlDelay(50000);
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, 0x30); Lcd_EnablePulse();
    SysCtlDelay(50000);
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, 0x20); Lcd_EnablePulse(); // 4-bit modu başlat
    SysCtlDelay(50000);

    // Konfigürasyon komutları
    Lcd_Komut(0x28); // 4-bit, 2 satır, 5x8 font
    Lcd_Komut(0x0C); // Ekran açık, imleç kapalı
    Lcd_Komut(0x06); // Otomatik imleç sağa kaydırma
    Lcd_Temizle();   // Ekranı temizle
    Lcd_Komut(0x02); // İmleç başa
}

void Lcd_Temizle(void) {
    Lcd_Komut(0x01);
    SysCtlDelay(50000);
}

void Lcd_Goto(char row, char col) {
    uint8_t addr;
    if (row == 1) addr = 0x80 + (col - 1);
    else          addr = 0xC0 + (col - 1);
    Lcd_Komut(addr);
}

void Lcd_Puts(char *str) {
    while(*str) {
        Lcd_Putch(*str++);
    }
}


