/*
 * hal.c
 *
 *  Created on: Dec 8, 2012
 *      Author: lars
 */



#include "hal.h"
#include "errors.h"
#include "filterhooks.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "quaddecode.h"
#include "midiprocessing.h"

/////////////////////////    Defines     /////////////////////////

#define QUADA_PIN   PIND
#define QUADA_PORT  PORTD
#define QUADA_DDR   DDRD
#define QUADA_MASK  (1u << 3)

#define QUADB_PIN   PINA
#define QUADB_PORT  PORTA
#define QUADB_DDR   DDRA
#define QUADB_MASK  (1u << 7)


#define BUTTONS_PIN   PINA
#define BUTTONS_PORT  PORTA
#define BUTTONS_DDR   DDRA

#define MIDI_BAUD_RATE 31250

// Debug signal is intended for timing measurements.
// It is pin 4 in J9 (ISP connector) and is driven high / low.
#define DEBUG_SIG_MASK (1u << 5)
#define DEBUG_SIG_PORT PORTB
#define DEBUG_SIG_DDR  DDRB


#define TX_BUFFER_MAX  30


// USB Hardware

#define USB_PORT         PORTB
#define USB_DDR          DDRB

#define USB_PULLUP_MASK  (1u << 1)
#define USB_DPLUS_MASK   (1u << 2)
#define USB_DMINUS_MASK  (1u << 0)


/////////////////////////   Variables    /////////////////////////

static uint16_t TickCount;

static uint8_t AdcState = 0;
static uint16_t AdcValue0;
static uint16_t AdcValue1;

static uint16_t AdcTemp;

// Midi transmit buffer
static uint8_t TxBuffer[TX_BUFFER_MAX];
static uint8_t TxBufferHead;
static uint8_t TxBufferTail;

/////////////////////////   Prototypes   /////////////////////////


static void midiIoSetup(void);
static void tickIsrSetup(void);
static void fastTimerSetup(void);
static void adcSetup(void);

static void outputTxComplete_ISR();

///////////////////////// Implementation /////////////////////////

void hal_initialize(void)
{
    wdt_disable();

    // Set unconfigured defaults on all IO to pull-up enabled inputs, except for
    // USB control signals, which should be high impedance to disable USB.
    PORTA = 0xFF;
    DDRA = 0;
    PORTB = ~(0x07);
    DDRB = 0;
    PORTC = 0xFF;
    DDRC = 0;
    PORTD = 0xFF;
    DDRD = 0;

    // Status led
    PORTB |= (1u << 4);
    DDRB |= (1u << 4);

    // LCD backlight
    PORTB &= ~(1u << 3);
    DDRB |= (1u << 3);

    // Debug signal is active low output:
    DEBUG_SIG_PORT &= ~DEBUG_SIG_MASK;
    DEBUG_SIG_DDR |= DEBUG_SIG_MASK;

    // Quadrature encoder, configure pull up enabled inputs
    QUADA_PORT |= QUADA_MASK;
    QUADA_DDR &= ~QUADA_MASK;

    QUADB_PORT |= QUADB_MASK;
    QUADB_DDR &= ~QUADB_MASK;

    // Setup quadrature signal change interrupts
    EICRA |= (1u << ISC10);  // INT1 (PD3) is on QUAD A signal
    EIMSK |= (1u << INT1);   // INT1 (PD3) is on QUAD A signal

    PCMSK0 |= (1u << PCINT7); // PCIN7 (PA7) is on QUAD B signal
    PCICR |= (1u << PCIE0);   // PCIN7 (PA7) is on QUAD B signal

    // Buttons, configure pull up enabled inputs
    BUTTONS_PORT |= (HAL_BUTTON_PUSH | HAL_BUTTON_SEL | HAL_BUTTON_BACK);
    BUTTONS_DDR &= ~(HAL_BUTTON_PUSH | HAL_BUTTON_SEL | HAL_BUTTON_BACK);

    // Init other stuff
    midiIoSetup();
    tickIsrSetup();
    fastTimerSetup();
    adcSetup();
}


void hal_statusLedSet(bool_t on)
{
    if (on)
    {
        PORTB &= ~(1u << 4);
    }
    else
    {
        PORTB |= (1u << 4);
    }

}

void hal_debugSignalSet(bool_t value)
{
    if (value)
    {
        DEBUG_SIG_PORT |= DEBUG_SIG_MASK;
    }
    else
    {
        DEBUG_SIG_PORT &= ~DEBUG_SIG_MASK;
    }

}


void hal_lcdBacklightSet(bool_t on)
{
    if (on)
    {
        PORTB |= (1u << 3);
    }
    else
    {
        PORTB &= ~(1u << 3);
    }

}


uint8_t hal_encoderQuadGet(void)
{
    uint8_t ret;

    ret = (QUADA_PIN & QUADA_MASK) ? HAL_ENCODER_QUAD_A : 0;
    ret |= (QUADB_PIN & QUADB_MASK) ? HAL_ENCODER_QUAD_B : 0;

    return ret;
}


ISR(INT1_vect)
{
    quaddecode_handleAChange_ISR(QUADB_PIN & QUADB_MASK, (BUTTONS_PIN & HAL_BUTTON_PUSH) == 0u);
}


ISR(PCINT0_vect)
{
    quaddecode_handleBChange_ISR(QUADA_PIN & QUADA_MASK, (BUTTONS_PIN & HAL_BUTTON_PUSH) == 0u);
}



uint8_t hal_buttonStatesGet(void)
{
    uint8_t ret;

    ret = BUTTONS_PIN & (HAL_BUTTON_PUSH | HAL_BUTTON_SEL | HAL_BUTTON_BACK);
    ret ^= (HAL_BUTTON_PUSH | HAL_BUTTON_SEL | HAL_BUTTON_BACK);

    return ret;
}

void hal_interruptsDisable(void)
{
    cli();
}

void hal_interruptsEnable(void)
{
    sei();
}


// MIDI Input1 / Input2 / Output UARTs


static void midiIoSetup(void)
{
    // Setup USART0. This is used for Input1 and Output
    ///////

    // Set baud rate (non-double speed mode)
    UBRR0 = 39; // F_CPU / (16 * MIDI_BAUD_RATE) - 1;

    // No double speed mode
    UCSR0A = 0u;

    // Enable receiver and transmitter
    UCSR0B = (1u << RXCIE0) | (1u << RXEN0) | (1u << TXEN0);

    // Set frame format: 8 data, 1 stop bit
    UCSR0C = (1u << UCSZ01) | (1u << UCSZ00);

    // Reset TX buffer
    TxBufferHead = 0;
    TxBufferTail = 0;

    // Setup USART1 for Input2 (only)
    ////////

    // Set baud rate (non-double speed mode)
    UBRR1 = 39; // F_CPU / (16 * MIDI_BAUD_RATE) - 1;

    // No double speed mode
    UCSR1A = 0u;

    // Enable receiver
    UCSR1B = (1u << RXCIE1) | (1u << RXEN1);

    // Set frame format: 8 data, 1 stop bit
    UCSR1C = (1u << UCSZ11) | (1u << UCSZ10);
}

ISR(USART0_RX_vect)
{
    uint8_t x;

    hal_statusLedSet(TRUE);
    x = UDR0;
    midiin_handleInput1_ISR(x);
    hal_statusLedSet(FALSE);
}

ISR(USART1_RX_vect)
{
    uint8_t x;

    hal_statusLedSet(TRUE);
    x = UDR1;
    midiin_handleInput2_ISR(x);
    hal_statusLedSet(FALSE);
}


// Midi out transmission on UART0, data register empty interrupt
ISR(USART0_UDRE_vect)
{
    hal_statusLedSet(TRUE);
    outputTxComplete_ISR();
    hal_statusLedSet(FALSE);
}

static void outputTxComplete_ISR()
{
    if ((UCSR0A & (1u << UDRE0)) != 0u)
    {
        // We are done transmitting, more bytes to send?

        if (TxBufferTail != TxBufferHead)
        {
            // Yes, send next byte
            UDR0 = TxBuffer[TxBufferTail];

            if (TxBufferTail >= (TX_BUFFER_MAX - 1))
            {
                TxBufferTail = 0;
            }
            else
            {
                TxBufferTail++;
            }
        }
        else
        {
            // We didn't find anything to send, or are already finished
            // Turn off this interrupt
            UCSR0B &= ~(1u << UDRIE0);
        }
    }
}


bool_t hal_getMidiTxBufferEmpty_ISR(void)
{
    return (TxBufferHead == TxBufferTail);
}


void hal_midiTxEnqueue_ISR(uint8_t x)
{
    uint8_t index;

    // Grab a location in tx buffer and increment head

    index = TxBufferHead;

    if (TxBufferHead >= (TX_BUFFER_MAX - 1))
    {
        TxBufferHead = 0;
    }
    else
    {
        TxBufferHead++;
    }

    if (TxBufferTail == TxBufferHead)
    {
        // Oops, buffer overflow. Roll back and don't do anything
        TxBufferHead = index;
    }
    else
    {
        // Place byte in buffer
        TxBuffer[index] = x;
    }

    // Make sure the transmit interrupt system is running in any case
    UCSR0B |= (1u << UDRIE0);
}


// ADC

static void adcSetup(void)
{
    // The ADC converts the analog voltage on ADC0 and ADC1 which represents pedal 2 and 1.
    // Components that may access the pedals should have accurate data available which is
    // as fresh as possible.
    //
    // To achieve this, the ADC conversions are done with the help of a timer ISR's
    // that does 1, 2, 3 or 4 alternately:
    //   1. Store ADC1 result, set MUX to ADC0 (now MUX has has some time to settle)
    //   2. Start ADC0 conversion
    //   3. Store ADC0 result, set MUX to ADC1 (now MUX has has some time to settle)
    //   4. Start ADC1 conversion
    //
    // The AD conversion itself is driven by the ADC clock, which should be between 50 and 200 kHz
    // With the slowest prescaler 1/128 we get 20 MHz / 128 = 156 kHz. This means that an
    // AD conversion takes 25 clocks / (156e3) = 0.16 ms.
    //
    // A suitable interval for the timer ISR would be about 1 ms. This is a compromise between
    // not burdening the MCU too much, and age of the ADC data available. With 1 ms, the data
    // would be between 1 ms and 4 ms old.


    // Setup timer0 for CTC, 1/1024 prescaler and compare match on 19:
    //
    //  f(OCR) = f(clk) / (N * (1 + OCR))
    //         = 20E6 / (1024 * (1 + 19)) = 976.6 Hz
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS00) | (1 << CS02);
    TCNT0 = 0;
    OCR0A = 19;
    TIMSK0 = (1 << OCF0A);

    AdcState = 0;
    AdcTemp = 0;

    // Initial configuration of ADC. Use VCC as reference.

    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Turn off input drivers of ADC0 and ADC1
    DIDR0 = (1 << ADC0D) | (1 << ADC1D);
}

// ADC TICK ISR. Executes with 976.6 Hz
ISR(TIMER0_COMPA_vect)
{
    if (AdcState == 0)
    {
        // First save result from ADC1
        AdcTemp += ADC;
        AdcValue1 = AdcTemp >> 2;

        // Setup sampling ADC0
        AdcTemp = 0;
        ADMUX = (1 << REFS0);

        // Start first conversion
        ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

        AdcState++;
    }
    else if (AdcState == 4)
    {
        // First save result from ADC0
        AdcTemp += ADC;
        AdcValue0 = AdcTemp >> 2;

        // Setup sampling ADC1
        AdcTemp = 0;
        ADMUX = (1 << REFS0) | (1 << MUX0);

        // Start first conversion
        ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

        AdcState++;
    }
    else
    {
        // AdcState 1, 2, 3 or
        // AdcState 5, 6, 7

        // Add last result to sum
        AdcTemp += ADC;

        // Start next conversion
        ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

        // Increment with wraparound
        AdcState = (AdcState + 1) & 7;
    }

}


uint16_t hal_adcGetValue_ISR(uint8_t channel)
{
    uint16_t ret = 0;

    if (channel == 0)
    {
        ret = AdcValue0;
    }
    else if (channel == 1)
    {
        ret = AdcValue1;
    }

    return ret;
}

uint16_t hal_adcGetValue_MAIN(uint8_t channel)
{
    uint16_t ret = 0;

    if (channel == 0)
    {
        hal_interruptsDisable();
        ret = AdcValue0;
        hal_interruptsEnable();
    }
    else if (channel == 1)
    {
        hal_interruptsDisable();
        ret = AdcValue1;
        hal_interruptsEnable();
    }

    return ret;
}

// Fast tick setup
static void fastTimerSetup(void)
{
    // Setup timer2 for free running mode with 1/1024 prescaler
    TCCR2A = 0;
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
}

// 100 HZ tick implementation

static void tickIsrSetup(void)
{
    // We want to generate a 100 Hz TICK interrupt based on the 20 MHz MCU clock.
    // The prescaler configuration of AVR does not allow this to be done accurately
    // with an 8-bit timer. But it is possible with the 16-bit timer.
    //
    // In this case, 16-bit timer1 is configured to Clear on Compare Match (CTC),
    // (WGM = 4), with an 1/8 prescaler and a OCR register value of:
    //
    //   OCR   = f(clk) / (N * f(OCR)) - 1
    //         = 20E6 / (8 * 100) - 1 = 24999

    TickCount = 0;

    TCCR1A = 0;
    TCCR1B = (1<<WGM12) | (1<<CS11);
    OCR1A = 24999u;
    TIMSK1 = (1<<OCIE1A);
}

// TICK ISR. Executes with 100 Hz
ISR(TIMER1_COMPA_vect)
{
    TickCount++;
    FILTER_HOOKS_TICK_ISR();

    // Hal module nurses some of the modules directly:
    midiproc_handleTick_ISR();
    quaddecode_handleTickIsr_ISR();
}

uint16_t hal_tickCountGet_ISR(void)
{
    return TickCount;
}


uint16_t hal_tickCountGet_MAIN(void)
{
    uint16_t tc;

    hal_interruptsDisable(); // TODO check if this inlines
    tc = TickCount;
    hal_interruptsEnable();

    return tc;
}

void hal_jumpBootloader(void)
{
    wdt_enable(WDTO_500MS);

    hal_interruptsDisable();

    while(1)
    {

    }

}
