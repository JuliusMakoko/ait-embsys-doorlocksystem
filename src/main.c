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
* @brief    Turn ON and OFF LEDs through channel 1,2,3 and 4 to Lock and Unlock the doors
*/

int main()
{
	  uint8_t byte;
    uint8_t activate_relay = RELAY_CH1 | RELAY_CH2 |RELAY_CH3 |RELAY_CH4;
	  SIC4310_config();
    timebase_config();

  	relay_config(activate_relay);
  	relay_turn_off(RELAY_CH1);			// DOOR-1 Green LED
		relay_turn_off(RELAY_CH2);			// DOOR-1 Red LED
		relay_turn_off(RELAY_CH3);			// DOOR-2 Green LED
		relay_turn_off(RELAY_CH4);			// DOOR-2 Red LED
	  
    while(1)
		{
			while (SIC4310_available())
      {
					byte = SIC4310_read();
				  //lock door 01
          if (byte == 0X00) {
						relay_turn_off(RELAY_CH1); //GREEN OFF
						relay_turn_on(RELAY_CH2); //RED ON
			
					}
					//unlock door 01
					else if (byte == 0X01){
							relay_turn_on(RELAY_CH1); //GREEN ON
						  relay_turn_off(RELAY_CH2); //RED OFF
							delay(1000);
					}
					//lock door 02
					else if (byte == 0X10){
							relay_turn_off(RELAY_CH3); //GREEN OFF
						  relay_turn_on(RELAY_CH4); //RED ON
							delay(1000);
					}
					//unlock door 02
					else if (byte == 0X11){
							relay_turn_on(RELAY_CH3); //GREEN ON
						  relay_turn_off(RELAY_CH4); //RED OFF
							delay(1000);
					}					
        }
    }
}
