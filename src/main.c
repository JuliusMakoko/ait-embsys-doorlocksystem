/**
* @file      main.c
* @author    Supachai Vorapojpisut
* @version   0.1
* @date      October 26, 2014
* @brief     Main program of floor plan light switch
*/

/* Includes -----------------------------------------------------------------------*/
#include <tsl.h>
#include "sic4310.h"
#include "relay.h"
#include "util.h"
#include <stdio.h>

/* Private types ------------------------------------------------------------------*/
/* Private constants --------------------------------------------------------------*/
/* Private macro ------------------------------------------------------------------*/
/* Private variables --------------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------------*/
/* Private functions --------------------------------------------------------------*/
uint8_t buff;
TSL_Params_T TSL_Params;

/**
* @brief    Turn ON and OFF LEDs through channel 1 and 2 to Lock and Unlock a door
*/

int main()
{
	  uint8_t byte;
    uint8_t activate_relay = RELAY_CH1 | RELAY_CH2;
	  SIC4310_config();
    timebase_config();

  	relay_config(activate_relay);
  	relay_turn_off(RELAY_CH1);			// DOOR Green LED
		relay_turn_off(RELAY_CH2);			// DOOR Red LED
		
	  
    while(1)
		{
			while (SIC4310_available())
      {
					byte = SIC4310_read();
				  // LOCK DOOR
          if (byte == 0X00) {
						relay_turn_off(RELAY_CH1); //GREEN OFF
						relay_turn_on(RELAY_CH2);  // RED ON
					}
				// UNLOCK DOOR
					else if (byte == 0X11){
							relay_turn_on(RELAY_CH1);  //GREEN ON
						  relay_turn_off(RELAY_CH2); //RED OFF
							delay(1000);
					}
        }
    }
}
