/*
 * hwtestmain.c
 *
 *  Created on: May 19, 2013
 *      Author: lars
 */


//
// Hardware bring up test:
//
// 1. Using ISP interface program fuses on avr (easy way, make fuse in USPaspLoader project)
//
// 2. Using ISP program midimixfix software release
//
// 3. Remove all connectors, place jumper between 3 and 4 in ISP connector
//
// 4. Plugin usb power. Observe status led blinking.
//    5 blinks first indicates test mode is entered
//    2 blinks, 8 blinks
//    2 blinks, 6 blinks indicates short between PORTB7 and PORTB5 (no problem, it's the jumper)
//    (other blinked codes IS a problem, investigate)
//
// 5. Power off, connect everything: display, buttons, optocouplers, pedals
//
// 6. Power on, (keep jumper between 3 and 4), after the blinking, display driven tests now start
//
//    - Knob idle: 00 03, clockwise 01, 00, 02 back to 03
//    - Push select: 10, back: 08, knob: 20
//    - Yamaha pedal forward ~25, back ~855
//    - Patch midi-out to midi-in1. Push select should generate 55, push back for AA
//    - Patch midi-out to midi-in2. Push select should generate 55, push back for AA


#include "../common.h"
#include "hardwaretest.h"
#include "../util.h"
#include "../hal.h"
#include "../lcd.h"

#include <util/delay.h>
#include <avr/pgmspace.h>


// What to avoid in pattern test:
#define PORTA_DONT_TEST  0xC0  // Don't touch GC1 and GC2
#define PORTB_DONT_TEST  0x17  // Don't touch USB control lines and LED
#define PORTC_DONT_TEST  0x00
#define PORTD_DONT_TEST  0x08  // Don't touch PD3 which is connected to GC1


// LED flashing
#define LED_PORT PORTB
#define LED_DDR  DDRB
#define LED_MASK (1 << 4)

#define FLASH_DELAY_MS  500
#define FLASH_ON_MS     250
#define FLASH_OFF_MS    250

// Jumper between pin 3 and 4 in ISP connector enables hardware test:
#define TESTJUMPER_PORT  PORTB
#define TESTJUMPER_DDR   DDRB
#define TESTJUMPER_PIN   PINB
#define TESTJUMPER_1     (1u << 7)
#define TESTJUMPER_2     (1u << 5)

#define CHECK_SAMPLES    4

static bool_t checkTestModeJumper(void)
{
    uint8_t i;
    uint8_t count = 0;

    // Set one pin pull up input
    TESTJUMPER_DDR  &= ~TESTJUMPER_1;
    TESTJUMPER_PORT |=  TESTJUMPER_1;

    // The other pin active low
    TESTJUMPER_DDR  |=  TESTJUMPER_2;
    TESTJUMPER_PORT &= ~TESTJUMPER_2;

    // If TESTJUMPER_1 pin went low, there's a jumper.
    // Since this is a high impedance, over sample and
    // only accept if all samples are low.

    for (i = 0; i < CHECK_SAMPLES; i++)
    {
        _delay_ms(1);

        if ((TESTJUMPER_PIN & TESTJUMPER_1) == 0)
        {
            count++;
        }
    }

    // Reset IO states
    TESTJUMPER_DDR  &= ~(TESTJUMPER_1 | TESTJUMPER_2);
    TESTJUMPER_PORT &= ~(TESTJUMPER_1 | TESTJUMPER_2);

    return (count == CHECK_SAMPLES);
}

static void flashLed(uint8_t flashes)
{
    uint8_t i;

    // Set LED off and correct data direction
    LED_PORT |= LED_MASK;
    LED_DDR  |= LED_MASK;

    _delay_ms(FLASH_DELAY_MS);

    for (i = 0; i < flashes; i++)
    {
        LED_PORT &= ~LED_MASK;
        _delay_ms(FLASH_ON_MS);
        LED_PORT |= LED_MASK;
        _delay_ms(FLASH_OFF_MS);
    }
}


static void ioPatternTest(uint8_t pattern)
{
    uint8_t errors[4];

    // Setup pattern on IO alternating between active low and active high

    PORTA = pattern & (~PORTA_DONT_TEST);
    DDRA  = 0xFF    & (~PORTA_DONT_TEST);

    PORTB = pattern & (~PORTB_DONT_TEST);
    DDRB  = 0xFF    & (~PORTB_DONT_TEST);

    PORTC = pattern & (~PORTC_DONT_TEST);
    DDRC  = 0xFF    & (~PORTC_DONT_TEST);

    PORTD = pattern & (~PORTD_DONT_TEST);
    DDRD  = 0xFF    & (~PORTD_DONT_TEST);

    _delay_ms(1);

    // If there are differences between port and pin states, there must be
    // some kind of a short:

    errors[0] = (PINA ^ pattern) & (~PORTA_DONT_TEST);
    errors[1] = (PINB ^ pattern) & (~PORTB_DONT_TEST);
    errors[2] = (PINC ^ pattern) & (~PORTC_DONT_TEST);
    errors[3] = (PIND ^ pattern) & (~PORTD_DONT_TEST);

    // Back to default states
    DDRA  = 0;
    PORTA = 0;

    DDRB  = 0;
    PORTB = 0;

    DDRC  = 0;
    PORTC = 0;

    DDRD  = 0;
    PORTD = 0;

    // Blink out errors on LED

    uint8_t port;
    uint8_t pin;

    for (port = 0; port < 4; port++)
    {
        for (pin = 0; pin < 8; pin++)
        {
            if (errors[port] & (1 << pin))
            {
                // We need delay to separate numbers
                _delay_ms(2000);
                // One blink PORTA, Two blinks PORTB, ...
                flashLed(port + 1);
                // One blink PIN1, two blinks PIN2, ...
                flashLed(pin + 1);
            }
        }
    }

}

static void fillLcd(char c)
{
    uint8_t i;

    // Fill LCD with character
    for (i = 0; i < 4; i++)
    {
        lcd_setCursor(i, 0);
        lcd_writeRepeat(c, 20);
    }

    _delay_ms(1000);
}


static void testMode(void)
{
    char buffer[20];
    char *p;
    bool_t alt = FALSE;
    uint16_t adc1 = 0, adc2 = 0;
    uint8_t midi_in;

    while (TRUE)
    {
        // Get input states
        lcd_setCursor(0, 0);
        lcd_writeString_P(PSTR("TEST_MODE"));

        // Write button states
        p = util_writeHex(buffer, hal_buttonStatesGet());
        *p++ = ' ';

        // Write encoder bits
        p = util_writeHex(p, hal_encoderQuadGet());
        *p++ = ' ';

        // Alternatingly start AD conversion on channel 0 and 1. First save result from last conversion
        if (alt)
        {
            adc1 = ADC;

            ADMUX = (1 << REFS0);
            ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
        }
        else
        {
            adc2 = ADC;

            ADMUX = (1 << REFS0) | (1 << MUX0);
            ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
        }

        p = util_writeInt16LA(p, adc1);
        *p++ = ' ';
        p = util_writeInt16LA(p, adc2);
        *p++ = ' ';
        *p++ = 0;

        // This completes line 2, write it on display
        lcd_setCursor(1,0);
        lcd_writeString(buffer);

        // See if there is something on midi in 1
        midi_in = 0;
        while ((UCSR0A & (1 << RXC0)) != 0)
        {
            midi_in = UDR0;
        }

        p = util_writeHex(buffer, midi_in);
        *p++ = ' ';

        // See if there is something on midi in 2
        midi_in = 0;
        while ((UCSR1A & (1 << RXC1)) != 0)
        {
            midi_in = UDR1;
        }

        p = util_writeHex(p, midi_in);
        *p++ = 0;

        // This completes line 3, write it on display
        lcd_setCursor(2,0);
        lcd_writeString(buffer);

        // Send byte on MIDI OUT if BACK or SEL is pushed
        if (hal_buttonStatesGet() & HAL_BUTTON_BACK)
        {
            UDR0 = 0xAA;
        }
        else if (hal_buttonStatesGet() & HAL_BUTTON_SEL)
        {
            UDR0 = 0x55;
        }

        _delay_ms(100);

        alt = !alt;
    }


}

void hwtest_enterIfTestMode(void)
{
    // Look for a short between 1 and 3
    if (checkTestModeJumper())
    {
        // OK, we are in test mode, say hello with five blinks
        flashLed(5);

        // Before we attempt to use the display, test lowlevel IO for shorts

        ioPatternTest(0x55); // 0b01010101
        ioPatternTest(0xAA); // 0b10101010

        ioPatternTest(0x92); // 0b10010010
        ioPatternTest(0x49); // 0b01001001
        ioPatternTest(0x24); // 0b00100100

        // Now, lets initialize regular midimixfix modules
        hal_initialize();
        lcd_initialize();

        // Try some patterns on LCD
        hal_lcdBacklightSet(TRUE);
        fillLcd(LCD_CHAR_FILLED_RECT);
        fillLcd(' ');
        fillLcd('a');
        fillLcd('Z');
        fillLcd(' ');

        // and enter interactive test mode
        testMode();

    }

}

