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
#define _XTAL_FREQ 8000000
void setup(void);

void ADC (void);
uint8_t adc;
uint8_t adc2;
 
void __interrupt() ISR(void){
    PORTD = spiRead();
    if (SSPIF == 1){ 
        if (PORTD ==1){
         spiWrite(adc);
    } 
        if (PORTD == 2){
          spiWrite(adc2);
        }
        SSPIF = 0;  
    }
}

void main(void) {
    setup();
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    ADC();
}

void setup (void){
    OSCCONbits.IRCF = 0b111; //8Mhz
    OSCCONbits.OSTS= 0;
    OSCCONbits.HTS = 0;
    OSCCONbits.LTS = 0;
    OSCCONbits.SCS = 1;
    TRISB = 0;
    TRISD = 0;
    TRISDbits.TRISD1 = 0;

    PORTB = 0;
    PORTD = 0;

    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1; 
    TRISA = 0b00001001;
    ANSEL = 0b00001001;
    ANSELH = 0;
}

void ADC (void){
    ADCON0bits.ADCS0 = 0;
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADON = 1;   // adc on
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    while(1){
        __delay_ms(5);
        ADCON0bits.CHS = 0b0000;
        ADCON0bits.ADON = 1;
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;
        adc = ADRESH;
        PORTB = adc;        
        __delay_ms(15);
        ADCON0bits.CHS = 0b0011;
        ADCON0bits.ADON = 1;   // adc on
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;
        adc2 = ADRESH; 
    }
    return;
} 