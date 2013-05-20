/*
 * hwtesthal.c
 *
 *  Created on: May 19, 2013
 *      Author: lars
 */

#include "ht_hal.h"
#include <avr/pgmspace.h>
#include <util/delay.h>

#define IO_NA         0
#define IO_PORTA      0x10
#define IO_PORTB      0x20
#define IO_PORTC      0x30
#define IO_PORTD      0x40
#define IO_PORT_MASK  0xF0

#define IO_BIT_MASK   0x0F


#define DDR_OUT   1
#define DDR_IN    0
#define PORT_HI   1
#define PORT_LO   0

#define FLASH_ON_MS     250
#define FLASH_SHORT_MS   10
#define FLASH_OFF_MS    250
#define FLASH_END_MS    500



// Map of atmega644p TQFP44 pins to IO defines:

#define IO_MAX 44

static uint8_t IoMap[IO_MAX] PROGMEM =
{
        IO_PORTB | 5,   // Pin 1
        IO_PORTB | 6,
        IO_PORTB | 7,
        IO_NA,
        IO_NA,
        IO_NA,
        IO_NA,
        IO_NA,
        IO_PORTD | 0,   // Pin 9
        IO_PORTD | 1,
        IO_PORTD | 2,
        IO_PORTD | 3,
        IO_PORTD | 4,
        IO_PORTD | 5,
        IO_PORTD | 6,
        IO_PORTD | 7,
        IO_NA,          // Pin 17
        IO_NA,
        IO_PORTC | 0,   // Pin 19
        IO_PORTC | 1,
        IO_PORTC | 2,
        IO_PORTC | 3,
        IO_PORTC | 4,
        IO_PORTC | 5,
        IO_PORTC | 6,
        IO_PORTC | 7,
        IO_NA,          // Pin 27
        IO_NA,
        IO_NA,
        IO_PORTA | 7,   // Pin 30
        IO_PORTA | 6,
        IO_PORTA | 5,
        IO_PORTA | 4,
        IO_PORTA | 3,
        IO_PORTA | 2,
        IO_PORTA | 1,
        IO_PORTA | 0,
        IO_NA,          // Pin 38
        IO_NA,
        IO_NA, // IO_PORTB | 0,   // Pin 40
        IO_NA, // IO_PORTB | 1,
        IO_NA, // IO_PORTB | 2,
        IO_PORTB | 3,
        IO_PORTB | 4,   // Pin 44
};


static void setIoState(uint8_t pin_no, bool_t ddr, bool_t port)
{
    uint8_t i;
    uint8_t portname;
    uint8_t bitmask;

    if (pin_no <= IO_MAX)
    {
        // Look up port and bitmask of pin_no:
        i = pgm_read_byte(&(IoMap[pin_no-1]));
        bitmask = 1u << (i & IO_BIT_MASK);
        portname = i & IO_PORT_MASK;

        // Convert ddr and port to set the correct bit
        ddr = ddr ? bitmask : 0;
        port = port ? bitmask : 0;

        // Invert bitmask
        bitmask = ~bitmask;

        switch(portname)
        {
        case IO_PORTA:
            DDRA = (DDRA & bitmask) | ddr;
            PORTA = (PORTA & bitmask) | port;
            break;
        case IO_PORTB:
            DDRB = (DDRB & bitmask) | ddr;
            PORTB = (PORTB & bitmask) | port;
            break;
        case IO_PORTC:
            DDRC = (DDRC & bitmask) | ddr;
            PORTC = (PORTC & bitmask) | port;
            break;
        case IO_PORTD:
            DDRD = (DDRD & bitmask) | ddr;
            PORTD = (PORTD & bitmask) | port;
            break;
        }
    }

}


static bool_t getIoState(uint8_t pin_no)
{
    uint8_t i;
    uint8_t portname;
    uint8_t bitmask;

    // Look up port and bitmask of pin_no:
    i = pgm_read_byte(&(IoMap[pin_no-1]));
    bitmask = 1u << (i & IO_BIT_MASK);
    portname = i & IO_PORT_MASK;

    switch(portname)
    {
    case IO_PORTA:
        i = PINA & bitmask;
        break;
    case IO_PORTB:
        i = PINB & bitmask;
        break;
    case IO_PORTC:
        i = PINC & bitmask;
        break;
    case IO_PORTD:
        i = PIND & bitmask;
        break;
    }

    return (i != 0) ? TRUE : FALSE;
}


static bool_t getIoAvailable(uint8_t pin_no)
{
    uint8_t i = IO_NA;

    if ((pin_no > 0) && (pin_no <= IO_MAX))
    {
        i = pgm_read_byte(&(IoMap[pin_no-1]));
    }

    return i != IO_NA;
}

bool_t hthal_testIoGnd(uint8_t pin_no)
{
    bool_t ret = FALSE;

    if (getIoAvailable(pin_no))
    {
        // Try setting IO pull up
        setIoState(pin_no, DDR_IN, PORT_HI);

        // If IO goes lo we are grounded
        ret = !(getIoState(pin_no));

        // Set Io state back to defaults
        setIoState(pin_no, DDR_IN, PORT_LO);
    }

    return ret;
}

bool_t hthal_testIoVcc(uint8_t pin_no)
{
    bool_t ret = FALSE;

    if (getIoAvailable(pin_no))
    {
        // Try setting IO low output
        setIoState(pin_no, DDR_OUT, PORT_LO);

        // If IO is still hi, we are tied to VCC
        ret = getIoState(pin_no);

        // Set Io state back to defaults
        setIoState(pin_no, DDR_IN, PORT_LO);
    }

    return ret;
}

bool_t hthal_testIoShort(uint8_t pin_no1, uint8_t pin_no2)
{
    bool_t ret = FALSE;

    if ((pin_no1 != pin_no2) && getIoAvailable(pin_no1) && getIoAvailable(pin_no2))
    {
        // Set IO 1 pull up, IO 2 active low
        setIoState(pin_no1, DDR_IN, PORT_HI);
        setIoState(pin_no2, DDR_OUT, PORT_LO);

        // If IO 1 is now low, there is a short, otherwise it will be high
        ret = !(getIoState(pin_no1));

        // Set Io state back to defaults
        setIoState(pin_no1, DDR_IN, PORT_LO);
        setIoState(pin_no2, DDR_IN, PORT_LO);
    }

    return ret;
}

void hthal_initialize(void)
{
    // Just turn off LED
    setIoState(44, DDR_OUT, PORT_HI);
}


void hthal_flashLed(uint8_t flashes)
{
    uint8_t i;

    _delay_ms(FLASH_END_MS);

    if (flashes == 0)
    {
        setIoState(44, DDR_OUT, PORT_LO);
        _delay_ms(FLASH_SHORT_MS);
        setIoState(44, DDR_OUT, PORT_HI);
        _delay_ms(FLASH_OFF_MS * 2);
    }
    else
    {
        for (i = 0; i < flashes; i++)
        {
            setIoState(44, DDR_OUT, PORT_LO);
            _delay_ms(FLASH_ON_MS);
            setIoState(44, DDR_OUT, PORT_HI);
            _delay_ms(FLASH_OFF_MS);
        }
    }

}

