/**
* @file      util.c
* @author    Supachai Vorapojpisut
* @version   1.0
* @date      November 4, 2014
* @brief     Collection of utility functions.
*/

/* Includes -----------------------------------------------------------------------*/
#include "util.h"

/* Private types ------------------------------------------------------------------*/
/* Private constants --------------------------------------------------------------*/
#define TICK_PER_SEC    1000

/* Private macro ------------------------------------------------------------------*/
/* Private variables --------------------------------------------------------------*/
volatile static uint32_t Tick = 0;
    
/* Private function prototypes ----------------------------------------------------*/
/* Private functions --------------------------------------------------------------*/


// IRQ handler for SysTick
void SysTick_Handler() 
{
    Tick++;
}


/**
* @brief    Configure timebase to generate 1-ms tick
* @param    None
* @retval   None
*/
void timebase_config()
{
    SysTick_Config(SystemCoreClock/TICK_PER_SEC);
}


/**
* @brief    Delay for specified number of tick
* @param    dly_tick    Number of ticks to be delayed
* @retval   None
*/
void delay(uint16_t dly_tick)
{
    uint32_t timeout_tick = Tick + dly_tick;
    
    while (Tick < timeout_tick) {
    }
}


/**
* @brief    Return number of ticks from the booting
* @param    None
* @retval   Tick value
*/
uint32_t millis()
{
    return Tick;
}
