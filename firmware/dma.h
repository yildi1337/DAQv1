/* ************************************************************************** */
/** DMA functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    dma.h
 */
/* ************************************************************************** */

#ifndef _DMA_H    /* Guard against multiple inclusion */
#define _DMA_H

#include "main.h"

extern uint32_t DMA_Channel_0_ActiveBuffer;
extern uint32_t DMA_Channel_1_ActiveBuffer;

extern void DMA_Init_Channel_0(uint32_t activeBuffer);
extern void DMA_Channel_0_InterruptHandler(void);

extern void DMA_Init_Channel_1(uint32_t activeBuffer);
extern void DMA_Channel_1_InterruptHandler(void);

#endif /* _DMA_H */

/* *****************************************************************************
 End of File
 */
