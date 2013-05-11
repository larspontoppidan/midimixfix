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


void hal_initialize(void);

void hal_statusLedSet(bool_t on);

void hal_lcdBacklightSet(bool_t on);

void hal_debugSignalSet(bool_t value);

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
uint8_t hal_encoderQuadGet(void);

// Button states
#define HAL_BUTTON_PUSH  (1u << 5)
#define HAL_BUTTON_SEL   (1u << 4)
#define HAL_BUTTON_BACK  (1u << 3)
uint8_t hal_buttonStatesGet(void);


void hal_interruptsDisable(void);
void hal_interruptsEnable(void);

uint16_t hal_tickCountGet_ISR(void);
uint16_t hal_tickCountGet_MAIN(void);

bool_t hal_midiTxGetActive_ISR(void);
void   hal_midiTxSend_ISR(uint8_t x);
void   hal_midiTxEnableIsr_ISR(bool_t en);


uint16_t hal_adcGetValue_ISR(uint8_t channel);
uint16_t hal_adcGetValue_MAIN(uint8_t channel);


#endif /* HAL_H_ */
