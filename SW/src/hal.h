/*
 * hal.h
 *
 *  Created on: Dec 8, 2012
 *      Author: lars
 */

#ifndef HAL_H_
#define HAL_H_

#include "common.h"
#include <avr/io.h>


void hal_Initialize(void);

void hal_StatusLedSet(bool_t on);

void hal_LcdBacklightSet(bool_t on);

// LCD hardware defines
#define HAL_LCD_DATA_PORT  PORTC
#define HAL_LCD_DATA_DDR   DDRC
#define HAL_LCD_DATA_PIN   PINC
#define HAL_LCD_CTRL_PORT  PORTD
#define HAL_LCD_CTRL_DDR   DDRD
#define HAL_LCD_RS_BIT     (1u << 5)
#define HAL_LCD_RW_BIT     (1u << 6)
#define HAL_LCD_CLK_BIT    (1u << 7)

// Quadrature input from rotary encoder

#define HAL_ENCODER_QUAD_A     0x01
#define HAL_ENCODER_QUAD_B     0x02
uint8_t hal_EncoderQuadGet(void);

// Button states
#define HAL_BUTTON_PUSH  (1u << 5)
#define HAL_BUTTON_SEL   (1u << 4)
#define HAL_BUTTON_BACK  (1u << 3)
uint8_t hal_ButtonStatesGet(void);


void hal_InterruptsDisable(void);
void hal_InterruptsEnable(void);

uint16_t hal_TickCountGet_ISR(void);
uint16_t hal_TickCountGet(void);

bool_t hal_MidiTxActiveGet_ISR(void);
void   hal_MidiTxSend_ISR(uint8_t x);
void   hal_MidiTxIsrEnable_ISR(bool_t en);

#endif /* HAL_H_ */
