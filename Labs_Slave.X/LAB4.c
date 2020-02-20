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
#define _XTAL_FREQ 4000000
#include <stdint.h>
void setup(void);
void SPI_WRITE_SV (uint8_t DATA);
void ADC (void);
void SPI_Slave_Init();
uint8_t SPI_Read();
uint8_t Data;
uint8_t adc;
uint8_t adc2;
 
void __interrupt() ISR(void){
    if (SSPIF == 1){
        PORTD = SPI_Read();
        if (PORTD==1){
            SPI_WRITE_SV(adc);
        }
//        if (PORTD == 2){
//            SPI_WRITE_SV(adc2);
//        }
        SSPIF = 0;
    }
}

void main(void) {
    setup();
    SPI_Slave_Init(); 
    ADC();
}

void setup (void){
    TRISB = 0;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA3 = 1;
    OSCCONbits.IRCF = 0b110; //8Mhz
    OSCCONbits.OSTS= 0;
    OSCCONbits.HTS = 0;
    OSCCONbits.LTS = 0;
    OSCCONbits.SCS = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE=1;
    PIE1bits.SSPIE = 1;
    ANSEL = 0b00001001;
    ANSELH = 0;
}
//OBTENIDO DE  https://deepbluembedded.com/spi-tutorial-with-pic-microcontrollers/ 
void SPI_Slave_Init(){
//MODO ESCLAVO + SS ENCENDIDO
    SSPCONbits.SSPM = 0b0100;
//SINCRONO
    SSPCONbits.SSPEN = 1;
// CONFIGURACION DEL LA POLARIDAD CLO CLK Y FASE DE FINCIONAMIENTO
    SSPCONbits.CKP = 1;
    SSPSTATbits.CKE = 0;
  // Clear The SMP Bit
    SSPSTATbits.SMP = 0;
  // CONFIGURACION DE PINES
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC5 = 0; // SDO -> Output
    TRISCbits.TRISC3 = 1; // SCK -> Intput
    TRISAbits.TRISA5 = 1;
}

void SPI_WRITE_SV (uint8_t DATA){
    SSPBUF=DATA;
  
}

uint8_t SPI_Read(){
  uint8_t Data;
  if(BF){
    Data = SSPBUF; // Read The Buffer
    BF = 0; // Clear The Buffer-Filled Indicator Bit
    SSPIF = 0; // Clear The Interrupt Flag Bit
    SSPOV = 0; // Clear The Overflow Indicator Bit
    return Data;
  }
}

void ADC (void){
    while(1){
    ADCON0bits.ADCS0 = 0;
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADON = 1;   // adc on
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    while(1){
        __delay_ms(1);
        ADCON0bits.CHS = 0b0000;
        ADCON0bits.ADON = 1;
        ADCON0bits.GO = 1;
        PIR1bits.ADIF = 0;
        //adc = ADRESH;
        PORTB = ADRESH;
        
//        __delay_ms(5);
//        ADCON0bits.CHS = 0b0011;
//        ADCON0bits.ADON = 1;   // adc on
//        ADCON0bits.GO = 1;
//        PIR1bits.ADIF = 0;
//        adc2 = ADRESH; 
    }
    }
    return;
}