/* #include "ds3231.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 11/04/2021
* Electronica y Circuitos
*
*/

#include "ds3231.h"

void DS3231_Set_Date_Time(uint8_t dy, uint8_t mth, uint8_t yr, uint8_t dw, uint8_t hr, uint8_t mn, uint8_t sc)
{
    sc &= 0x7F;
    hr &= 0x3F;
    I2C_Start();
    I2C_Write(DS3231_ADDRESS);
    I2C_Write(0x00);
    I2C_Write(DS3231_Bin_Bcd(sc));
    I2C_Write(DS3231_Bin_Bcd(mn));
    I2C_Write(DS3231_Bin_Bcd(hr));
    I2C_Write(DS3231_Bin_Bcd(dw));
    I2C_Write(DS3231_Bin_Bcd(dy));
    I2C_Write(DS3231_Bin_Bcd(mth));
    I2C_Write(DS3231_Bin_Bcd(yr));
    I2C_Stop();
}

void DS3231_Get_Date(uint8_t *day, uint8_t *mth, uint8_t *year, uint8_t *dow)
{
    *dow = DS3231_Bcd_Bin(DS3231_Read(DS3231_DAY) & 0x7F);
    *day = DS3231_Bcd_Bin(DS3231_Read(DS3231_DATE) & 0x3F);
    *mth = DS3231_Bcd_Bin(DS3231_Read(DS3231_MONTH) & 0x1F);
    *year = DS3231_Bcd_Bin(DS3231_Read(DS3231_YEAR));
}

void DS3231_Get_Time(uint8_t *hr, uint8_t *min, uint8_t *sec)
{
    *sec = DS3231_Bcd_Bin(DS3231_Read(DS3231_SEC) & 0x7F);
    *min = DS3231_Bcd_Bin(DS3231_Read(DS3231_MIN) & 0x7F);
    *hr = DS3231_Bcd_Bin(DS3231_Read(DS3231_HOUR) & 0x3F);
}

void DS3231_Get_DayOfWeek(char* str)
{
    uint8_t lday;
    uint8_t lmonth;
    uint8_t lyr;
    uint8_t ldow;
    DS3231_Get_Date(&lday, &lmonth, &lyr, &ldow);
    sprintf(str, "%s", dw[ldow]);
}

uint8_t DS3231_Read(uint8_t reg)
{
    uint8_t data = 0;
    I2C_Start();
    I2C_Write(DS3231_ADDRESS);
    I2C_Write(reg);
    I2C_Restart();
    I2C_Write(DS3231_ADDRESS | 0x01);
    data = I2C_Read();
    I2C_Nack();
    I2C_Stop();
    return data;
}

uint8_t DS3231_Bin_Bcd(uint8_t binary_value)
{
    uint8_t temp;
    uint8_t retval;
    temp = binary_value;
    retval = 0;
    while(1)
    {
        if(temp >= 10){
            temp -= 10;
            retval += 0x10;
        }else{
            retval += temp;
            break;
        }
    }
    return(retval);
}

uint8_t DS3231_Bcd_Bin(uint8_t bcd_value)
{
    uint8_t temp;
    temp = bcd_value;
    temp >>= 1;
    temp &= 0x78;
    return(temp + (temp >> 2) + (bcd_value & 0x0f));
}