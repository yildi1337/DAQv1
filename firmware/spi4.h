/* ************************************************************************** */
/** SPI4 functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    spi4.h
 */
/* ************************************************************************** */

#ifndef _SPI4_H    /* Guard against multiple inclusion */
#define _SPI4_H

#include "main.h"

extern uint32_t __attribute__((coherent)) SPI4_RXSamplesBuffer0[SPI_RX_BUFFER_SIZE_SAMPLES];
extern uint32_t __attribute__((coherent)) SPI4_RXSamplesBuffer1[SPI_RX_BUFFER_SIZE_SAMPLES];
extern uint32_t * SPI4_FullRXSamplesBuffer;

extern void SPI4_Init(void);
extern void SPI4_RX_InterruptHandler(void);

#endif /* _SPI4_H */

/* *****************************************************************************
 End of File
 */
