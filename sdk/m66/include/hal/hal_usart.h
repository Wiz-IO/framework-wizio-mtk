/*
* This file is part of the DZ09 project.
*
* Copyright (C) 2019 AJScorp
*
* This program is free software; you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; version 2 of the License.
*
* This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with this program; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. 
*/
#ifndef _USART_H_
#define _USART_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "hal.h"

#define USARTDEFCLOCK   52000000
#define USARTDEFOVS     16

typedef struct tag_USART
{
    union
    {
        uint8x32_t RXB;    /* DLAB=0: Receive buffer register*/
        uint8x32_t TXB;    /* DLAB=0: Transmit buffer register */
        uint8x32_t DLL;    /* DLAB=1: Divisor, LS byte latch */
    };
    union
    {
        uint8x32_t IER;    /* DLAB=0: Interrupt-enable register */
        uint8x32_t DLM;    /* DLAB=1: Divisor, MS byte latch */
    };
    union
    {
        uint8x32_t IIR;    /* DLAB=0: Interrupt status register */
        uint8x32_t FCR;    /* DLAB=0: FIFO control register */
        uint8x32_t EFR;    /* DLAB=1: Enhanced feature register */
    };
    uint8x32_t LCR;
    union
    {
        uint8x32_t MCR;
        uint8x32_t XON1;
    };
    uint8x32_t LSR;
    uint8x32_t XOFF1;
    uint8x32_t SCR;
    uint8x32_t AUTOBAUD_EN;
    uint8x32_t HIGHSPEED;
    uint8x32_t SAMPLECOUNT;
    uint8x32_t SAMPLEPOINT;
    uint8x32_t AUTOBAUD_REG;
    uint8x32_t RATEFIX_AD;
    uint8x32_t AUTOBAUDSAMPLE;
    uint8x32_t GUARD;
    uint8x32_t ESCAPE_DAT;
    uint8x32_t ESCAPE_EN;
    uint8x32_t SLEEP_EN;
    uint8x32_t DMA_EN;
    uint8x32_t RXTRI_AD;
    uint8x32_t FRACDIV_L;
    uint8x32_t FRACDIV_M;
    uint8x32_t FCR_RD;
} TUSART, *pUSART;

//IER
#define CTSI                        (1 << 7)
#define RTSI                        (1 << 6)
#define XOFFI                       (1 << 5)
#define EDSSI                       (1 << 3)
#define ELSI                        (1 << 2)  /* Error interrupt */
#define ETBEI                       (1 << 1)  /* TX empty interrupt */
#define ERBFI                       (1 << 0)  /* RX interrupt */
//IIR
#define FIFOE(v)                    (((v) & 0x03) << 6)
#define INTID(v)                    ((v) & 0x3F)
#define U_NOINT                     0x01
#define U_LSINT                     0x06
#define U_RXTO                      0x0C
#define U_RXDATA                    0x04
#define U_TXEMPTY                   0x02
#define U_MODCHANGE                 0x00
#define U_SWFLOW                    0x10
#define U_HWFLOW                    0x20
//FCR
#define RFTL1_RFTL(v)               (((v) & 0x03) << 6)
#define RXFIFO_1                    0
#define RXFIFO_6                    1
#define RXFIFO_12                   2
#define RX_TRIG                     3
#define TFTL1_TFTL(v)               (((v) & 0x03) << 4)
#define TXFIFO_1                    0
#define TXFIFO_6                    1
#define TXFIFO_12                   2
#define TXFIFO_14                   3
#define CLRT                        (1 << 2)
#define CLRR                        (1 << 1)
#define FIFOEN                      (1 << 0)
//EFR
#define AUTO_CTS                    (1 << 7)
#define AUTO_RTS                    (1 << 6)
#define ENABLE_EFR                  (1 << 4)
#define SW_FLOW_CONT(v)             (((v) & 0x0F) << 0)
#define SWFLOW_TX                   0x08
#define SWFLOW_RX                   0x02
//LCR
#define U_DLAB                      (1 << 7)
#define U_SB                        (1 << 6)
#define U_SP                        (1 << 5)
#define U_EPS                       (1<,4)
#define U_PEN                       (1 << 3)
#define U_STB                       (1 << 2)
#define U_WLS(v)                    (((v) & 0x03) << 0)
#define WLS_5                       0
#define WLS_6                       1
#define WLS_7                       2
#define WLS_8                       3
//MCR
#define XOFF_STATUS                 (1 << 7)
#define LOOP                        (1 << 4)
#define RTS                         (1 << 1)
//LSR
#define FIFOERR                     (1 << 7)
#define U_TEMT                      (1 << 6)
#define U_THRE                      (1 << 5)
#define U_BI                        (1 << 4)
#define U_FE                        (1 << 3)
#define U_PE                        (1 << 2)
#define U_OE                        (1 << 1)
#define U_DR                        (1 << 0)
//AUTOBAUD_EN
#define SLEEP_ACK_SEL               (1 << 2)
#define AUTOBAUD_SEL                (1 << 1)
#define AUTOBAUDEN                  (1 << 0)
//HIGHSPEED
#define USPEED(v)                   (((v) & 0x03) << 0)
#define USPEED_16                   0
#define USPEED_8                    1
#define USPEED_4                    2
#define USPEED_SC                   3
//AUTOBAUD_REG
#define ABAUD_STAT(v)               (((v) & 0xF0) >> 4)
#define ABAUD_DETECTING             0
#define AT_7N1                      1
#define AT_7O1                      2
#define AT_7E1                      3
#define AT_8N1                      4
#define AT_8O1                      5
#define AT_8E1                      6

#define at_7N1                      7
#define at_7E1                      8
#define at_7O1                      9
#define at_8N1                      10
#define at_8E1                      11
#define at_8O1                      12
#define ABAUD_FAILS                 13
#define ABAUD_RATE(v)               (((v) & 0x0F) >> 0)
#define ABAUD_115200                0
#define ABAUD_57600                 1
#define ABAUD_38400                 2
#define ABAUD_19200                 3
#define ABAUD_9600                  4
#define ABAUD_4800                  5
#define ABAUD_2400                  6
#define ABAUD_1200                  7
#define ABAUD_300                   8
#define ABAUD_110                   9
//RATEFIX_AD
#define FREQ_SEL_26                 (0 << 2)
#define FREQ_SEL_13                 (1 << 2)
#define AUTOBAUDRATEFIX_52          (0 << 1)
#define AUTOBAUDRATEFIX_FS          (1 << 1)
#define RATEFIX_52                  (0 << 0)
#define RATEFIX_FS                  (1 << 0)
//AUTOBAUDSAMPLE
#define AUTOBAUDSAMPLE_52           27
#define AUTOBAUDSAMPLE_26           13
#define AUTOBAUDSAMPLE_13           6
#define AUTOBAUDSAMPLE_NS           15
//GUARD
#define GUARD_EN                    (1 << 4)
#define GUARD_CNT(v)                (((v) & 0x0F) << 0)
//ESCAPE_EN
#define ESC_EN                      (1 << 0)
//SLEEP_EN
#define SLEEPEN                     (1 << 0)
//DMA_EN
#define FIFO_ISR_SEL                (1 << 3)
#define TO_CNT_AUTORST              (1 << 2)
#define TX_DMA_EN                   (1 << 1)
#define RX_DMA_EN                   (1 << 0)
//RXTRI_AD
#define RXTRIG(v)                   (((v) & 0x0F) << 0)

typedef enum tag_USART_FLAGS
{
    USART_EnFlag        = (1 << 0),
    USART_TXInt_EnFlag  = (1 << 1),
    USART_RXInt_EnFlag  = (1 << 2),
    USART_ERRInt_EnFlag = (1 << 3),
    USART_HWFlow_EnFlag = (1 << 4),
    USART_TXFIFOEnFlag  = (1 << 5),
    USART_RXFIFOEnFlag  = (1 << 6)
} TUSART_FLAGS;

typedef struct tag_USART_CONTEXT
{
    TUSART            *USART;
    TUSART_FLAGS      Flags;
    uint8_t           *USART_RX_Buffer;
    uint8_t           *USART_TX_Buffer;
    volatile uint32_t USART_RX_Index;
    volatile uint32_t USART_TX_Count;
    volatile uint32_t USART_TX_Index;

    void * user;
    void (*rx_handler)(void);
    void (*tx_handler)(void);
} TUSART_CONTEXT, *pUSART_CONTEXT;

#define USART1  ((volatile TUSART *) USART1_base)
#define USART2  ((volatile TUSART *) USART2_base)
#define USART3  ((volatile TUSART *) USART3_base)

extern TUSART_CONTEXT USARTINFO[3];

extern int32_t USART_GetPDCode(TUSART *USART);
extern bool USART_Initialize(TUSART *USART, uint32_t Speed, void (*Handler)(void), uint32_t Flags);
extern void USART_EnableInterrupts(TUSART *USART, TUSART_FLAGS Flags);
extern void USART_DisableInterrupts(TUSART *USART, TUSART_FLAGS Flags);
extern void USART_FlashRX(TUSART *USART);
extern void USART_FlashTX(TUSART *USART);
extern void USART_SendChar(TUSART *USART, char c);
extern void USART_SendString(TUSART *USART, char *Str);
extern void USART_SendBufferPoll(TUSART_CONTEXT *USARTINF);
extern void USART_SendBufferInt(TUSART_CONTEXT *USARTINF);

#ifdef __cplusplus
}
#endif
#endif /* _USART_H_ */
