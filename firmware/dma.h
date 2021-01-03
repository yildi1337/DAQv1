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

extern void DMA_Init_Channel_0(void);
extern void DMA_Channel_0_InterruptHandler(void);

extern void DMA_Init_Channel_1(void);
extern void DMA_Channel_1_InterruptHandler(void);

#endif /* _DMA_H */

/* *****************************************************************************
 End of File
 */
