/**
* @file      sic4310.c
* @author    Supachai Vorapojpisut
* @version   1.0
* @date      November 4, 2014
* @brief     SIC4310 module using USART1 in IRQ mode.
*/

/* Includes -----------------------------------------------------------------------*/
#include "sic4310.h"
#include <tsl.h>
#include "relay.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

/* Private constants --------------------------------------------------------------*/
#define SIC4310_BAUDRATE    115200
#define SIC4310_BUF_LENGTH  64

/* Private types ------------------------------------------------------------------*/
typedef struct _SIC4310_Buffer {
    uint32_t  Tx_HeadIdx;
    uint32_t  Tx_TailIdx;
    uint8_t   Tx[SIC4310_BUF_LENGTH];
    uint32_t  Rx_HeadIdx;
    uint32_t  Rx_TailIdx;
    uint8_t   Rx[SIC4310_BUF_LENGTH];	
} SIC4310_Buffer;

/* Private macro ------------------------------------------------------------------*/
#define __BUF_IS_FULL(headidx, tailidx)		((tailidx)==(((headidx)+1)%SIC4310_BUF_LENGTH))
#define __BUF_IS_EMPTY(headidx, tailidx)	((headidx)==(tailidx))
#define __BUF_RESET(bufidx)					((bufidx)=0)
#define __BUF_INCR(bufidx)					((bufidx)=((bufidx)+1)%SIC4310_BUF_LENGTH)
#define __BUF_ALLOC(headidx, tailidx)		((headidx>=tailidx)?(headidx-tailidx):(SIC4310_BUF_LENGTH-tailidx+headidx))

/* Private variables --------------------------------------------------------------*/
SIC4310_Buffer Msg_Buffer;
volatile static int UART_TxIrqState = RESET;

/* Private function prototypes ----------------------------------------------------*/
/* Private functions --------------------------------------------------------------*/

/**
* @brief    Configure USART1, pins PB6/PB7, and enable TXE/RXNE IRQ handler
* @param    None
* @retval   None
*/
void SIC4310_config()
{
    USART_InitTypeDef usart_init_struct;
    GPIO_InitTypeDef gpio_init_struct;
    NVIC_InitTypeDef nvic_init_struct;
    
    // Enable GPIOB and USART1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	
	// Configure pin PB6 for Tx, pin PB7 for Rx
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_1);
   
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
    gpio_init_struct.GPIO_OType = GPIO_OType_PP;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;      
    gpio_init_struct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &gpio_init_struct);
               
    // Configure Tx pin (D1) with AF  -> GPIOA_Pin_2
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
    gpio_init_struct.GPIO_OType = GPIO_OType_PP;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_init_struct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &gpio_init_struct);

	  // Configure USART1 for 115200,8,n,1
    usart_init_struct.USART_BaudRate = SIC4310_BAUDRATE;
    usart_init_struct.USART_WordLength = USART_WordLength_8b;
    usart_init_struct.USART_Parity = USART_Parity_No;
    usart_init_struct.USART_StopBits = USART_StopBits_1;
    usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &usart_init_struct);
    
    // Configure NVIC for USART1 IRQ
    nvic_init_struct.NVIC_IRQChannel = USART1_IRQn;
    nvic_init_struct.NVIC_IRQChannelPriority = 0;
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_struct);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    
    USART_Cmd(USART1, ENABLE);
}


/**
* @brief    Read stored byte
* @param    None
* @retval   One byte from Rx buffer
*/
uint8_t SIC4310_read()
{
    uint8_t val = -1;

	while (__BUF_IS_EMPTY(Msg_Buffer.Rx_HeadIdx, Msg_Buffer.Rx_TailIdx));
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	val = Msg_Buffer.Rx[Msg_Buffer.Rx_TailIdx];
	__BUF_INCR(Msg_Buffer.Rx_TailIdx);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	return val;
}


/**
* @brief    Write into buffer
* @param    byte    Byte to be sent
* @retval   None
*/
void SIC4310_write(uint8_t byte)
{
    if (__BUF_IS_EMPTY(Msg_Buffer.Tx_HeadIdx, Msg_Buffer.Tx_TailIdx) && (UART_TxIrqState == RESET)) {
		USART_SendData(USART1, byte);
		UART_TxIrqState = SET;
	} else {
		while (__BUF_IS_FULL(Msg_Buffer.Tx_HeadIdx, Msg_Buffer.Tx_TailIdx));
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		Msg_Buffer.Tx[Msg_Buffer.Tx_HeadIdx] = byte;
		__BUF_INCR(Msg_Buffer.Tx_HeadIdx);			
	}
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}


/**
* @brief    Return status of buffer storing UART data
* @param    None
* @retval   True if available, 0 otherwise
*/
int SIC4310_available(void)
{
    return __BUF_ALLOC(Msg_Buffer.Rx_HeadIdx, Msg_Buffer.Rx_TailIdx);
}


// IRQ handler for USART1 TXE and RXNE events
void USART1_IRQHandler()
{   
    uint8_t dummy;
    
    // See 8.2.1 TxRU command, 64-byte downlink FIFO
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        if (!__BUF_IS_FULL(Msg_Buffer.Rx_HeadIdx, Msg_Buffer.Rx_TailIdx)) {
            Msg_Buffer.Rx[Msg_Buffer.Rx_HeadIdx] = USART_ReceiveData(USART1);  
            __BUF_INCR(Msg_Buffer.Rx_HeadIdx);
        } else {
            dummy = USART_ReceiveData(USART1); // dummy read
        }
    }
    
    // See 8.2.2 RxUR command, 64-byte uplink FIFO
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
        if (!__BUF_IS_EMPTY(Msg_Buffer.Tx_HeadIdx, Msg_Buffer.Tx_TailIdx)) {
            USART_SendData(USART1, Msg_Buffer.Tx[Msg_Buffer.Tx_TailIdx]);
            __BUF_INCR(Msg_Buffer.Tx_TailIdx);
        } else {
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
            UART_TxIrqState = RESET;
        }
    }
}
