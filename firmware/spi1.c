/* ************************************************************************** */
/** SPI1 functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    spi1.c
 */
/* ************************************************************************** */

#include "main.h"

void SPI1_Init(void)
{
    /* Disable SPI1 interrupts */
    IEC3bits.SPI1EIE = 0;       // Fault interrupt
    IEC3bits.SPI1RXIE = 0;      // Rx interrupt
    IEC3bits.SPI1TXIE = 0;      // Tx interrupt

    /* Stop and reset SPI */
    SPI1CON = 0;

    /* Clear receive buffer */
    uint32_t rData = SPI1BUF;
    rData = rData;

    /* Clear interrupt flags */
    IFS3bits.SPI1EIF = 0;       // Fault interrupt
    IFS3bits.SPI1RXIF = 0;      // Rx interrupt
    IFS3bits.SPI1TXIF = 0;      // Tx interrupt

    /* Clear the overflow */
    SPI1STATbits.SPIROV = 0;

    /* Configure control register */
    SPI1CONbits.FRMEN = 1;      // Framed SPI support
    SPI1CONbits.FRMSYNC = 1;    // Frame sync pulse input (slave mode))
    SPI1CONbits.FRMPOL = 1;     // Frame pulse is active-high
    SPI1CONbits.DISSDO = 1;     // SDO1 pin is not used by the module
    SPI1CONbits.MODE32 = 1;     // 32-bit mode
    SPI1CONbits.SSEN = 1;       // SS1 pin used for slave mode
    SPI1CONbits.SRXISEL = 3;    // SPI1RXIF is set when the buffer is full
    
    /* Disable receive done interrupt */
    IEC3bits.SPI1RXIE = 0;

    /* Enable SPI1 */
    SPI1CONbits.ON = 1;
}

void SPI1_RX_InterruptHandler(void)
{
    /* Clear Rx interrupt flag */
    IFS3CLR = _IFS3_SPI1RXIF_MASK;
}

/*******************************************************************************
 End of File
*/