/**
* @file      test_relay.c
* @author    group project
* @version   0.1
* @date      November 4, 2014
* @brief     Test code for relay module
*/

/* Includes -----------------------------------------------------------------------*/
#include "relay.h"
#include "util.h"

/* Private types ------------------------------------------------------------------*/
/* Private constants --------------------------------------------------------------*/
/* Private macro ------------------------------------------------------------------*/
/* Private variables --------------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------------*/
/* Private functions --------------------------------------------------------------*/


/**
* @brief    Turn ON and OFF channel 1 and 5 every 1 second
*/
int main()
{
    uint8_t activate_relay = RELAY_CH1 | RELAY_CH5;
    
    relay_config(activate_relay);
    timebase_config();
    
    while(1) {
        relay_turn_on(RELAY_CH1 | RELAY_CH5);
        delay(1000);
        relay_turn_off(RELAY_CH1);
        delay(1000);
        relay_turn_off(RELAY_CH5);
        delay(1000);        
    }
}
