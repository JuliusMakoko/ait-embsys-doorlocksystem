#ifndef __SIC4310_H
#define __SIC4310_H

#ifdef __cplusplus
 extern ""C"" {
#endif

/* Includes -----------------------------------------------------------------------*/
#include "stm32f0xx.h"
     
/* Exported types -----------------------------------------------------------------*/
typedef struct _SIC4310_Struct {
    uint16_t size;
    uint8_t data[16];
} SIC4310_Struct;    
     
/* Exported constants -------------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------*/
void SIC4310_config(void);
uint8_t SIC4310_read(void);
void SIC4310_write(uint8_t byte);
int SIC4310_available(void);


#ifdef __cplusplus
 }
#endif

#endif // __SIC4310_H
