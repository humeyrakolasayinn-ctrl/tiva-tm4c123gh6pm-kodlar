#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/timer.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "lcd.h"
#include "driverlib/adc.h"
#include "inc/hw_memmap.h"

// --- AYARLAR ---
#define SICAKLIK_KANALI ADC_CTL_CH0 // PE3 pinine bağlı analog kanal

// --- DEĞİŞKENLER ---
volatile int sicaklik_derece = 0; // Okunan analog verinin dönüştürülmüş hali
volatile bool saniye_gecti = false; // Ekran güncelleme zamanlaması

// --- FONKSİYONLAR ---
void Sistem_Ayarlari();
void Timer0_Kesme();
void ADC_Baslat();
int Sicaklik_Hesapla();
void Ekran_Guncelle();
void Lcd_Yazi_Yaz(char *s);

int main (void)
{
    Sistem_Ayarlari(); // Donanım ayarlarını yükle
    Lcd_init();        // LCD'yi hazırla
    Lcd_Temizle();
    
    while(1)
    {
        // Saniyede bir ADC ölçümü yap ve ekranı güncelle
        if(saniye_gecti == true)
        {
            saniye_gecti = false; // Bayrağı sıfırla
            sicaklik_derece = Sicaklik_Hesapla(); // Analog veriyi oku
            Ekran_Guncelle(); // Sonucu LCD'ye yaz
        }
    }
}

// LCD Üzerinde ADC Verisini Gösteren Fonksiyon
void Ekran_Guncelle()
{
    Lcd_Goto(1, 1);
    Lcd_Yazi_Yaz("ADC DEGERI:");
    
    Lcd_Goto(2, 1);
    // ADC değerini veya hesaplanan sıcaklığı ekrana bas
    Lcd_Putch((sicaklik_derece / 10) + '0');
    Lcd_Putch((sicaklik_derece % 10) + '0');
    Lcd_Putch(0xDF); // Derece sembolü
    Lcd_Putch('C');
    Lcd_Yazi_Yaz("            "); // Satır sonunu temizle
}

// Analog Veriyi Dijital Veriye Çeviren Fonksiyon
int Sicaklik_Hesapla() {
    uint32_t raw[1];
    ADCProcessorTrigger(ADC0_BASE, 3); // ADC tetikle
    while(!ADCIntStatus(ADC0_BASE, 3, false)); // Bitmesini bekle
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, raw); // Ham veriyi al

    // 0-3.3V arası analog veriyi 12-bit (0-4095) hassasiyetle işleme
    float voltaj = (float)raw[0] * 3.3 / 4096.0;
    return (int)(voltaj * 100.0);
}

// LCD'ye Metin Yazdırma Yardımı
void Lcd_Yazi_Yaz(char *s) { while(*s) Lcd_Putch(*s++); }

// Zamanlayıcı Kesmesi: Ölçüm periyodunu belirler
void Timer0_Kesme() {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    saniye_gecti = true;
}

// ADC Birimini Yapılandırma
void ADC_Baslat() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); // PE3 pinini analog giriş yap
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, SICAKLIK_KANALI | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
}

// Genel Donanım Ayarları
void Sistem_Ayarlari() {
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(LCDPORTENABLE);
    
    ADC_Baslat(); // ADC'yi aktif et
    
    // Timer ayarları: Saniyede bir kesme üretir
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC); 
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());
    IntMasterEnable(); 
    IntEnable(INT_TIMER0A); 
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0_Kesme); 
    TimerEnable(TIMER0_BASE, TIMER_A);
}
