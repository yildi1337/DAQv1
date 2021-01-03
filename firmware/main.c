/* ************************************************************************** */
/** MAIN program

  @Author
    Phillip Durdaut (2020)

  @File Name
    main.c
 */
/* ************************************************************************** */

#include "main.h"

/*** DEVCFG0 ***/
#pragma config DEBUG =      ON
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_LEVEL_3
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_LEVEL_3
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/
#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       OFF
#pragma config POSCMOD =    EC
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF

/*** DEVCFG2 ***/
#pragma config FPLLIDIV =   DIV_3
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config UPLLFSEL =   FREQ_24MHZ

/*** DEVCFG3 ***/
#pragma config USERID =     0xFFFF
#pragma config FMIIEN =     ON
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   ON

/*** BF1SEQ0 ***/
#pragma config TSEQ =       0xFFFF
#pragma config CSEQ =       0x0

uint32_t __attribute__((coherent)) MAIN_SPI4_DMA0_RXCircularSamplesBuffer[2][SPI_RX_BUFFER_SIZE_SAMPLES] = { 0 };
uint32_t __attribute__((coherent)) MAIN_SPI1_DMA1_RXCircularSamplesBuffer[2][SPI_RX_BUFFER_SIZE_SAMPLES] = { 0 };
uint32_t __attribute__((coherent)) MAIN_USB_TXSamplesBuffer[SPI_RX_BUFFER_SIZE_SAMPLES][CHANNELS] = { 0 };

uint8_t MAIN_MeasurementRunning = 0;
uint8_t MAIN_SamplesRequested = 0;

uint32_t MAIN_SPIDataReadyForUSBTransmissionChannel0 = 0;
uint32_t MAIN_SPIDataReadyForUSBTransmissionChannel1 = 0;

void __ISR(_SPI4_RX_VECTOR) _SPI4_RX_InterruptHandler(void)
{
    SPI4_RX_InterruptHandler();
}

void __ISR(_SPI1_RX_VECTOR) _SPI1_RX_InterruptHandler(void)
{
    SPI1_RX_InterruptHandler();
}

void __ISR(_DMA0_VECTOR, IPL3SRS) _DMA_Channel_0_InterruptHandler(void)
{
    DMA_Channel_0_InterruptHandler();
}

void __ISR(_DMA1_VECTOR, IPL3SRS) _DMA_Channel_1_InterruptHandler(void)
{
    DMA_Channel_1_InterruptHandler();
}

void __ISR(_USB_VECTOR, ipl7AUTO) _USB_InterruptHandler(void)
{
    USB_InterruptHandler();
}

int main(void)
{
    /* Disable global interrupts */
    __builtin_disable_interrupts();
    
    /* Initialize GPIO */
    GPIO_Init();
    
    /* Initialize SPI interfaces */
    SPI4_Init();
    SPI1_Init();
    
    /* Initialize DMA controllers */
    DMA_Init_Channel_0();
    DMA_Init_Channel_1();
    
    /* Initialize USB */
    USB_Init(0);
    
    /* Configure interrupt priorities */
    INTCONSET = _INTCON_MVEC_MASK;    
    
    IPC41bits.SPI4RXIP = 0; // set SPI4 RX priority
    IPC41bits.SPI4RXIS = 0; // set SPI4 RX sub-priority  
    IPC27bits.SPI1RXIP = 0; // set SPI1 RX priority
    IPC27bits.SPI1RXIS = 0; // set SPI1 RX sub-priority
    
    IPC33bits.DMA0IP = 3; // set DMA channel 0 priority to 3
    IPC33bits.DMA0IS = 1; // set DMA channel 0 sub-priority to 1
    IPC33bits.DMA1IP = 3; // set DMA channel 1 priority to 3
    IPC33bits.DMA1IS = 1; // set DMA channel 1 sub-priority to 1
    
    IPC33bits.USBIP = 1; // set USB priority to 7
    IPC33bits.USBIS = 1; // set USB sub-Priority to 1
    
    /* Enable global interrupts */
    __builtin_enable_interrupts();
    
    /* Endless loop */    
    while(1)
    {
        /* Check if buffers are ready for transmission */
        if (MAIN_SPIDataReadyForUSBTransmissionChannel0 && MAIN_SPIDataReadyForUSBTransmissionChannel1)
        {
            if (MAIN_MeasurementRunning && MAIN_SamplesRequested)
            {            
                /* Transmit data via USB */
                USB_TXEndpoint1((uint8_t *)(&(MAIN_USB_TXSamplesBuffer[0][0])));

                /* Reset flag */
                MAIN_SamplesRequested = 0;
            }                

            /* Reset flags */
            MAIN_SPIDataReadyForUSBTransmissionChannel0 = 0;
            MAIN_SPIDataReadyForUSBTransmissionChannel1 = 0;
        }
    }

    /* Execution should not come here during normal operation */
    return (EXIT_FAILURE);
}

/*******************************************************************************
 End of File
*/