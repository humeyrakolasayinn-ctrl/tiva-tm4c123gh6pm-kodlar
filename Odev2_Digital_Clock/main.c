#include "stdint.h"
#include "stdbool.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "driverlib/sysctl.h"
#include "driverlib/sysctl.c"
#include "driverlib/pin_map.h"
#include "math.h"
#include "inc/hw_gpio.h"
#include "driverlib/timer.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "lcd.h"

void initmikro();
void timerkesme();


int sn, dk, sa;


int main (void)
{
    initmikro();
    while(1){

    }
}


void initmikro()
{

    SysCtlClockSet(SYSCTL_DSLP_OSC_MAIN |SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL |SYSCTL_SYSDIV_5 ); // 40MHz

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);


    SysCtlPeripheralEnable(LCDPORTENABLE);
    Lcd_init();
    Lcd_Temizle();
    Lcd_Goto(1, 1); // İmleci 1. satır, 1. sütuna getir

    // --- TİMER AYARLARI ---
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 4000000-1);

    IntMasterEnable();
    IntEnable(INT_TIMER0A);

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, timerkesme);

    TimerEnable(TIMER0_BASE, TIMER_A);
}

void timerkesme()
{

    int onlar, birler;

    // HIZLI AKIŞ SAYACI--0.1 saniyede bir artar

        sn++;


    // SN, DK, SA KONTROL MANTIKLARI
    if (sn==60){
        sn=0; dk++;
    }
    if(dk==60){
        dk=0; sa++;
    }
    if(sa==24){
        sa=0;
    }


    Lcd_Goto(1, 1); // Saati aynı konuma baştan yazmak için

    // SAAT (HH)
    onlar  = sa / 10;
    birler = sa % 10;
    Lcd_Putch(onlar + '0');
    Lcd_Putch(birler + '0');
    Lcd_Putch(':');

    // DAKİKA (MM)
    onlar  = dk / 10;
    birler = dk % 10;
    Lcd_Putch(onlar + '0');
    Lcd_Putch(birler + '0');
    Lcd_Putch(':');

    // SANİYE (SS)
    onlar  = sn / 10;
    birler = sn % 10;
    Lcd_Putch(onlar + '0');
    Lcd_Putch(birler + '0');




    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
    TimerIntClear(TIMER0_BASE, TIMER_A);
}
