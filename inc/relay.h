#ifndef __RELAY_H
#define __RELAY_H

#ifdef __cplusplus
 extern ""C"" {
#endif

/* Includes -----------------------------------------------------------------------*/
#include <stm32f0xx.h>
     
/* Exported types -----------------------------------------------------------------*/
/* Exported constants -------------------------------------------------------------*/
#define RELAY_CH1   (1 << 0)
#define RELAY_CH2   (1 << 1)
#define RELAY_CH3   (1 << 2)
#define RELAY_CH4   (1 << 3)
#define RELAY_CH5   (1 << 4)
#define RELAY_CH6   (1 << 5)
#define RELAY_CH7   (1 << 6)
#define RELAY_CH8   (1 << 7)
          
/* Exported macro -----------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------*/
void relay_config(uint8_t relays);
void relay_turn_on(uint8_t relays);
void relay_turn_off(uint8_t relays);     
     
     
#ifdef __cplusplus
 }
#endif

#endif // __RELAY_H
