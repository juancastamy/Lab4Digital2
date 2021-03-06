/*
 * Second lab for Digital 2 course
 * File:   lab_4.c
 * Author: Juan Diego Castillo Amaya
 * Student ID: 17074
 *
 * Created on Feb. 16, 2020
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include "SPI_SLAVE.h"
#include "OSCI_SLAVE.h"
#define _XTAL_FREQ 8000000
void setup(void);
void ADC (void);
uint8_t adc = 0;
uint8_t adc2 = 0;

 
//void __interrupt() ISR(void){
//    if(SSPIF == 1){
//          PORTD = spiRead();
//          if (PORTD == 1){
//              spiWrite(adc);
//           }
//           if (PORTD == 2){
//               spiWrite(adc2);
//           }
//        SSPIF = 0;
//    }
//}

void main(void) {
    setup();
    initOsc(7);
    ADC();
    
}

void setup (void){
    
    TRISA = 0b00001001;
    TRISB = 0;
    TRISC = 0b00001000;
    TRISD = 0;
    ANSEL = 0b00001001;
    ANSELH = 0;

//    INTCONbits.GIE = 1;         // Habilitamos interrupciones
//    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
//    INTCONbits.T0IE = 1;
//    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
//    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1; 
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}

void ADC (void){
    ADCON0bits.ADCS0 = 0;
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADON = 1;   // adc on
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    PORTA=0;
    PORTB=0;
    PORTC=0;
    PORTD=0;
   
    while(1){
        PORTD = spiRead();
        __delay_ms(1);
        if (PORTD == 1){
        ADCON0bits.CHS0 = 0;
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS3 = 0;
        __delay_us(600);
        ADCON0bits.ADON = 1;
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1);
        adc = ADRESH;
        spiWrite(adc);
        }
       __delay_us(600);
        if(PORTD == 2){ 
        ADCON0bits.CHS0 = 1;
        ADCON0bits.CHS1 = 1;
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS3 = 0;
        __delay_us(600);
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1);
        adc2 = ADRESH; 
        spiWrite(adc2);
        }
    }
    return;
}