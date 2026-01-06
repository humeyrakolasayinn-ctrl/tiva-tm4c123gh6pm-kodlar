

#include <stdint.h>
#include <stdbool.h>

#ifndef LCD_H_
#define LCD_H_

// --- PIN TANIMLAMALARI (Sizin Tanımladığınız Şekilde) ---
#define LCDPORT         GPIO_PORTB_BASE
#define LCDPORTENABLE   SYSCTL_PERIPH_GPIOB
#define RS              GPIO_PIN_0
#define E               GPIO_PIN_1
#define RW              GPIO_PIN_2 // RW pinini de ekledik (Varsayım: B2)
#define D4              GPIO_PIN_4
#define D5              GPIO_PIN_5
#define D6              GPIO_PIN_6
#define D7              GPIO_PIN_7

// --- FONKSİYON PROTOTİPLERİ ---
void Lcd_Komut(uint8_t cmd);     // Lcd'ye komut göndermeye yarar
void Lcd_Temizle(void);          // Lcd ekranını temizler
void Lcd_Puts(char* str);        // String ifade yazdırır
void Lcd_Goto(char row, char col); // Kursorü istenilen yere gönderir
void Lcd_init(void);             // Lcd başlangıç ayarları
void Lcd_Putch(uint8_t data);    // Tek karekter yazdırır
void Lcd_EnablePulse(void);      // Dahili fonksiyon prototipi

#endif /* LCD_H_ 
