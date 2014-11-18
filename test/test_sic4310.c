/**
* @file      test_sic4310.c
* @author    Supachai Vorapojpisut
* @version   0.1
* @date      November 4, 2014
* @brief     Test code for 
*/

/* Includes -----------------------------------------------------------------------*/
#include "sic4310.h"
#include "util.h"

/* Private types ------------------------------------------------------------------*/
/* Private constants --------------------------------------------------------------*/
/* Private macro ------------------------------------------------------------------*/
/* Private variables --------------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------------*/
/* Private functions --------------------------------------------------------------*/


/**
* @brief    Check and accept value from SIC4310 module, then echo back
*/
int main()
{
    uint8_t byte;
    
    SIC4310_config();
    timebase_config();
    while(1) {
        while (SIC4310_available()) {
            byte = SIC4310_read();
            delay(100);
            SIC4310_write(byte);
        }
        delay(1000);
    }
}
