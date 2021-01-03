/* ************************************************************************** */
/** USB functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    usb.c
 */
/* ************************************************************************** */

#include "main.h"

uint8_t USB_DeviceDescriptor[] = {
/* bLength                                  */ USB_DEVICE_DESCRIPTOR_LENGTH,
/* bDescriptorType                          */ 0x01,
/* bcdUSB                                   */ USB_16bitTo8bitArrange(0x0200),      // USB 2.0
/* bDeviceClass								*/ 0xff,                                // vendor specific
/* bDeviceSubClass							*/ 0xff,
/* bDeviceProtocol							*/ 0xff,
/* bMaxPacketSize0                          */ 64,          
/* idVendor									*/ USB_16bitTo8bitArrange(0x1337),
/* idProduct								*/ USB_16bitTo8bitArrange(0x2020),
/* bcdDevice								*/ USB_16bitTo8bitArrange(0x0100),      // (firmware revision 1.0)
/* iManufacturer							*/ 1,                                   // USB_String1_Manufacturer
/* iProduct									*/ 2,                                   // USB_String2_Product
/* iSerialNumber							*/ 3,                                   // USB_String3_SerialNumber
/* bNumConfigurations						*/ 1
};

uint8_t USB_ConfigurationDescriptor[] = {
/* bLength                                  */ USB_CONFIGURATION_DESCRIPTOR_LENGTH,
/* bDescriptorType                          */ 0x02,
/* wTotalLength                             */ USB_16bitTo8bitArrange(USB_CONFIGURATION_DESCRIPTOR_wTotalLength),
/* bNumInterfaces							*/ 1,
/* bConfigurationValue						*/ 1,
/* iConfiguration							*/ 0,                                   // no string
/* bmAttributes                             */ 0x80 | 0x00,                         // not self-powered
/* bMaxPower								*/ 0xFA,                                // 500 mA
	
/* bLength                                  */ USB_INTERFACE_DESCRIPTOR_LENGTH,
/* bDescriptorType                          */ 0x04,
/* bInterfaceNumber                         */ 0,
/* bAlternateSetting						*/ 0,
/* bNumEndpoints							*/ 2,
/* bInterfaceClass							*/ 0xff,        // vendor specific
/* bInterfaceSubClass						*/ 0xff,
/* bInterfaceProtocol						*/ 0xff,
/* iInterface								*/ 0,           // no string

/* bLength                                  */ USB_ENDPOINT_DESCRIPTOR_LENGTH,
/* bDescriptorType                          */ 0x05,
/* bEndpointAddress                         */ 0x01,        // endoint 1 out (host to device)
/* bmAttributes                             */ 0x02,        // bulk
/* wMaxPacketSize							*/ USB_16bitTo8bitArrange(512U),
/* bInterval								*/ 0,

/* bLength                                  */ USB_ENDPOINT_DESCRIPTOR_LENGTH,
/* bDescriptorType                          */ 0x05,
/* bEndpointAddress                         */ 0x81,        // endoint 1 in (device to host)
/* bmAttributes                             */ 0x02,        // bulk
/* wMaxPacketSize							*/ USB_16bitTo8bitArrange(512U),
/* bInterval								*/ 0
};

uint8_t USB_String0_SupportedLanguages[] = {	
/* bLength                                  */ USB_STRING0_SUPPORTED_LANGUAGES_LENGTH,
/* bDescriptorType                          */ 0x03,
/* wLANGID[0]                               */ USB_16bitTo8bitArrange(0x0409)   // English - United States
};

uint8_t USB_String1_Manufacturer[] = {	
/* bLength                                  */ USB_STRING1_MANUFACTURER_LENGTH,
/* bDescriptorType                          */ 0x03,
/* bString                                  */ 'p',0,
                                               'd',0
};	

uint8_t USB_String2_Product[] = {
/* bLength                                  */ USB_STRING2_PRODUCT_LENGTH,
/* bDescriptorType                          */ 0x03,
/* bString                                  */ 'D',0,
                                               'A',0,
                                               'Q',0,
                                               ' ',0,
                                               'v',0,
                                               '1',0
};

uint8_t USB_String3_SerialNumber[] = {
/* bLength                                  */ USB_STRING3_SERIAL_NUMBER_LENGTH,
/* bDescriptorType                          */ 0x03,
/* bString                                  */ '0',0,
                                               '0',0,
                                               '0',0,
                                               '0',0
};

uint8_t USB_DeviceStatus[] = { 0x0, 0x0 };
uint8_t USB_InterfaceStatus[] = { 0x0, 0x0 };
uint8_t USB_EndpointStatus[] = { 0x0, 0x0 };

volatile uint8_t USB_Address;
volatile uint8_t USB_SetNewAddress;

volatile uint8_t USB_Ep0_RXBuffer[USB_EP0_BUFFERSIZE_BYTES];
volatile uint8_t USB_Ep1_RXBuffer[USB_EP1_BUFFERSIZE_BYTES];
volatile uint32_t USB_Ep0_NumberOfReceivedBytes;
volatile uint32_t USB_Ep1_NumberOfReceivedBytes;

volatile uint8_t USB_SetupPacket_bmRequestType;
volatile uint8_t USB_SetupPacket_bmRequestType_Recipient;
volatile uint8_t USB_SetupPacket_bmRequestType_Type;
volatile uint8_t USB_SetupPacket_bmRequestType_Direction;
volatile uint8_t USB_SetupPacket_bRequest;
volatile uint16_t USB_SetupPacket_wValue;
volatile uint16_t USB_SetupPacket_wIndex;
volatile uint16_t USB_SetupPacket_wLength;

void USB_Init(uint8_t reset)
{
    /* Disable USB D+/D- lines */
    if (reset == 0)
        USBCSR0bits.SOFTCONN = 0;

    /* Enable USB Hi-Speed mode */
    USBCSR0bits.HSEN = 1;

    /* Disable all endpoint 0-7 interrupts */
    USBCSR1bits.EP0IE = 0;
    USBCSR1bits.EP1TXIE = 0;
    USBCSR1bits.EP2TXIE = 0;
    USBCSR1bits.EP3TXIE = 0;
    USBCSR1bits.EP4TXIE = 0;
    USBCSR1bits.EP5TXIE = 0;
    USBCSR1bits.EP6TXIE = 0;
    USBCSR1bits.EP7TXIE = 0;
    USBCSR2bits.EP1RXIE = 0;
    USBCSR2bits.EP2RXIE = 0;
    USBCSR2bits.EP3RXIE = 0;
    USBCSR2bits.EP4RXIE = 0;
    USBCSR2bits.EP5RXIE = 0;
    USBCSR2bits.EP6RXIE = 0;
    USBCSR2bits.EP7RXIE = 0;
    
    /* Disable start of frame interrupt */
    USBCSR2bits.SOFIE = 0;    
    
    /* Disable reset/babble interrupt */
    USBCSR2bits.RESETIE = 0;
    
    /* Disable USB interrupts */
    IEC4bits.USBIE = 1;
    
    /* Disable USB general interrupts */
    USBCRCONbits.USBIE = 1;
    
    /* Clear USB interrupt flag */
    IFS4bits.USBIF = 0;
    
     /* Clear endpoint 0 interrupt flag */
    USBCSR0bits.EP0IF = 0;
    
    /* Clear endpoint 1-7 Tx interrupt flags */
    USBCSR0bits.EP1TXIF = 0;
    USBCSR0bits.EP2TXIF = 0;
    USBCSR0bits.EP3TXIF = 0;
    USBCSR0bits.EP4TXIF = 0;
    USBCSR0bits.EP5TXIF = 0;
    USBCSR0bits.EP6TXIF = 0;
    USBCSR0bits.EP7TXIF = 0;
    
    /* Clear endpoint 1-7 Rx interrupt flags */
    USBCSR1bits.EP1RXIF = 0;
    USBCSR1bits.EP2RXIF = 0;
    USBCSR1bits.EP3RXIF = 0;
    USBCSR1bits.EP4RXIF = 0;
    USBCSR1bits.EP5RXIF = 0;
    USBCSR1bits.EP6RXIF = 0;
    USBCSR1bits.EP7RXIF = 0;
    
    /* Clear start of frame interrupt flag */
    USBCSR2bits.SOFIF = 0;
    
    /* Clear USB address (will be set by the host later) */
    if (reset == 0)
        USB_SetAddress(0);
    
    /* Configure endpoint 0 */
    USBE0CSR0bits.TXMAXP = USB_EP0_BUFFERSIZE_BYTES;      // buffer size
    USBE0CSR2bits.SPEED = 1;                              // Hi-Speed
    
    /* Configure endpoint 1 as out, i.e. Rx, for now (Rx: host to device) */
    USB_ConfigureEndpoint1(USB_DIRECTION_OUT);
    
    /* Configure dynamic FIFO addresses */
    USBE0CSR3bits.DYNFIFOS = 1;
    
    /* Enable endpoint 0 interrupts */
    USBCSR1bits.EP0IE = 1;
    
    /* Enable USB reset interrupt */
    USBCSR2bits.RESETIE = 1;
    
    /* Enable USB general interrupts */
    USBCRCONbits.USBIE = 1;
    
    /* Enable USB interrupts */
    IEC4bits.USBIE = 1;
    
    /* Enable USB D+/D- lines */
    USBCSR0bits.SOFTCONN = 1;
}

void USB_InterruptHandler(void)
{
    /* Reset interrupt */
    if (USBCSR2bits.RESETIF)
    {
        /* Reset signaling detected */
        USB_Init(1);        
    }
    
    /* Endpoint 0 interrupt */
    if (USBCSR0bits.EP0IF)
    {
        /* Set new USB address if requested */
        if (USB_SetNewAddress)
        {
            USB_SetAddress(USB_Address);
            USB_SetNewAddress = 0;
        }
        
        /* Check for received data */
        if (USBE0CSR0bits.RXRDY)
        {
            USB_Ep0_NumberOfReceivedBytes = USBE0CSR2bits.RXCNT;
            volatile uint8_t * pUSBFIFO = (uint8_t *)&USBFIFO0;
            uint32_t i;
            
            /* Read from USB FIFO */
            for (i = 0; i < USB_Ep0_NumberOfReceivedBytes; i++)
                USB_Ep0_RXBuffer[i] = *(pUSBFIFO + (i & 0x03));
            
            /* Serviced Rx */
            USBE0CSR0bits.RXRDYC = 1;
            
            /* Decompose the received data into the standardized setup packet structure */
            USB_SetupPacket_bmRequestType = USB_Ep0_RXBuffer[0];            
            USB_SetupPacket_bmRequestType_Recipient = (USB_SetupPacket_bmRequestType & 0x1F) >> 0;
            USB_SetupPacket_bmRequestType_Type = (USB_SetupPacket_bmRequestType & 0x60) >> 5;
            USB_SetupPacket_bmRequestType_Direction = (USB_SetupPacket_bmRequestType & 0x80) >> 7;            
            USB_SetupPacket_bRequest = USB_Ep0_RXBuffer[1];
            USB_SetupPacket_wValue = (uint16_t)((USB_Ep0_RXBuffer[3] << 8) | USB_Ep0_RXBuffer[2]);
            USB_SetupPacket_wIndex = (uint16_t)((USB_Ep0_RXBuffer[5] << 8) | USB_Ep0_RXBuffer[4]);
            USB_SetupPacket_wLength = (uint16_t)((USB_Ep0_RXBuffer[7] << 8) | USB_Ep0_RXBuffer[6]);
            
            /* Treat the received request */
            USB_TreatRequestFromHost();
            
            /* Necessary? */
            if (USB_SetupPacket_wLength == 0)
                USBE0CSR0bits.DATAEND = 1;
        }
        
        /* Setup finished */
        if (USBE0CSR0bits.SETEND)
            USBE0CSR0bits.SETENDC = 1;
    
        /* Clear endpoint 0 interrupt */
        USBCSR0bits.EP0IF = 0;
    }
    
    /* Endpoint 1 interrupt */
    if (USBCSR1bits.EP1RXIF)
    {
        USB_RXEndpoint1();
        
        if (USB_Ep1_RXBuffer[0] == 'S' && USB_Ep1_RXBuffer[1] == 'T' && USB_Ep1_RXBuffer[2] == 'A' && USB_Ep1_RXBuffer[3] == 'R' && USB_Ep1_RXBuffer[4] == 'T' && USB_Ep1_RXBuffer[5] == ';')
        {
            GPIO_LED3_Set();
            MAIN_MeasurementRunning = 1;
        }
        
        if (USB_Ep1_RXBuffer[0] == 'S' && USB_Ep1_RXBuffer[1] == 'T' && USB_Ep1_RXBuffer[2] == 'O' && USB_Ep1_RXBuffer[3] == 'P' && USB_Ep1_RXBuffer[4] == ';')
        {
            GPIO_LED3_Clear();
            MAIN_MeasurementRunning = 0;
        }
        
        if (USB_Ep1_RXBuffer[0] == 'G' && USB_Ep1_RXBuffer[1] == 'E' && USB_Ep1_RXBuffer[2] == 'T' && USB_Ep1_RXBuffer[3] == ';')
        {
            MAIN_SamplesRequested = 1;
        }
        
        /* Clear endpoint 1 Rx interrupt */
        USBCSR1bits.EP1RXIF = 0;
    }
       
    /* Clear USB interrupt flag */
    IFS4bits.USBIF = 0;
}

void USB_TreatRequestFromHost(void)
{
    switch ((USB_SetupPacket_bmRequestType << 8) | USB_SetupPacket_bRequest)
    {
        /* ------------------------------------------------------------------ */
        /* --------------- Device requests ---------------------------------- */
        /* ------------------------------------------------------------------ */
        case 0x8000:    // GET_STATUS
            USB_TXEndpoint0(USB_DeviceStatus, sizeof(USB_DeviceStatus));
            break;
        
        case 0x0001:    // CLEAR_FEATURE
            break;
        
        case 0x0003:    // SET_FEATURE
            break;
        
        case 0x0005:    // SET_ADDRESS
            USB_Address = USB_SetupPacket_wValue & 0x00FF;
            USB_SetNewAddress = 1;            
            break;
            
        case 0x8006:    // GET_DESCRIPTOR            
            switch ((USB_SetupPacket_wValue & 0xFF00) >> 8)
            {
                case 1:     // Device
                    USB_TXEndpoint0(USB_DeviceDescriptor, sizeof(USB_DeviceDescriptor));
                    break;
                    
                case 2:     // Configuration
                    if (USB_SetupPacket_wLength > 0x00 && USB_SetupPacket_wLength < 0xff)
                        USB_TXEndpoint0(USB_ConfigurationDescriptor, USB_SetupPacket_wLength);
                    else
                        USB_TXEndpoint0(USB_ConfigurationDescriptor, sizeof(USB_ConfigurationDescriptor));
                    break;
                    
                case 3:     // String
                    switch (USB_SetupPacket_wValue & 0x00FF)
                    {  
                        case 0:     // Supported languages
                            USB_TXEndpoint0(USB_String0_SupportedLanguages, sizeof(USB_String0_SupportedLanguages));
                            break;
                            
                        case 1:     // Manufacturer
                            USB_TXEndpoint0(USB_String1_Manufacturer, sizeof(USB_String1_Manufacturer));
                            break;
                            
                        case 2:     // Product
                            USB_TXEndpoint0(USB_String2_Product, sizeof(USB_String2_Product));
                            break;
                            
                        case 3:     // Serial number
                            USB_TXEndpoint0(USB_String3_SerialNumber, sizeof(USB_String3_SerialNumber));
                            break;
                    }
                    break;
                    
                case 4:     // Interface
                    break;
                    
                case 5:     // Endpoint
                    break;
                    
                case 6:     // Device Qualifier
                    break;
                    
                case 7:     // Other Speed Configuration
                    break;
                    
                case 8:     // Interface Power (obsolete)
                    break;
                    
                case 9:     // On-The-Go (OTG)
                    break;
                
                /* Send status handshake */
                default:
                    USBE0CSR0bits.STALL = 1;
                    break;
            }            
            break;
        
        case 0x0007:    // SET_DESCRIPTOR
            break;
        
        case 0x8008:    // GET_CONFIGURATION
            break;
        
        case 0x0009:    // SET_CONFIGURATION
            break;
           
        /* ------------------------------------------------------------------ */
        /* --------------- Interface requests ------------------------------- */
        /* ------------------------------------------------------------------ */
        case 0x8100:    // GET_STATUS
            USB_TXEndpoint0(USB_InterfaceStatus, sizeof(USB_InterfaceStatus));
            break;
        
        case 0x0101:    // CLEAR_FEATURE
            break;
        
        case 0x0103:    // SET_FEATURE
            break;
        
        case 0x810A:    // GET_INTERFACE
            break;
        
        case 0x0111:    // SET_INTERFACE
            break;

        /* ------------------------------------------------------------------ */
        /* --------------- Endpoint requests -------------------------------- */
        /* ------------------------------------------------------------------ */
        case 0x8200:    // GET_STATUS
            USB_TXEndpoint0(USB_EndpointStatus, sizeof(USB_EndpointStatus));
            break;
        
        case 0x0201:    // CLEAR_FEATURE
            break;
        
        case 0x0203:    // SET_FEATURE
            break;
        
        case 0x8212:    // SYNCH_FRAME
            break;
            
        /* Send status handshake */
        default:
            USBE0CSR0bits.STALL = 1;
            break;
    }
}

void USB_SetAddress(volatile uint8_t address)
{    
    USB_Address = address & 0x7F;
    USBCSR0bits.FUNC = USB_Address;
}

void USB_ConfigureEndpoint1(uint8_t direction)
{
    /* Out, i.e. Rx (host to device) */    
    if (direction == USB_DIRECTION_OUT)
    {
        USBE1CSR0bits.MODE = 0;                                             // Out, i.e. Rx
        USBE1CSR1bits.RXMAXP = USB_EP1_BUFFERSIZE_BYTES;                    // Maximum Rx payload (512 bytes is the maximum)
        USBE1CSR3bits.PROTOCOL = 2;                                         // Bulk mode
        USBE1CSR1bits.PIDERR = 0;
        USBE1CSR3bits.SPEED = 1;                                            // Hi-Speed
        
        /* Enable endpoint 1 Rx interrupt */
        USBCSR2bits.EP1RXIE = 1;
    }
    
    /* In, i.e. Tx (device to host) */    
    if (direction == USB_DIRECTION_IN)
    {
        USBE1CSR0bits.MODE = 1;                                             // In, i.e. Tx
        USBE1CSR0bits.TXMAXP = USB_EP1_BUFFERSIZE_BYTES;                    // Maximum Tx payload (512 bytes is the maximum)
        USBE1CSR2bits.PROTOCOL = 2;                                         // Bulk mode
        USBE1CSR1bits.PIDERR = 0;
        USBE1CSR3bits.SPEED = 1;                                            // Hi-Speed
        
        /* Disable endpoint 1 Rx interrupt */
        USBCSR2bits.EP1RXIE = 0;    
    }
}

void USB_TXEndpoint0(volatile uint8_t * pBuffer, uint32_t numberOfBytes)
{
    volatile uint8_t * pUSBFIFO = (uint8_t *)&USBFIFO0;
    uint32_t i;
    
    /* Wait for ongoing transmission */
    while (USBE0CSR0bits.TXRDY);
    
    /* Load data into USB FIFO */
    for (i = 0; i < numberOfBytes; i++)
        *pUSBFIFO = pBuffer[i];

    /* Set Tx packet ready control bit (USB FIFO loaded) */
    USBE0CSR0bits.TXRDY = 1;
    
    /* Wait for ongoing transmission */
    while (USBE0CSR0bits.TXRDY);
}

void USB_RXEndpoint1()
{
	USB_Ep1_NumberOfReceivedBytes = USBE1CSR2bits.RXCNT;
    volatile uint8_t * pUSBFIFO = (uint8_t *)&USBFIFO1;
    
    /* Read from USB FIFO */
	uint32_t i;
    for (i = 0; i < USB_Ep1_NumberOfReceivedBytes; i++)
	    USB_Ep1_RXBuffer[i] = *(pUSBFIFO + (i & 0x03));

    /* Data has been unloaded from Rx FIFO */
	USBE1CSR1bits.RXPKTRDY = 0;
}

void USB_TXEndpoint1(volatile uint8_t * pBuffer)
{
	volatile uint8_t * pUSBFIFO;
    uint32_t p;
    uint32_t i;
    
    /* Configure endpoint 1 as in, i.e. Tx, for now (Tx: device to host) */
    USB_ConfigureEndpoint1(USB_DIRECTION_IN);
    
    /* Pointer on USB FIFO */
    pUSBFIFO = (uint8_t *)&USBFIFO1;
    
    /* Send several packets */
    for (p = 0; p < USB_NUMBER_OF_PACKETS; p++)
    {
        /* Wait for ongoing transmission */
        while (USBE1CSR0bits.TXPKTRDY == 1);
    
        /* Load data into the FIFO */
        for (i = 0; i < USB_BYTES_PER_PACKET; i++)
            *pUSBFIFO = pBuffer[(p*USB_BYTES_PER_PACKET) + i];
        
        /* Data packet has been loaded into Tx FIFO */
        USBE1CSR0bits.TXPKTRDY = 1;

        /* Wait for ongoing transmission */
        while (USBE1CSR0bits.TXPKTRDY == 1);
    }
    
    /* Configure endpoint 1 as out, i.e. Rx, for now (Rx: host to device) */
    USB_ConfigureEndpoint1(USB_DIRECTION_OUT);
}

/*******************************************************************************
 End of File
*/