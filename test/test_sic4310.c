/**
* @file      test_sic4310.c
* @author    Supachai Vorapojpisut
* @version   0.1
* @date      November 4, 2014
* @brief     Test code for 
*/

/* Includes -----------------------------------------------------------------------*/
#include "sic4310.h"
#include "relay.h"
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
    uint8_t activate_relay = RELAY_CH1 | RELAY_CH5;
    
    relay_config(activate_relay);
    timebase_config();
    SIC4310_config();
    timebase_config();
    while(1) {
        while (SIC4310_available()) {
            byte = SIC4310_read();
						if (byte == 00){
							relay_turn_off(RELAY_CH1);
							delay(1000);
								
						}
						else if (byte == 01){
							relay_turn_on(RELAY_CH1);
							delay(1000);
						
						}
											
            delay(100);
            SIC4310_write(byte);
        }
        delay(1000);
    }
}
