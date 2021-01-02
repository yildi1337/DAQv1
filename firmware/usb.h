/* ************************************************************************** */
/** USB functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    usb.h
 */
/* ************************************************************************** */

#ifndef _USB_H    /* Guard against multiple inclusion */
#define _USB_H

#include "main.h"

#define USB_16bitTo8bitArrange(a)                   (a & 0xFF), ((a >> 8) & 0xFF)

#define USB_EP0_BUFFERSIZE_MULTIPLES_OF_8BYTES      8
#define USB_EP0_BUFFERSIZE_BYTES                    (USB_EP0_BUFFERSIZE_MULTIPLES_OF_8BYTES * 8)

#define USB_EP1_BUFFERSIZE_MULTIPLES_OF_8BYTES      64
#define USB_EP1_BUFFERSIZE_BYTES                    (USB_EP1_BUFFERSIZE_MULTIPLES_OF_8BYTES * 8)

#define USB_DEVICE_DESCRIPTOR_LENGTH                18
#define USB_CONFIGURATION_DESCRIPTOR_LENGTH         9
#define USB_INTERFACE_DESCRIPTOR_LENGTH             9
#define USB_ENDPOINT_DESCRIPTOR_LENGTH              7

#define USB_STRING0_SUPPORTED_LANGUAGES_LENGTH      4
#define USB_STRING1_MANUFACTURER_LENGTH             6
#define USB_STRING2_PRODUCT_LENGTH                  14
#define USB_STRING3_SERIAL_NUMBER_LENGTH            18

#define USB_CONFIGURATION_DESCRIPTOR_wTotalLength   (USB_CONFIGURATION_DESCRIPTOR_LENGTH+USB_INTERFACE_DESCRIPTOR_LENGTH+(2*USB_ENDPOINT_DESCRIPTOR_LENGTH))

#define USB_DEVICE_STATUS_LENGTH                    2
#define USB_INTERFACE_STATUS_LENGTH                 2
#define USB_ENDPOINT_STATUS_LENGTH                  2

#define USB_DIRECTION_OUT                           0
#define USB_DIRECTION_IN                            1

uint8_t USB_DeviceDescriptor[USB_DEVICE_DESCRIPTOR_LENGTH];
uint8_t USB_ConfigurationDescriptor[USB_CONFIGURATION_DESCRIPTOR_wTotalLength];

uint8_t USB_String0_SupportedLanguages[USB_STRING0_SUPPORTED_LANGUAGES_LENGTH];
uint8_t USB_String1_Manufacturer[USB_STRING1_MANUFACTURER_LENGTH];
uint8_t USB_String2_Product[USB_STRING2_PRODUCT_LENGTH];
uint8_t USB_String3_SerialNumber[USB_STRING3_SERIAL_NUMBER_LENGTH];

uint8_t USB_DeviceStatus[USB_DEVICE_STATUS_LENGTH];
uint8_t USB_InterfaceStatus[USB_INTERFACE_STATUS_LENGTH];
uint8_t USB_EndpointStatus[USB_ENDPOINT_STATUS_LENGTH];

extern void USB_Init(uint8_t reset);
extern void USB_InterruptHandler(void);
extern void USB_TreatRequestFromHost(void);
extern void USB_SetAddress(uint8_t address);
extern void USB_ConfigureEndpoint1(uint8_t direction);
extern void USB_TXEndpoint0(volatile uint8_t * pBuffer, uint32_t numberOfBytes);
extern void USB_RXEndpoint1();
extern void USB_TXEndpoint1(volatile uint8_t * pBuffer);

#endif /* _USB_H */

/* *****************************************************************************
 End of File
 */
