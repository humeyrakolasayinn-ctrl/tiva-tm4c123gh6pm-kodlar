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
#include "driverlib/uart.h"
#include "inc/hw_uart.h"

// --- AYARLAR VE PIN TANIMLAMALARI ---
#define BUTON_PORT      GPIO_PORTF_BASE
#define BUTON_PIN       GPIO_PIN_4
#define SICAKLIK_KANALI ADC_CTL_CH0      // PE3 pinine bağlı ADC kanalı

// --- GLOBAL DEĞİŞKENLER ---
volatile int sn = 0, dk = 0, sa = 0; // Saat değişkenleri (Kesme içinde değiştiği için volatile)
volatile bool saniye_gecti = false;  // PC'ye veri gönderme tetikleyicisi
volatile int sicaklik_derece = 0;    // Ölçülen sıcaklık değeri
volatile bool sistem_aktif = false;  // PC'den ilk saat gelene kadar bekleme durumu

char lcd_mesaj[10] = "    ";         // LCD'nin 2. satırında görünecek özel mesaj
char giden_paket[50];                // PC'ye gönderilecek veri paketi
char gelen_buffer[15];               // UART üzerinden gelen verilerin toplandığı alan
int indeks = 0;                      // Buffer indeks takibi

void Sistem_Ayarlari();
void Timer0_Kesme();
void ADC_Baslat();
int Sicaklik_Hesapla();
void Ekran_Guncelle();
void Lcd_Yazi_Yaz(char *s);
void PCye_Veri_Gonder(char *veri);

int main (void)
{
    Sistem_Ayarlari();    // Donanım konfigürasyonlarını yaptık
    Lcd_init();           // LCD sürücüsünü başlattık
    Lcd_Temizle();
    Lcd_Goto(1,1);
    Lcd_Yazi_Yaz("PC BEKLENİYOR");     // İlk bağlantı mesajı

    while(1)
    {
        //  PERİYODİK VERİ GÖNDERİMİ VE EKRAN GÜNCELLEMESİ
        // Her saniye başında bir kez çalışır!
        if(saniye_gecti == true && sistem_aktif == true)
        {
            saniye_gecti = false;      // Bayrağı indir
            sicaklik_derece = Sicaklik_Hesapla();     // Sensörden güncel sıcaklığı oku
            int btn = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0) ? 1 : 0;    // Buton basılı mı kontrol et

            // PC'ye gidecek paketi oluştur: T12:00:00S25B1! formatı
            sprintf(giden_paket, "T%02d:%02d:%02dS%02dB%d!\n", sa, dk, sn, sicaklik_derece, btn);
            
            PCye_Veri_Gonder(giden_paket);     // UART üzerinden PC'ye yolla
            Ekran_Guncelle();                  // LCD'deki saat ve verileri yenile
        }

        //  UART ÜZERİNDEN GELEN VERİLERİ DİNLEMESİ
        while (UARTCharsAvail(UART0_BASE))           // Okunacak karakter var mı?
        {
            char gelen = UARTCharGet(UART0_BASE);       // Karakteri al

            // DURUM A: Saat senkronizasyonu (*) karakteri ile biter
            if (gelen == '*')
            {
                // Buffer'daki veriyi saat/dk/sn olarak sayıya çevir
                int yeni_sa = (gelen_buffer[0] - '0') * 10 + (gelen_buffer[1] - '0');
                int yeni_dk = (gelen_buffer[3] - '0') * 10 + (gelen_buffer[4] - '0');
                int yeni_sn = (gelen_buffer[6] - '0') * 10 + (gelen_buffer[7] - '0');
                
                if(yeni_sa < 24 && yeni_dk < 60 && yeni_sn < 60) {
                    sa = yeni_sa; dk = yeni_dk; sn = yeni_sn;
                    sistem_aktif = true; // Saati aldık, sistemi başlat
                    Lcd_Temizle();
                    Ekran_Guncelle();
                }
                indeks = 0; // Bir sonraki paket için buffer'ı sıfırla
            }
            // DURUM B: LCD'ye yazılacak mesaj (#) karakteri ile biter
            else if (gelen == '#')
            {
                if(indeks > 0)
                {
                    int i;
                    for(i=0; i<9; i++) {
                        if(i < indeks) lcd_mesaj[i] = gelen_buffer[i];
                        else lcd_mesaj[i] = ' '; // Eksik kısımları boşlukla doldur
                    }
                    lcd_mesaj[indeks] = '\0'; // String sonlandırıcı
                    if(sistem_aktif) Ekran_Guncelle(); // Hemen ekrana yansıt
                }
                indeks = 0;
            }
            // Karakter biriktirme (Paket bitene kadar buffer'a yaz)
            else
            {
                if(indeks < 14) {
                    gelen_buffer[indeks] = gelen;
                    indeks++;
                }
            }
        }
    }
}

// --- LCD EKRAN YÖNETİMİ ---
void Ekran_Guncelle()
{
    // 1. Satıra güncel saati yazdır (HH:MM:SS)
    Lcd_Goto(1, 1);
    Lcd_Putch((sa / 10) + '0'); Lcd_Putch((sa % 10) + '0'); Lcd_Putch(':');
    Lcd_Putch((dk / 10) + '0'); Lcd_Putch((dk % 10) + '0'); Lcd_Putch(':');
    Lcd_Putch((sn / 10) + '0'); Lcd_Putch((sn % 10) + '0');

    // 2. Satıra PC'den gelen mesajı ve sıcaklığı yazdır
    Lcd_Goto(2, 1);
    Lcd_Yazi_Yaz(lcd_mesaj); // PC'den gelen mesaj
    
    Lcd_Goto(2, 10); // Sıcaklığı sağ tarafa hizala
    Lcd_Putch((sicaklik_derece / 10) + '0');
    Lcd_Putch((sicaklik_derece % 10) + '0');
    Lcd_Putch(0xDF); // Derece simgesi (°)
    Lcd_Putch('C');
}

// --- ADC SICAKLIK ÖLÇÜMÜ ---
int Sicaklik_Hesapla() {
    uint32_t raw[1];
    ADCProcessorTrigger(ADC0_BASE, 3); // ADC ölçümünü tetikle
    while(!ADCIntStatus(ADC0_BASE, 3, false)); // Ölçümün bitmesini bekle
    ADCIntClear(ADC0_BASE, 3); // Bayrağı temizle
    ADCSequenceDataGet(ADC0_BASE, 3, raw); // Veriyi oku

    // LM35 Sensör hesabı (10mV/derece, 3.3V referans, 12 bit ADC)
    float voltaj = (float)raw[0] * 3.3 / 4096.0;
    return (int)(voltaj * 100.0);
}

// --- UART İLETİŞİM FONKSİYONLARI ---
void PCye_Veri_Gonder(char *veri) { 
    while(*veri) UARTCharPut(UART0_BASE, *veri++); // Paketi karakter karakter gönder
}

void Lcd_Yazi_Yaz(char *s) { 
    while(*s) Lcd_Putch(*s++); // String veriyi LCD'ye karakter olarak yolla
}

// --- KESME HİZMET RUTİNİ (ISR) ---
void Timer0_Kesme() {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Kesme bayrağını temizle
    if(sistem_aktif) {
        sn++; // Her saniye artır
        if(sn>59){sn=0; dk++;}
        if(dk>59){dk=0; sa++;}
        if(sa>23){sa=0;}
        saniye_gecti = true; // PC'ye gönderim yapılması gerektiğini ana döngüye bildir
    }
}

// --- DONANIM YAPILANDIRMASI ---
void Sistem_Ayarlari() {
    // İşlemci hızını 80MHz'e ayarla (TivaWare kütüphanesine göre)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    // Birimlerin saat sinyallerini aktif et
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(LCDPORTENABLE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); // UART0 Birimini aç
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // UART pinleri PA0-PA1 üzerindedir

    // UART pin yapılandırması (PA0=RX, PA1=TX)
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    
    // UART Konfigürasyonu: 9600 Baud, 8-N-1 formatı
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTEnable(UART0_BASE);

    // SW2 Buton ayarı (PF4)
    GPIOPinTypeGPIOInput(BUTON_PORT, BUTON_PIN);
    GPIOPadConfigSet(BUTON_PORT, BUTON_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    ADC_Baslat(); // ADC modülünü yapılandır

    // Timer0 Kesme Ayarları (Her 1 saniyede bir kesme)
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()); // 1 saniyelik değer yükle
    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0_Kesme);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void ADC_Baslat() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); // PE3 analog giriş
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); // Sequencer 3 kullanımı
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, SICAKLIK_KANALI | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
}
