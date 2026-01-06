/*
 * lcd.h
 *
 *  Created on: 13 Kas 2025
 *      Author: Acer
 */



#include <stdint.h>
#include <stdbool.h>

#ifndef LCD_H_
#define LCD_H_


// --- PIN ve PORT TANIMLAMALARI ---
// Port B kullanımı garanti altına alınmıştır
#define LCDPORT         GPIO_PORTB_BASE
#define LCDPORTENABLE   SYSCTL_PERIPH_GPIOB
#define RS              GPIO_PIN_0
#define E               GPIO_PIN_1
#define RW              GPIO_PIN_2
#define D4              GPIO_PIN_4
#define D5              GPIO_PIN_5
#define D6              GPIO_PIN_6
#define D7              GPIO_PIN_7

// --- FONKSİYON PROTOTİPLERİ ---
void Lcd_Komut(uint8_t komut);
void Lcd_Temizle(void);
void Lcd_Goto(char satir, char sutun);
void Lcd_init(void);
void Lcd_Putch(uint8_t karakter);
void Lcd_EnablePulse(void);

#endif /* LCD_H_ */
