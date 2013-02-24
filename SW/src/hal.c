/*
 * hal.c
 *
 *  Created on: Dec 8, 2012
 *      Author: lars
 */



#include "hal.h"
#include "errors.h"
#include "midiio.h"
#include "midiparser.h"
#include "componenthooks.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "quaddecode.h"

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

/////////////////////////   Variables    /////////////////////////

static uint16_t tickCount;


/////////////////////////   Prototypes   /////////////////////////


static void hal_MidiIoSetup(void);
static void hal_TickIsrSetup(void);


///////////////////////// Implementation /////////////////////////

void hal_Initialize(void)
{
    // Status led
    PORTB |= (1u << 4);
    DDRB |= (1u << 4);

    // LCD backlight
    PORTB &= ~(1u << 3);
    DDRB |= (1u << 3);

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
    hal_MidiIoSetup();
    hal_TickIsrSetup();
}

void hal_StatusLedSet(bool_t on)
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

void hal_LcdBacklightSet(bool_t on)
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


uint8_t hal_EncoderQuadGet(void)
{
    uint8_t ret;

    ret = (QUADA_PIN & QUADA_MASK) ? HAL_ENCODER_QUAD_A : 0;
    ret |= (QUADB_PIN & QUADB_MASK) ? HAL_ENCODER_QUAD_B : 0;

    return ret;
}


ISR(INT1_vect)
{
    qd_handleAchange_isr(QUADB_PIN & QUADB_MASK, (BUTTONS_PIN & HAL_BUTTON_PUSH) == 0u);
}


ISR(PCINT0_vect)
{
    qd_handleBchange_isr(QUADA_PIN & QUADA_MASK, (BUTTONS_PIN & HAL_BUTTON_PUSH) == 0u);
}



uint8_t hal_ButtonStatesGet(void)
{
    uint8_t ret;

    ret = BUTTONS_PIN & (HAL_BUTTON_PUSH | HAL_BUTTON_SEL | HAL_BUTTON_BACK);
    ret ^= (HAL_BUTTON_PUSH | HAL_BUTTON_SEL | HAL_BUTTON_BACK);

    return ret;
}

void hal_InterruptsDisable(void)
{
    cli();
}

void hal_InterruptsEnable(void)
{
    sei();
}


// MIDI Input1 / Input2 / Output UARTs


static void hal_MidiIoSetup(void)
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

    hal_StatusLedSet(TRUE);
    x = UDR0;
    mparser_handleInput1Rx_Isr(x);
    hal_StatusLedSet(FALSE);
}

ISR(USART1_RX_vect)
{
    uint8_t x;

    hal_StatusLedSet(TRUE);
    x = UDR1;
    mparser_handleInput2Rx_Isr(x);
    hal_StatusLedSet(FALSE);
}


// Midi out transmission on UART0, data register empty interrupt
ISR(USART0_UDRE_vect)
{
    hal_StatusLedSet(TRUE);
    midiio_OutputTxComplete_ISR();
    hal_StatusLedSet(FALSE);
}


// Return true if Uart Data Register Empty bit is not set
bool_t hal_MidiTxActiveGet_ISR(void)
{
    return ((UCSR0A & (1u << UDRE0)) == 0u);
}


void hal_MidiTxSend_ISR(uint8_t x)
{
    UDR0 = x;
}

void hal_MidiTxIsrEnable_ISR(bool_t en)
{

    if (en)
    {
        // Enable the USART0_UDRE_vect interrupt
        UCSR0B |= (1u << UDRIE0);
    }
    else
    {
        UCSR0B &= ~(1u << UDRIE0);
    }

}

// 100 HZ tick implementation

static void hal_TickIsrSetup(void)
{
    // We want to generate a 100 Hz TICK interrupt based on the 20 MHz MCU clock.
    // The prescaler configuration of AVR does not allow this to be done accurately
    // with an 8-bit timer. But it is possible with the 16-bit timer.
    //
    // In this case, 16-bit timer1 is configured to Clear on Compare Match (CTC),
    // (WGM = 4), with an 1/8 prescaler and a OCR register value of:
    //
    //   OCR   = f(clk) / (N * f(OCR)) - 1
    //         = 20e6 / (8 * 100) - 1 = 24999

    tickCount = 0;

    TCCR1A = 0;
    TCCR1B = (1<<WGM12) | (1<<CS11);
    OCR1A = 24999u;
    TIMSK1 = (1<<OCIE1A);
}

// TICK ISR. Executes with 100 Hz
ISR(TIMER1_COMPA_vect)
{
    tickCount++;
    COMP_TICK_ISR_HOOKS();
}

uint16_t hal_TickCountGet_ISR(void)
{
    return tickCount;
}


uint16_t hal_TickCountGet(void)
{
    uint16_t tc;

    hal_InterruptsDisable(); // TODO check if this inlines
    tc = tickCount;
    hal_InterruptsEnable();

    return tc;
}

