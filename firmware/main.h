/* ************************************************************************** */
/** MAIN functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    main.h
 */
/* ************************************************************************** */

#define SAMPLE_RATE                     256000
#define BYTES_PER_SAMPLE                4
#define CHANNELS                        2
#define SPI_RX_BUFFER_SIZE_SAMPLES      12800                                               // full every 50 ms at 256 kS/s
#define SPI_RX_BUFFER_SIZE_BYTES        (SPI_RX_BUFFER_SIZE_SAMPLES*BYTES_PER_SAMPLE)       // 51200
#define USB_TX_BUFFER_SIZE_BYTES        (SPI_RX_BUFFER_SIZE_BYTES*CHANNELS)                 // 102400
#define USB_BYTES_PER_PACKET            512
#define USB_NUMBER_OF_PACKETS           (USB_TX_BUFFER_SIZE_BYTES/USB_BYTES_PER_PACKET)     // 200

#ifndef _MAIN_H    /* Guard against multiple inclusion */
#define _MAIN_H

#include <xc.h>
#include <proc/p32mz2048efh064.h>
#include <sys/attribs.h>
#include <sys/kmem.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "gpio.h"
#include "spi4.h"
#include "spi1.h"
#include "dma.h"
#include "usb.h"

extern uint8_t MAIN_MeasurementRunning;
extern uint8_t MAIN_SamplesRequested;
extern uint32_t MAIN_SPIDataReadyForUSBTransmissionChannel0;
extern uint32_t MAIN_SPIDataReadyForUSBTransmissionChannel1;

#endif /* _MAIN_H */

/* *****************************************************************************
 End of File
 */
