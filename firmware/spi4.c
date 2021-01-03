/* ************************************************************************** */
/** SPI4 functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    spi4.c
 */
/* ************************************************************************** */

#include "main.h"

void SPI4_Init(void)
{
    /* Disable SPI4 interrupts */
    IEC5bits.SPI4EIE = 0;       // Fault interrupt
    IEC5bits.SPI4RXIE = 0;      // Rx interrupt
    IEC5bits.SPI4TXIE = 0;      // Tx interrupt

    /* Stop and reset SPI */
    SPI4CON = 0;

    /* Clear receive buffer */
    uint32_t rData = SPI4BUF;
    rData = rData;

    /* Clear interrupt flags */
    IFS5bits.SPI4EIF = 0;       // Fault interrupt
    IFS5bits.SPI4RXIF = 0;      // Rx interrupt
    IFS5bits.SPI4TXIF = 0;      // Tx interrupt

    /* Clear the overflow */
    SPI4STATbits.SPIROV = 0;

    /* Configure control register */
    SPI4CONbits.FRMEN = 1;      // Framed SPI support
    SPI4CONbits.FRMSYNC = 1;    // Frame sync pulse input (slave mode))
    SPI4CONbits.FRMPOL = 1;     // Frame pulse is active-high
    SPI4CONbits.DISSDO = 1;     // SDO4 pin is not used by the module
    SPI4CONbits.MODE32 = 1;     // 32-bit mode
    SPI4CONbits.SSEN = 1;       // SS4 pin used for slave mode
    SPI4CONbits.SRXISEL = 3;    // SPI4RXIF is set when the buffer is full
    
    /* Disable receive done interrupt */
    IEC5bits.SPI4RXIE = 0;

    /* Enable SPI4 */
    SPI4CONbits.ON = 1;
}

void SPI4_RX_InterruptHandler(void)
{
    /* Clear Rx interrupt flag */
    IFS5CLR = _IFS5_SPI4RXIF_MASK;
}

/*******************************************************************************
 End of File
*/