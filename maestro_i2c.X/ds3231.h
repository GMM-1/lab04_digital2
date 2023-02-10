/* #include "ds3231.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 11/04/2021
* Electronica y Circuitos
*
*/

#include <xc.h>
#define _XTAL_FREQ 8000000

#include <stdint.h>
#include <stdio.h>
#include "i2c.h"

#define DS3231_ADDRESS  0xD0

#define DS3231_SEC      0x00
#define DS3231_MIN      0x01
#define DS3231_HOUR     0x02
#define DS3231_DAY      0x03
#define DS3231_DATE     0x04
#define DS3231_MONTH    0x05
#define DS3231_YEAR     0x06

extern char dw[7][11] = {"Domingo\0","Lunes\0","Martes\0","Miercoles\0","Jueves\0","Viernes\0","Sabado\0"};

void DS3231_Set_Date_Time(uint8_t dy, uint8_t mth, uint8_t yr, uint8_t dw, uint8_t hr, uint8_t mn, uint8_t sc);
void DS3231_Get_Date(uint8_t *day, uint8_t *mth, uint8_t *year, uint8_t *dow);
void DS3231_Get_Time(uint8_t *hr, uint8_t *min, uint8_t *sec);
void DS3231_Get_DayOfWeek(char* str);
uint8_t DS3231_Read(uint8_t reg);
uint8_t DS3231_Bin_Bcd(uint8_t binary_value);
uint8_t DS3231_Bcd_Bin(uint8_t bcd_value);
