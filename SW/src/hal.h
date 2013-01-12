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

void hal_SetStatusLed(bool_t on);

void hal_LcdBacklight(bool_t on);

// LCD hardware defines
#define LCD_DATA_PORT  PORTC
#define LCD_DATA_DDR   DDRC
#define LCD_DATA_PIN   PINC
#define LCD_CTRL_PORT  PORTD
#define LCD_CTRL_DDR   DDRD
#define LCD_RS_BIT     (1u << 5)
#define LCD_RW_BIT     (1u << 6)
#define LCD_CLK_BIT    (1u << 7)


// Quadrature input from rotary encoder

#define HAL_QUAD_A     0x01
#define HAL_QUAD_B     0x02
uint8_t hal_GetQuadState(void);

// Button states
#define HAL_BUTTON_PUSH  (1u << 5)
#define HAL_BUTTON_SEL   (1u << 4)
#define HAL_BUTTON_BACK  (1u << 3)
uint8_t hal_GetButtonStates(void);


void hal_DisableInterrupts(void);
void hal_EnableInterrupts(void);

uint16_t hal_GetTickCounterISR(void);

bool_t hal_MidiOutTxActive(void);
void hal_MidiOutTx(uint8_t x);
void hal_MidiOutIsrEnable(bool_t en);

#endif /* HAL_H_ */
