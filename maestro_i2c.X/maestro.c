/*
********************************************************************************
Universidad del Valle de Guatemala
curso: Electronica Digital 2
Autor: Gabriel Andrade
compilador: XC8
proyecto: laboratorio 04
hardware: PIC 16F887
creado: 08/02/2023
última modificación: 
********************************************************************************
 */
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

////////////////////////////////////////////////////////////////////////////////
//LIBRERIAS
////////////////////////////////////////////////////////////////////////////////
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "LCD.h" //lIBRERIA LCD
#include "i2c.h" //LIBRERIA I2C

#define _XTAL_FREQ 8000000 //Frecuencia 8MHz

////////////////////////////////////////////////////////////////////////////////
//VARIABLES
////////////////////////////////////////////////////////////////////////////////
unsigned char valor_ADC;
char buffer[20];

////////////////////////////////////////////////////////////////////////////////
//DECLARACION DE FUNCIONES
////////////////////////////////////////////////////////////////////////////////
void setup(void);

////////////////////////////////////////////////////////////////////////////////
//CODIGO PRINCIPAL 
////////////////////////////////////////////////////////////////////////////////
void main(void) {
    setup();                    //setup del reloj, puertos y modulos
    Lcd_Init();                 //inicializacion de la LCD
    
    //MENSAJE FIJO
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("ADC:");
    
    while(1)
    {
        //RECEPCION DE VALORES CON I2C
        I2C_Start();              //INICIAMOS LA COMUNICACION
        I2C_Write(0x51);          //SELECCIONAMOS RECEPCION DE DATOS
        valor_ADC = I2C_Read();   //RECIVIMOS EL VALOR DEL ADC
        I2C_Stop();               //TERMINAMOS LA COMUNICACION            
        __delay_us(10);           //DELAY DE FUNCIONAMIENTO 
        
        //MENSAJE VARIABLE 
        Lcd_Set_Cursor(1,5);
        sprintf(buffer,"%d ", valor_ADC); //escribimos en la LCD el valor del ADC
        Lcd_Write_String(buffer);
        
    }
}
////////////////////////////////////////////////////////////////////////////////
//CODIGOS AUXILIARES
////////////////////////////////////////////////////////////////////////////////
void setup(void)
{
    //PUERTOS DIGITALES 
    ANSEL = 0;
    ANSELH = 0;
    
    //CONFIGURACION DE SALIDAS
    TRISB = 0; 
    TRISD = 0;
    
    //REINICIO DE PUERTOS
    PORTB = 0;
    PORTD = 0;
    
    //FRECUENCIA DE TRABAJO A 8MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    
    //RELOJ INTERNO
    OSCCONbits.SCS = 1;
    
    //CONFIGURACION DEL I2C PARA EL MAESTRO
    I2C_Init_Master(I2C_100KHZ);
}