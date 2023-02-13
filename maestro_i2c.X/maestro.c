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

#define _XTAL_FREQ 8000000 //Frecuencia 8MHz
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "LCD.h"    //lIBRERIA LCD
#include "i2c.h"    //LIBRERIA I2C
#include "ds3231.h" //LIBRERIA RELOJ

////////////////////////////////////////////////////////////////////////////////
//VARIABLES
////////////////////////////////////////////////////////////////////////////////
unsigned char valor_ADC; //guarda el valor del ADC

char buffer[20];         //buffer para guardar la cadena de la LCD

int8_t selector = 0;     //selecciona el modo de configuracion

//variables para guardar los datos del sensor
int8_t dia;
int8_t mes;
int8_t ano; 
int8_t dow;
int8_t horas;
int8_t minutos;
int8_t segundos;

////////////////////////////////////////////////////////////////////////////////
//DECLARACION DE FUNCIONES
////////////////////////////////////////////////////////////////////////////////
void setup(void);
void CLK_CONFIG (void);

////////////////////////////////////////////////////////////////////////////////
//CODIGO PRINCIPAL 
////////////////////////////////////////////////////////////////////////////////
void main(void) {
    setup();                    //setup del reloj, puertos y modulos
    while(1){
        //RECEPCION DE VALORES CON I2C
        I2C_Start();              //INICIAMOS LA COMUNICACION
        I2C_Write(0x51);          //SELECCIONAMOS RECEPCION DE DATOS
        valor_ADC = I2C_Read();   //RECIVIMOS EL VALOR DEL ADC
        I2C_Stop();               //TERMINAMOS LA COMUNICACION            
        __delay_us(10);           //DELAY DE FUNCIONAMIENTO 
        
          if(PORTAbits.RA0 == 1) // verificamos que el PB de cambio de estado este presionado
        {
            while(PORTAbits.RA0 == 1);
            __delay_ms(20);      //antirebotes
            Lcd_Clear();        //limpiamos la pantalla
            selector++;         //incrementamos el selector 
        }
        
        CLK_CONFIG();
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
    
    TRISAbits.TRISA0 = 1;                             // Pin RA0 como entrada
    TRISAbits.TRISA1 = 1;                             // Pin RA1 como entrada
    TRISAbits.TRISA2 = 1;                             // Pin RA2 como entrada

    //CONFIGURACION DE SALIDAS
    TRISB = 0;
    TRISD = 0;
        
    //REINICIO DE PUERTOS
    PORTB = 0;
    PORTD = 0;
    PORTA = 0;
   
    //FRECUENCIA DE TRABAJO A 8MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    
    //RELOJ INTERNO
    OSCCONbits.SCS = 1;
    
     //inicializacion de la LCD
     Lcd_Init();                
    
    //CONFIGURACION DEL I2C PARA EL MAESTRO
    I2C_Init_Master(I2C_100KHZ);
      
}

void CLK_CONFIG (void)
{
    switch(selector)
        {
            case 0:
                DS3231_Get_Date((uint8_t)&dia, (uint8_t)&mes, (uint8_t)&ano, (uint8_t)&dow);
                DS3231_Get_Time((uint8_t)&horas, (uint8_t)&minutos, (uint8_t)&segundos);
                
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("ADC: ");
                sprintf(buffer, "%u ", valor_ADC);
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String(buffer);

                sprintf(buffer, "%02u/%02u/20%02u", dia, mes, ano);
                Lcd_Set_Cursor(1,6);
                Lcd_Write_String(buffer);
                sprintf(buffer, "%02u:%02u:%02u", horas, minutos, segundos);
                Lcd_Set_Cursor(2,8);
                Lcd_Write_String(buffer);
                __delay_ms(200);
                break;

            case 1:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Dia: %02u", dia);
                Lcd_Write_String(buffer);
                if(PORTAbits.RA1 == 1)
                {
                    while(PORTAbits.RA1 == 1);
                    __delay_ms(20);
                    dia++;
                    if(dia > 31){
                        dia = 31;
                    }
                }
                if(PORTAbits.RA2 == 1)
                {
                    while(PORTAbits.RA2 == 1);
                    __delay_ms(20);
                    dia--;
                    if(dia < 1){
                        dia = 1;
                    }
                }
                break;

            case 2:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Mes: %02u", mes);
                Lcd_Write_String(buffer);
                if(PORTAbits.RA1 == 1)
                {
                    while(PORTAbits.RA1 == 1);
                    __delay_ms(20);
                    mes++;
                    if(mes > 12){
                        mes = 12;
                    }
                }
                if(PORTAbits.RA2 == 1)
                {
                    while(PORTAbits.RA2 == 1);
                    __delay_ms(20);
                    mes--;
                    if(mes < 1){
                        mes = 1;
                    }
                }
                break;

            case 3:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "ANO: %02u", ano);
                Lcd_Write_String(buffer);
                if(PORTAbits.RA1 == 1)
                {
                    while(PORTAbits.RA1 == 1);
                    __delay_ms(20);
                    ano++;
                    if(ano > 99){
                        ano = 99;
                    }
                }
                if(PORTAbits.RA2 == 1)
                {
                    while(PORTAbits.RA2 == 1);
                    __delay_ms(20);
                    ano--;
                    if(ano == -1){
                        ano = 0;
                    }
                }
                break;

            case 4:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Dia semana: %u", dow);
                Lcd_Write_String(buffer);
                Lcd_Set_Cursor(2,1);
                sprintf(buffer, "%s    ", dw[dow]);
                Lcd_Write_String(buffer);
                if(PORTAbits.RA1 == 1)
                {
                    while(PORTAbits.RA1 == 1);
                    __delay_ms(20);
                    dow++;
                    if(dow > 6){
                        dow = 6;
                    }
                }
                if(PORTAbits.RA2 == 1)
                {
                    while(PORTAbits.RA2 == 1);
                    __delay_ms(20);
                    dow--;
                    if(dow == -1){
                        dow = 0;
                    }
                }
                break;

            case 5:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Hora: %02u", horas);
                Lcd_Write_String(buffer);
                if(PORTAbits.RA1 == 1)
                {
                    while(PORTAbits.RA1 == 1);
                    __delay_ms(20);
                    horas++;
                    if(horas > 23){
                        horas = 23;
                    }
                }
                if(PORTAbits.RA2 == 1)
                {
                    while(PORTAbits.RA2 == 1);
                    __delay_ms(20);
                    horas--;
                    if(horas == -1){
                        horas = 0;
                    }
                }
                break;

            case 6:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "minutosuto: %02u", minutos);
                Lcd_Write_String(buffer);
                if(PORTAbits.RA1 == 1)
                {
                    while(PORTAbits.RA1 == 1);
                    __delay_ms(20);
                    minutos++;
                    if(minutos > 59){
                        minutos = 59;
                    }
                }
                if(PORTAbits.RA2 == 1)
                {
                    while(PORTAbits.RA2 == 1);
                    __delay_ms(20);
                    minutos--;
                    if(minutos == -1){
                        minutos = 0;
                    }
                }
                break;

            case 7:
                Lcd_Set_Cursor(1,1);
                sprintf(buffer, "Segundo: %02u", segundos);
                Lcd_Write_String(buffer);
                if(PORTAbits.RA1 == 1)
                {
                    while(PORTAbits.RA1 == 1);
                    __delay_ms(20);
                    segundos++;
                    if(segundos > 59){
                        segundos = 59;
                    }
                }
                if(PORTAbits.RA2 == 1)
                {
                    while(PORTAbits.RA2 == 1);
                    __delay_ms(20);
                    segundos--;
                    if(segundos == -1){
                        segundos = 0;
                    }
                }
                break;

            default:
            	dia = (uint8_t)dia;
            	mes = (uint8_t)mes;
            	ano = (uint8_t)ano;
            	dow = (uint8_t)dow;
            	horas = (uint8_t)horas;
            	minutos = (uint8_t)minutos;
            	segundos = (uint8_t)segundos;
                DS3231_Set_Date_Time(dia,mes,ano,dow,horas,minutos,segundos);
                selector = 0;
                break;
        }
}