#ifndef __UTIL_H
#define __UTIL_H

#ifdef __cplusplus
 extern ""C"" {
#endif

/* Includes -----------------------------------------------------------------------*/
#include <stm32f0xx.h>
     
/* Exported types -----------------------------------------------------------------*/
/* Exported constants -------------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------*/
void timebase_config(void);
void delay(uint16_t dly_tick);
uint32_t millis(void);
    

#ifdef __cplusplus
 }
#endif

#endif // __UTIL_H
