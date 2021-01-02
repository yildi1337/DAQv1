/* ************************************************************************** */
/** GPIO functionality

  @Author
    Phillip Durdaut (2020)

  @File Name
    gpio.h
 */
/* ************************************************************************** */

#ifndef _GPIO_H    /* Guard against multiple inclusion */
#define _GPIO_H

#include "main.h"

#define GPIO_LED1_Set()                 (LATGSET = (1 << 6))
#define GPIO_LED1_Clear()               (LATGCLR = (1 << 6))
#define GPIO_LED1_Toggle()              (LATGINV = (1 << 6))
#define GPIO_LED1_Get()                 ((PORTG >> 6) & 0x1)
#define GPIO_LED1_OutputEnable()        (TRISGCLR = (1 << 6))
#define GPIO_LED1_InputEnable()         (TRISGSET = (1 << 6))
#define GPIO_LED1_PIN                   GPIO_PIN_RG6

#define GPIO_LED2_Set()                 (LATGSET = (1 << 7))
#define GPIO_LED2_Clear()               (LATGCLR = (1 << 7))
#define GPIO_LED2_Toggle()              (LATGINV = (1 << 7))
#define GPIO_LED2_Get()                 ((PORTG >> 7) & 0x1)
#define GPIO_LED2_OutputEnable()        (TRISGCLR = (1 << 7))
#define GPIO_LED2_InputEnable()         (TRISGSET = (1 << 7))
#define GPIO_LED2_PIN                   GPIO_PIN_RG7

#define GPIO_LED3_Set()                 (LATGSET = (1 << 8))
#define GPIO_LED3_Clear()               (LATGCLR = (1 << 8))
#define GPIO_LED3_Toggle()              (LATGINV = (1 << 8))
#define GPIO_LED3_Get()                 ((PORTG >> 8) & 0x1)
#define GPIO_LED3_OutputEnable()        (TRISGCLR = (1 << 8))
#define GPIO_LED3_InputEnable()         (TRISGSET = (1 << 8))
#define GPIO_LED3_PIN                   GPIO_PIN_RG8

typedef enum
{
    GPIO_PIN_RB0 = 0,
    GPIO_PIN_RB1 = 1,
    GPIO_PIN_RB2 = 2,
    GPIO_PIN_RB3 = 3,
    GPIO_PIN_RB4 = 4,
    GPIO_PIN_RB5 = 5,
    GPIO_PIN_RB6 = 6,
    GPIO_PIN_RB7 = 7,
    GPIO_PIN_RB8 = 8,
    GPIO_PIN_RB9 = 9,
    GPIO_PIN_RB10 = 10,
    GPIO_PIN_RB11 = 11,
    GPIO_PIN_RB12 = 12,
    GPIO_PIN_RB13 = 13,
    GPIO_PIN_RB14 = 14,
    GPIO_PIN_RB15 = 15,
    GPIO_PIN_RC12 = 28,
    GPIO_PIN_RC13 = 29,
    GPIO_PIN_RC14 = 30,
    GPIO_PIN_RC15 = 31,
    GPIO_PIN_RD0 = 32,
    GPIO_PIN_RD1 = 33,
    GPIO_PIN_RD2 = 34,
    GPIO_PIN_RD3 = 35,
    GPIO_PIN_RD4 = 36,
    GPIO_PIN_RD5 = 37,
    GPIO_PIN_RD9 = 41,
    GPIO_PIN_RD10 = 42,
    GPIO_PIN_RD11 = 43,
    GPIO_PIN_RE0 = 48,
    GPIO_PIN_RE1 = 49,
    GPIO_PIN_RE2 = 50,
    GPIO_PIN_RE3 = 51,
    GPIO_PIN_RE4 = 52,
    GPIO_PIN_RE5 = 53,
    GPIO_PIN_RE6 = 54,
    GPIO_PIN_RE7 = 55,
    GPIO_PIN_RF0 = 64,
    GPIO_PIN_RF1 = 65,
    GPIO_PIN_RF3 = 67,
    GPIO_PIN_RF4 = 68,
    GPIO_PIN_RF5 = 69,
    GPIO_PIN_RG6 = 86,
    GPIO_PIN_RG7 = 87,
    GPIO_PIN_RG8 = 88,
    GPIO_PIN_RG9 = 89,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;

extern void GPIO_Init(void);

#endif /* _GPIO_H */

/* *****************************************************************************
 End of File
 */
