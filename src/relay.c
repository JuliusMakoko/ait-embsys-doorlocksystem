/**
* @file      relay.c
* @author    group project
* @version   1.0
* @date      October 28, 2014
* @brief     Relay module.
*/

/* Includes -----------------------------------------------------------------------*/
#include "relay.h"

/* Private types ------------------------------------------------------------------*/
/* Private constants --------------------------------------------------------------*/
#define RELAX_COUNT     8

GPIO_TypeDef *IDX2PORT[RELAX_COUNT] = {GPIOC, GPIOC, GPIOC, GPIOC, GPIOF, GPIOF, GPIOF, GPIOF};
uint16_t IDX2PIN[RELAX_COUNT] = {GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7};

/* Private macro ------------------------------------------------------------------*/
/* Private variables --------------------------------------------------------------*/
static uint8_t Active_Relays = 0;

/* Private function prototypes ----------------------------------------------------*/
/* Private functions --------------------------------------------------------------*/


/**
* @brief    Configure relay channels
* @param    relays  Pattern of active relays
* @retval   None
*/
void relay_config(uint8_t relays)
{
    GPIO_InitTypeDef relay_activate_struct, relay_deactivate_struct;
    int idx;
    
    // Enable GPIOC and GPIOF for relay channels
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOF, ENABLE);
    
    relay_activate_struct.GPIO_Mode = GPIO_Mode_OUT;
    relay_activate_struct.GPIO_OType = GPIO_OType_PP;
    relay_activate_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    relay_activate_struct.GPIO_Speed = GPIO_Speed_Level_1;
    
    relay_deactivate_struct.GPIO_Mode = GPIO_Mode_IN;
    relay_deactivate_struct.GPIO_PuPd = GPIO_PuPd_DOWN;
    
    // Configure relay channel, either activate or deactivate
    for (idx = 0; idx < RELAX_COUNT; idx++) {
        if ((relays & (1<<idx)) != 0) { 
            relay_activate_struct.GPIO_Pin = IDX2PIN[idx];
            GPIO_Init(IDX2PORT[idx], &relay_activate_struct);
            GPIO_WriteBit(IDX2PORT[idx], IDX2PIN[idx], Bit_RESET);
        } else {
            relay_deactivate_struct.GPIO_Pin = IDX2PIN[idx];
            GPIO_Init(IDX2PORT[idx], &relay_deactivate_struct);
        }
    }
    
    // Store activated relay channels
    Active_Relays = relays;
}


/**
* @brief    Turn ON specified activated relays
* @param    relays  Pattern of ON/OFF
* @retval   None
*/
void relay_turn_on(uint8_t relays)
{
    int idx;
    
    for (idx = 0; idx < RELAX_COUNT; idx++) {
        if ((relays & (1<<idx) & Active_Relays) != 0) { 
            GPIO_WriteBit(IDX2PORT[idx], IDX2PIN[idx], Bit_SET);
        }
    }
}


/**
* @brief    Turn OFF specified activated relays
* @param    relays  Pattern of ON/OFF
* @retval   None
*/
void relay_turn_off(uint8_t relays)
{
    int idx;
    
    for (idx = 0; idx < RELAX_COUNT; idx++) {
        if ((relays & (1<<idx) & Active_Relays) != 0) { 
            GPIO_WriteBit(IDX2PORT[idx], IDX2PIN[idx], Bit_RESET);
        }
    }
}
