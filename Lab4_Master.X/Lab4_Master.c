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
#include <stdint.h>
#define _XTAL_FREQ 4000000
uint8_t Data;
uint8_t pot1;
uint8_t pot2;
void setup(void);
void spi_init (void);
void SPI_WRITE (uint8_t DATA);
uint8_t SPI_READ_MT ();

void __interrupt() ISR(){
    if (SSPIF == 1){
        PORTB = SSPBUF;
        BF=0;
        SSPIF==0;
        
    }
    return;
}

void main(void) {
    setup();
    spi_init();
    while(1){
    PORTAbits.RA0 = 0;
    __delay_ms(1);
    SPI_WRITE(1);
    pot1 = SPI_READ_MT ();
    PORTB = pot1;
//    __delay_ms(1);
//    PORTAbits.RA0 = 1;
//    __delay_ms(1);
//    PORTAbits.RA0 = 0;
//    __delay_ms(1);
//    SPI_WRITE(2);
//    pot2 = SPI_READ_MT ();
//    PORTAbits.RA0 = 1;
    }
}
void setup(void){
    TRISB=0;
    PORTB=0;
    TRISAbits.TRISA0 = 0;
    OSCCONbits.IRCF = 0b110; //4Mhz
    OSCCONbits.OSTS= 0;
    OSCCONbits.HTS = 0;
    OSCCONbits.LTS = 0;
    OSCCONbits.SCS = 1;   
    INTCONbits.GIE = 1;
    INTCONbits.PEIE=1;
    PIE1bits.SSPIE = 1;
    PORTAbits.RA0 = 0;
}

void spi_init (void){
   SSPSTATbits.CKE=0;
   SSPCONbits.CKP = 0;
   
   SSPSTATbits.SMP=0;
           
//sincrono
   SSPCONbits.SSPEN = 1;
//PINES
   TRISCbits.TRISC4 = 1;
   TRISCbits.TRISC5 = 0;
   TRISCbits.TRISC3 = 0;
   SSPCONbits.SSPM=0b0010;
   
}
void SPI_WRITE (uint8_t DATA){
    SSPBUF=DATA;
}

uint8_t SPI_READ_MT (){
  if(BF==1) // Check If Any New Data Is Received
  {
    Data = SSPBUF; // Read The Buffer
    BF = 0; // Clear The Buffer-Filled Indicator Bit
    SSPOV = 0;
  }
  return Data;
}
