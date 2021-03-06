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
#include <stdio.h>	
#include "SPI_MASTER.h"
#include "UART.h"
#include "OSCI.h"
#define _XTAL_FREQ 8000000


void setup(void);
uint8_t pot1;
uint8_t pot2;
uint8_t cont;



void main(void) {
    setup();
    initOsc(7);
    UART_INIT(9600);
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
      while (1){
        
        	        
        PORTDbits.RD1 = 0;          //Seleccionar al SLAVE
        __delay_ms (1);             //Delay de 1 milisegundo
        spiWrite(1);
       
        pot1 = spiRead();        //Leer dato preveniente de SLAVE PIC
        
        __delay_ms(1);
        PORTDbits.RD1 = 1;          //Slave Deselect 
        	        
        PORTDbits.RD1 = 0;          //Seleccionar al SLAVE
        __delay_ms (1);             //Delay de 1 milisegundo
        spiWrite(2);
       
        pot2= spiRead();        //Leer dato preveniente de SLAVE PIC
        __delay_ms(1);
        PORTDbits.RD1 = 1;          //Slave Deselect
        
        
        PORTB = UART_READ();     //Valor transmitido por la computadora es le�do y colocado en puerto

        UART_WRITE(pot1);        //Escribir el el registro de UART para transmitir dato 1	        UART_Write(RecPOT1);        //Escribir el el registro de UART para transmitir dato 1
        __delay_ms(5);              //Delay de 5 milisegundos	        __delay_ms(5);              //Delay de 5 milisegundos
        UART_WRITE(pot2);        //Escribir el el registro de UART para transmitir dato 1	        UART_Write(RecPOT2);        //Escribir el el registro de UART para transmitir dato 1

    }	    
    return;
}
void setup(void){
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC7 = 1;
    TRISD = 0;
    TRISDbits.TRISD1 = 0;
    ANSEL = 0;
    ANSELH = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);  
}

