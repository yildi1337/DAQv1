/* ************************************************************************** */
/** DMA functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    dma.c
 */
/* ************************************************************************** */

#include "main.h"

uint32_t DMA_Channel_0_ActiveBuffer = 0;
uint32_t DMA_Channel_1_ActiveBuffer = 0;

void DMA_Init_Channel_0(uint32_t activeBuffer)
{
    /* Disable DMA channel 0 interrupt */
    IEC4bits.DMA0IE = 0;

    /* Clear interrupt flags */
    DCH0INTCLR = 0xFF;
    IFS4bits.DMA0IF = 0;
    
    /* Enable DMA controller */
    DMACONbits.ON = 1;
    
    /* Configure DMA channel 0 source address */
    DCH0SSA = KVA_TO_PA(&SPI4BUF);
    
    /* Configure DMA channel 0 desination address */
    if (activeBuffer == 0)
    {
        DCH0DSA = KVA_TO_PA(&SPI4_RXSamplesBuffer0);
        DMA_Channel_0_ActiveBuffer = 0;
    }
    else if (activeBuffer == 1)
    {
        DCH0DSA = KVA_TO_PA(&SPI4_RXSamplesBuffer1);
        DMA_Channel_0_ActiveBuffer = 1;
    }
    
    /* Number of source bytes */
    DCH0SSIZ = 4;
    
    /* Number of destination bytes */
    DCH0DSIZ = SPI_RX_BUFFER_SIZE_BYTES;
    
    /* Number of bytes transferred per event */
    DCH0CSIZ = 4;
    
    /* Channel is continously enabled */
    DCH0CONbits.CHAEN = 1;
    
    /* Start channel cell transfer on SPI4 RX complete interrupt */
    DCH0ECONbits.CHSIRQ = _SPI4_RX_VECTOR;
    DCH0ECONbits.SIRQEN = 1;
    
    /* Enable channel block transfer complete interrupt */
    DCH0INTbits.CHBCIE = 1;
    
    /* Enable DMA channel 0 interrupt */
    IEC4bits.DMA0IE = 1;
    
    /* Enable DMA channel 0 */
    DCH0CONbits.CHEN = 1;    
}

void DMA_Init_Channel_1(uint32_t activeBuffer)
{
    /* Disable DMA channel 1 interrupt */
    IEC4bits.DMA1IE = 0;

    /* Clear interrupt flags */
    DCH1INTCLR = 0xFF;
    IFS4bits.DMA1IF = 0;
    
    /* Enable DMA controller */
    DMACONbits.ON = 1;
    
    /* Configure DMA channel 1 source address */
    DCH1SSA = KVA_TO_PA(&SPI1BUF);
    
    /* Configure DMA channel 1 desination address */
    if (activeBuffer == 0)
    {
        DCH1DSA = KVA_TO_PA(&SPI1_RXSamplesBuffer0);
        DMA_Channel_1_ActiveBuffer = 0;
    }
    else if (activeBuffer == 1)
    {
        DCH1DSA = KVA_TO_PA(&SPI1_RXSamplesBuffer1);
        DMA_Channel_1_ActiveBuffer = 1;
    }
    
    /* Number of source bytes */
    DCH1SSIZ = 4;
    
    /* Number of destination bytes */
    DCH1DSIZ = SPI_RX_BUFFER_SIZE_BYTES;
    
    /* Number of bytes transferred per event */
    DCH1CSIZ = 4;
    
    /* Channel is continously enabled */
    DCH1CONbits.CHAEN = 1;
    
    /* Start channel cell transfer on SPI1 RX complete interrupt */
    DCH1ECONbits.CHSIRQ = _SPI1_RX_VECTOR;
    DCH1ECONbits.SIRQEN = 1;
    
    /* Enable channel block transfer complete interrupt */
    DCH1INTbits.CHBCIE = 1;
    
    /* Enable DMA channel 1 interrupt */
    IEC4bits.DMA1IE = 1;
    
    /* Enable DMA channel 1 */
    DCH1CONbits.CHEN = 1;    
}

void DMA_Channel_0_InterruptHandler(void)
{
    if (DMA_Channel_0_ActiveBuffer == 0)
    {
        SPI4_FullRXSamplesBuffer = SPI4_RXSamplesBuffer0;
        DCH0DSA = KVA_TO_PA(&SPI4_RXSamplesBuffer1);
        DMA_Channel_0_ActiveBuffer = 1;
    }
    else if (DMA_Channel_0_ActiveBuffer == 1)
    {
        SPI4_FullRXSamplesBuffer = SPI4_RXSamplesBuffer0;
        DCH0DSA = KVA_TO_PA(&SPI4_RXSamplesBuffer0);
        DMA_Channel_0_ActiveBuffer = 0;
    }
    
    MAIN_SPIDataReadyForUSBTransmissionChannel0 = 1;
    
    /* Clear interrupt flags */
    DCH0INTCLR = 0xFF;
    IFS4CLR = _IFS4_DMA0IF_MASK;
}

void DMA_Channel_1_InterruptHandler(void)
{
    if (DMA_Channel_1_ActiveBuffer == 0)
    {
        SPI1_FullRXSamplesBuffer = SPI1_RXSamplesBuffer0;
        DCH1DSA = KVA_TO_PA(&SPI1_RXSamplesBuffer1);
        DMA_Channel_1_ActiveBuffer = 1;
    }
    else if (DMA_Channel_1_ActiveBuffer == 1)
    {
        SPI1_FullRXSamplesBuffer = SPI1_RXSamplesBuffer1;
        DCH1DSA = KVA_TO_PA(&SPI1_RXSamplesBuffer0);
        DMA_Channel_1_ActiveBuffer = 0;
    }
    
    MAIN_SPIDataReadyForUSBTransmissionChannel1 = 1;
    
    /* Clear interrupt flags */
    DCH1INTCLR = 0xFF;
    IFS4CLR = _IFS4_DMA1IF_MASK;
}

/*******************************************************************************
 End of File
*/