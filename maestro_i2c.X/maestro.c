/*
 ********************************************************************************
Universidad del Valle de Guatemala
 * curso: 
Autor: Gabriel Andrade
compilador: XC8
proyecto: 
hardware: PIC 16F887
creado: 
última modificación: 
 ********************************************************************************
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 4000000
#include <xc.h>
#include "i2c.h"						// Libreria del protocolo i2c

unsigned char dato_tx;                      // Almacena el dato a enviar
unsigned char dato_rx;                      // Almacena el dato recibido del esclavo

////////////////////////////////////////////////////////////////////////////////
// PROTOTIPOS DE FUNCIONES
////////////////////////////////////////////////////////////////////////////////
void setupINTOSC(void);

////////////////////////////////////////////////////////////////////////////////
// CODIGO PRINCIPAL
////////////////////////////////////////////////////////////////////////////////

void main(void) {
    setupINTOSC();
    while (1) 
    {
        //transmicion de datos 
        dato_tx = PORTA & 0x0F;
        I2C_Start();
        I2C_Write(0xA0);
        I2C_Write(dato_tx);
        I2C_Stop();
        __delay_us(10);
        
        //recepcion de datos 
        I2C_Start();
        I2C_Write(0xA0 | 1);
        dato_rx = I2C_Read();
        PORTD = dato_rx;
        I2C_Stop();
        __delay_us(10);
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////
// FUNCIONES
////////////////////////////////////////////////////////////////////////////////

void setupINTOSC(void) {
    //Seleccion de Oscilador interno
    OSCCONbits.SCS = 1;

    // oscilador a 4MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    
    //Pines digitales 
    ANSEL = 0;
    ANSELH = 0;
    
    //Entradas/salidas
    TRISA = 0xFF;                           // Puerto A como entrada
    TRISDbits.TRISD0 = 0;                      // Pin RD0 como salida
    TRISDbits.TRISD1 = 0;                      // Pin RD1 como salida
    TRISDbits.TRISD2 = 0;                      // Pin RD2 como salida
    TRISDbits.TRISD3 = 0;                      // Pin RD3 como salida
    PORTD = 0x00;                            // Limpia el puerto D
    
    //configuracion del i2c
    I2C_Init_Master(I2C_100KHZ);            // Inicializa el protocolo I2C en modo maestro
}
