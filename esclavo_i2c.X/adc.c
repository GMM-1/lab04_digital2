#include "adc.h"

void ADC_Init(unsigned char p_ang)
{
    ADCON1 = 0x00;        // justificacion izquierda, referencias VSS y VDD
    ADCON0 = p_ang;     
    // Fosc/8, status completed, ADC disabled
}

unsigned int ADC_Read(unsigned char ch)
{
    if(ch > 13){
        return 0;
    }else{
        ADCON0 = 0b01000000;
        ADCON0 = (ch << 2);
        ADCON0bits.ADON = 1;
        ADCON0bits.GO_DONE = 1;
        while(ADCON0bits.GO_DONE == 1);
        return ADRESH;
    }
}
