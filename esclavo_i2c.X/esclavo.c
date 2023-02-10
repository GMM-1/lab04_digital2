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
#include "i2c.h"


#define _XTAL_FREQ 8000000 //Frecuencia 8MHz

////////////////////////////////////////////////////////////////////////////////
//VARIABLES
////////////////////////////////////////////////////////////////////////////////
unsigned char valor_ADC;
unsigned char temporal;

////////////////////////////////////////////////////////////////////////////////
//DECLARACION DE FUNCIONES
////////////////////////////////////////////////////////////////////////////////
void setup(void);

////////////////////////////////////////////////////////////////////////////////
//RUTINA DE INTERRUPCION
////////////////////////////////////////////////////////////////////////////////
void __interrupt() isr(void)
{ 
    //INTERRUPCION DEL ADC
    if (PIR1bits.ADIF == 1) //si se enciende la bandera del adc
    {
        valor_ADC = ADRESH; //asignamos el valor de adc a la variable 
        PIR1bits.ADIF = 0;  //apagamos la bandera
    }
    
    //INTERRUPCION DEL I2C
    if (PIR1bits.SSPIF == 1) //si se enciende la bandera
    {
        if(I2C_Error_Read() != 0) //rutina para verificar errores
        {
            I2C_Error_Data();
        }
        if(I2C_Write_Mode() == 1) //recibir datos
        {
            temporal = I2C_Read_Slave(); //recibimos el dato en una variable 
        }
        if(I2C_Read_Mode() == 1) //mandar datos
        {
            I2C_Write_Slave(valor_ADC); //enviamos el ADC
        }
        PIR1bits.SSPIF = 0; //apagamos la bandera
    }
}

////////////////////////////////////////////////////////////////////////////////
//CODIGO PRINCIPAL 
////////////////////////////////////////////////////////////////////////////////
void main(void) 
{
    setup();    //setup del reloj, puertos, modulos e interrupciones 
    while(1)
    {
        if (ADCON0bits.GO == 0)
        {
            ADCON0bits.GO = 1;  //bit the inicio para el ADC
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//CODIGOS AUXILIARES
////////////////////////////////////////////////////////////////////////////////
void setup(void)
{
    //PUERTO A0 COMO ANALOGICO 
    ANSELbits.ANS0 = 1;
    ANSELH = 0;
   
    //reloj a 8 MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    
    //reloj interno seleccionado
    OSCCONbits.SCS = 1;
    
    //configuracion del i2c para el esclavo 
    I2C_Init_Slave(0x50);
    
    //CONFIGURACION DEL ADC
    PIR1bits.ADIF = 0;      //flag del adc apagada
    PIE1bits.ADIE = 1;      //habilitamos el adc
    
    ADCON0bits.ADCS1 = 0;   // Fosc/8        
    ADCON0bits.ADCS0 = 0;         
    
    ADCON1bits.VCFG1 = 0;   //Ref VSS 
    ADCON1bits.VCFG0 = 0;   //Ref VDD
    
    ADCON1bits.ADFM = 0;    // hacia izquierda
    
    ADCON0bits.CHS3 = 0;    // Canal AN0
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;        
    
    ADCON0bits.ADON = 1;    // Habilitamos el ADC
    __delay_us(100);        //delay de funcionamiento
}