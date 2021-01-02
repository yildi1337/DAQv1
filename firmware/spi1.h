/* ************************************************************************** */
/** SPI1 functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    spi1.h
 */
/* ************************************************************************** */

#ifndef _SPI1_H    /* Guard against multiple inclusion */
#define _SPI1_H

#include "main.h"

extern uint32_t __attribute__((coherent)) SPI1_RXSamplesBuffer0[SPI_RX_BUFFER_SIZE_SAMPLES];
extern uint32_t __attribute__((coherent)) SPI1_RXSamplesBuffer1[SPI_RX_BUFFER_SIZE_SAMPLES];
extern uint32_t * SPI1_FullRXSamplesBuffer;

extern void SPI1_Init(void);
extern void SPI1_RX_InterruptHandler(void);

#endif /* _SPI1_H */

/* *****************************************************************************
 End of File
 */
