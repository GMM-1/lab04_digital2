#include <xc.h>
#define _XTAL_FREQ 4000000
//Fosc/8 canal variable y status completed y ADC disabled
//ADCON 0 combinaciones
#define AN0          0b01000000
#define AN1          0b01000100
#define AN2          0b01001000
#define AN3          0b01001100
#define AN4          0b01010000
#define AN5          0b01010100
#define AN6          0b01011000
#define AN7          0b01011100
#define AN8          0b01100000
#define AN9          0b01100100
#define AN10         0b01101000
#define AN11         0b01101100
#define AN12         0b01110000
#define AN13         0b01110100

void ADC_Init(unsigned char p_ang);
unsigned int ADC_Read(unsigned char ch);
