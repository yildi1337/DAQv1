/* ************************************************************************** */
/** GPIO functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    gpio.c
 */
/* ************************************************************************** */

#include "main.h"

void GPIO_Init(void)
{
    /* LED1 (RG6) */
    ANSELGbits.ANSG6 = 0;   // Digital mode
    TRISGbits.TRISG6 = 0;   // Output pin
    LATGbits.LATG6 = 0;     // Initial latch value
    PORTGbits.RG6 = 0;      // Initial output value
    
    /* LED2 (RG7) */
    ANSELGbits.ANSG7 = 0;   // Digital mode
    TRISGbits.TRISG7 = 0;   // Output pin
    LATGbits.LATG7 = 0;     // Initial latch value
    PORTGbits.RG7 = 0;      // Initial output value
    
    /* LED3 (RG8) */
    ANSELGbits.ANSG8 = 0;   // Digital mode
    TRISGbits.TRISG8 = 0;   // Output pin
    LATGbits.LATG8 = 0;     // Initial latch value
    PORTGbits.RG8 = 0;      // Initial output value

    /* Unlock system for PPS configuration */
    SYSKEY = 0x00000000;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    CFGCONbits.IOLOCK = 0;

    /* PPS Input Remapping */
    SS4R = 0x00;    // nSS4 -> RPD9
    SDI4R = 0x03;   // SDI4 -> RPD11    
    SS1R = 0x04;    // nSS1 -> RPD4
    SDI1R = 0x07;   // SDI1 -> RPC14

    /* Lock back the system after PPS configuration */
    SYSKEY = 0x00000000;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    CFGCONbits.IOLOCK = 1;
}

/*******************************************************************************
 End of File
*/