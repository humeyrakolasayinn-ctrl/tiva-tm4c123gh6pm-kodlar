#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include "inc/hw_memmap.h"

// --- LCD DONANIM TANIMLAMALARI ---
// Ödevdeki bağlantı şemasına göre Port B kullanılmıştır
#define LCDPORT          GPIO_PORTB_BASE      // LCD'nin bağlı olduğu port
#define LCDPORTENABLE    SYSCTL_PERIPH_GPIOB  // Port B'nin saat sinyali
#define RS               GPIO_PIN_0           // Register Select pini
#define E                GPIO_PIN_1           // Enable pini
#define D4               GPIO_PIN_4           // Veri pini 4
#define D5               GPIO_PIN_5           // Veri pini 5
#define D6               GPIO_PIN_6           // Veri pini 6
#define D7               GPIO_PIN_7           // Veri pini 7

// --- FONKSİYON PROTOTİPLERİ ---
void Lcd_init(void);                          // LCD başlatma ayarları
void Lcd_Cmd(unsigned char cp);               // LCD'ye komut gönderir
void Lcd_Putch(unsigned char cp);              // LCD'ye tek karakter yazar
void Lcd_Goto(char y, char x);                // İmleci koordinata taşır
void Lcd_Temizle(void);                       // Ekranı tamamen siler

#endif
