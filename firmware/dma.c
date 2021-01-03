/* ************************************************************************** */
/** DMA functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    dma.c
 */
/* ************************************************************************** */

#include "main.h"

void DMA_Init_Channel_0(void)
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
    DCH0DSA = KVA_TO_PA(&MAIN_SPI4_DMA0_RXCircularSamplesBuffer);
    
    /* Number of source bytes */
    DCH0SSIZ = 4;
    
    /* Number of destination bytes */
    DCH0DSIZ = DMA_SPI_RX_CIRCULAR_BUFFER_SIZE_BYTES;
    
    /* Number of bytes transferred per event */
    DCH0CSIZ = 4;
    
    /* Channel is continously enabled */
    DCH0CONbits.CHAEN = 1;
    
    /* Start channel cell transfer on SPI4 RX complete interrupt */
    DCH0ECONbits.CHSIRQ = _SPI4_RX_VECTOR;
    DCH0ECONbits.SIRQEN = 1;
    
    /* Enable channel destination half full interrupt */
    DCH0INTbits.CHDHIE = 1;
    
    /* Enable channel block transfer complete interrupt */
    DCH0INTbits.CHBCIE = 1;
    
    /* Enable DMA channel 0 interrupt */
    IEC4bits.DMA0IE = 1;
    
    /* Enable DMA channel 0 */
    DCH0CONbits.CHEN = 1;    
}

void DMA_Init_Channel_1(void)
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
    DCH1DSA = KVA_TO_PA(&MAIN_SPI1_DMA1_RXCircularSamplesBuffer);
    
    /* Number of source bytes */
    DCH1SSIZ = 4;
    
    /* Number of destination bytes */
    DCH1DSIZ = DMA_SPI_RX_CIRCULAR_BUFFER_SIZE_BYTES;
    
    /* Number of bytes transferred per event */
    DCH1CSIZ = 4;
    
    /* Channel is continously enabled */
    DCH1CONbits.CHAEN = 1;
    
    /* Start channel cell transfer on SPI1 RX complete interrupt */
    DCH1ECONbits.CHSIRQ = _SPI1_RX_VECTOR;
    DCH1ECONbits.SIRQEN = 1;
    
    /* Enable channel destination half full interrupt */
    DCH1INTbits.CHDHIE = 1;
    
    /* Enable channel block transfer complete interrupt */
    DCH1INTbits.CHBCIE = 1;
    
    /* Enable DMA channel 1 interrupt */
    IEC4bits.DMA1IE = 1;
    
    /* Enable DMA channel 1 */
    DCH1CONbits.CHEN = 1;    
}

void DMA_Channel_0_InterruptHandler(void)
{
    uint8_t DMAHalfTransferComplete = 0;
	uint8_t DMAFullTransferComplete = 0;
    
    if (DCH0INTbits.CHDHIF == 1)
        DMAHalfTransferComplete = 1;
    else if (DCH0INTbits.CHBCIF == 1)
        DMAFullTransferComplete = 1;
    
    if (DMAHalfTransferComplete)
	{
        /* Copy data from SPI Rx buffer (first half) to USB transmit buffer */
        uint32_t i;
        for (i = 0; i < SPI_RX_BUFFER_SIZE_SAMPLES; i++)
        {
            MAIN_USB_TXSamplesBuffer[i][0] = MAIN_SPI4_DMA0_RXCircularSamplesBuffer[0][i];
        }
        MAIN_SPIDataReadyForUSBTransmissionChannel0 = 1;
    }
    else if (DMAFullTransferComplete)
	{
        /* Copy data from SPI Rx buffer (second half) to USB transmit buffer */
        uint32_t i;
        for (i = 0; i < SPI_RX_BUFFER_SIZE_SAMPLES; i++)
        {
            MAIN_USB_TXSamplesBuffer[i][0] = MAIN_SPI4_DMA0_RXCircularSamplesBuffer[1][i];
        }
        MAIN_SPIDataReadyForUSBTransmissionChannel0 = 1;
    }
    
    /* Clear interrupt flags */
    DCH0INTCLR = 0xFF;
    IFS4CLR = _IFS4_DMA0IF_MASK;
}

void DMA_Channel_1_InterruptHandler(void)
{
    uint8_t DMAHalfTransferComplete = 0;
	uint8_t DMAFullTransferComplete = 0;
    
    if (DCH1INTbits.CHDHIF == 1)
        DMAHalfTransferComplete = 1;
    else if (DCH1INTbits.CHBCIF == 1)
        DMAFullTransferComplete = 1;
    
    if (DMAHalfTransferComplete)
	{
        /* Copy data from SPI Rx buffer (first half) to USB transmit buffer */
        uint32_t i;
        for (i = 0; i < SPI_RX_BUFFER_SIZE_SAMPLES; i++)
        {
            MAIN_USB_TXSamplesBuffer[i][1] = MAIN_SPI1_DMA1_RXCircularSamplesBuffer[0][i];
        }
        MAIN_SPIDataReadyForUSBTransmissionChannel1 = 1;
    }
    else if (DMAFullTransferComplete)
	{
        /* Copy data from SPI Rx buffer (second half) to USB transmit buffer */
        uint32_t i;
        for (i = 0; i < SPI_RX_BUFFER_SIZE_SAMPLES; i++)
        {
            MAIN_USB_TXSamplesBuffer[i][1] = MAIN_SPI1_DMA1_RXCircularSamplesBuffer[1][i];
        }
        MAIN_SPIDataReadyForUSBTransmissionChannel1 = 1;
    }    
    
    /* Clear interrupt flags */
    DCH1INTCLR = 0xFF;
    IFS4CLR = _IFS4_DMA1IF_MASK;
}

/*******************************************************************************
 End of File
*/