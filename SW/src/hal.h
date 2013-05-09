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


void Hal_initialize(void);

void Hal_statusLedSet(bool_t on);

void Hal_lcdBacklightSet(bool_t on);

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
uint8_t Hal_encoderQuadGet(void);

// Button states
#define HAL_BUTTON_PUSH  (1u << 5)
#define HAL_BUTTON_SEL   (1u << 4)
#define HAL_BUTTON_BACK  (1u << 3)
uint8_t Hal_buttonStatesGet(void);


void Hal_interruptsDisable(void);
void Hal_interruptsEnable(void);

uint16_t Hal_tickCountGet_ISR(void);
uint16_t Hal_tickCountGet_MAIN(void);

bool_t Hal_midiTxGetActive_ISR(void);
void   Hal_midiTxSend_ISR(uint8_t x);
void   Hal_midiTxEnableIsr_ISR(bool_t en);

#endif /* HAL_H_ */
