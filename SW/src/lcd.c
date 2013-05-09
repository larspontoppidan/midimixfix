/*
 * lcd.c
 *
 *  Created on: Dec 8, 2012
 *      Author: lars
 */

#include "lcd.h"
#include "hal.h"
#include "errors.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#define CMD_INITIALIZE        0x30 // 0b00110000

// LCD command definitions

#define CMD_CLEAR_DISP        0x01 // 0b00000001

#define CMD_RETURN_HOME       0x02 // 0b00000010

#define CMD_ENTRY_MODE        0x04 // 0b00000100
#define FLAG_ENTRY_MODE_INC   0x02 // 0b00000010
#define FLAG_ENTRY_MODE_SHIFT 0x01 // 0b00000001

#define CMD_DISP_CTRL         0x08 // 0b00001000
#define FLAG_DISP_CTRL_ON     0x04 // 0b00000100
#define FLAG_DISP_CTRL_CURSOR 0x02 // 0b00000010
#define FLAG_DISP_CTRL_BLINK  0x01 // 0b00000001

#define CMD_SHIFT_CURSOR      0x10 // 0b00010000
#define CMD_SHIFT_DISPLAY     0x18 // 0b00011000
#define FLAG_SHIFT_RIGHT      0x04 // 0b00000100
#define FLAG_SHIFT_LEFT       0x00 // 0b00000000

#define CMD_FUNC_SET          0x20 // 0b00100000
#define FLAG_FUNC_8BIT        0x10 // 0b00010000
#define FLAG_FUNC_MULT_LINES  0x08 // 0b00001000
#define FLAG_FUNC_5X8_FONT    0x04 // 0b00000100

#define CMD_SET_CGRAM_ADDR    0x40 // 0b01000000
#define CMD_SET_DDRAM_ADDR    0x80 // 0b10000000


// Busy status defines
#define FLAG_BUSY             0x80 // 0b10000000


/////////////////////////   Variables    /////////////////////////

static bool_t LcdReady = FALSE;


/////////////////////////   Prototypes   /////////////////////////


// Low level LCD IO functions
static void sendLcdCommand(uint8_t data);
static void sendLcdData(uint8_t data);

static uint8_t readLcdStatus(void);
//static uint8_t ReadLcdData(void);

static bool_t checkReady();

//////////////////////// Private functions ////////////////////////


static void sendLcdCommand(uint8_t data)
{
    // Wait for Lcd to be ready
    while (!checkReady())
        ;

    // Load data to data port and set direction output
    HAL_LCD_DATA_PORT = data;
    HAL_LCD_DATA_DDR = 0xFFu;

    // Clear RW bit, clear RS bit
    HAL_LCD_CTRL_PORT &= ~HAL_LCD_RW_BIT;
    HAL_LCD_CTRL_PORT &= ~HAL_LCD_RS_BIT;

    // Make clock transitions
    _delay_us(10);
    HAL_LCD_CTRL_PORT |= HAL_LCD_CLK_BIT;
    _delay_us(10);
    HAL_LCD_CTRL_PORT &= ~HAL_LCD_CLK_BIT;

    // Now display is in a non ready state
    LcdReady = FALSE;
}


static void sendLcdData(uint8_t data)
{
    // Wait for Lcd to be ready
    while (!checkReady())
        ;

    // Load data to data port and set direction output
    HAL_LCD_DATA_PORT = data;
    HAL_LCD_DATA_DDR = 0xFFu;

    // Clear RW bit, set RS bit
    HAL_LCD_CTRL_PORT &= ~HAL_LCD_RW_BIT;
    HAL_LCD_CTRL_PORT |= HAL_LCD_RS_BIT;

    // Make clock transitions
    _delay_us(10);
    HAL_LCD_CTRL_PORT |= HAL_LCD_CLK_BIT;
    _delay_us(10);
    HAL_LCD_CTRL_PORT &= ~(HAL_LCD_CLK_BIT | HAL_LCD_RS_BIT);

    // Now display is in a non ready state
    LcdReady = FALSE;
}


static uint8_t readLcdStatus(void)
{
    uint8_t data;

    // Make port input, no pullup
    HAL_LCD_DATA_PORT = 0u;
    HAL_LCD_DATA_DDR = 0u;

    // Set RW bit, clear RS bit
    HAL_LCD_CTRL_PORT |= HAL_LCD_RW_BIT;
    HAL_LCD_CTRL_PORT &= ~HAL_LCD_RS_BIT;

    _delay_us(10);
    HAL_LCD_CTRL_PORT |= HAL_LCD_CLK_BIT;
    _delay_us(10);

    data = HAL_LCD_DATA_PIN;

    HAL_LCD_CTRL_PORT &= ~(HAL_LCD_CLK_BIT | HAL_LCD_RS_BIT | HAL_LCD_RW_BIT);

    return data;
}

//static uint8_t ReadLcdData(void)
//{
//    uint8_t data;
//
//    // Make port input, no pullup
//    HAL_LCD_DATA_PORT = 0u;
//    HAL_LCD_DATA_DDR = 0u;
//
//    // Set RW bit, set or clear RS bit
//    HAL_LCD_CTRL_PORT |= HAL_LCD_RW_BIT;
//    HAL_LCD_CTRL_PORT |= HAL_LCD_RS_BIT;
//
//    _delay_us(10);
//    HAL_LCD_CTRL_PORT |= HAL_LCD_CLK_BIT;
//    _delay_us(10);
//
//    data = HAL_LCD_DATA_PIN;
//
//    HAL_LCD_CTRL_PORT &= ~(HAL_LCD_CLK_BIT | HAL_LCD_RS_BIT | HAL_LCD_RW_BIT);
//
//    return data;
//}


// Read the LCD status and check if module is ready for new command or data
// The ready status is cached such that extra requests are avoided
static bool_t checkReady()
{
    if (LcdReady == FALSE)
    {
        if ((readLcdStatus() & FLAG_BUSY) == 0u)
        {
            LcdReady = TRUE;
        }
    }

    return LcdReady;
}


//////////////////////// Public functions ////////////////////////

void Lcd_initialize(void)
{
    // Make data port inputs and control signals low outputs

    HAL_LCD_DATA_PORT = 0u;
    HAL_LCD_DATA_DDR = 0u;
    HAL_LCD_CTRL_PORT &= ~(HAL_LCD_CLK_BIT | HAL_LCD_RS_BIT | HAL_LCD_RW_BIT);
    HAL_LCD_CTRL_DDR |= (HAL_LCD_CLK_BIT | HAL_LCD_RS_BIT | HAL_LCD_RW_BIT);

    // Delay for more than 15 ms for power on reset.
    _delay_ms(16u);

    // Send initialize command and wait at least 4.1ms extra
    // Bypass lcd busy checking at this point
    LcdReady = TRUE;
    sendLcdCommand(CMD_INITIALIZE);
    _delay_ms(6);

    // Send initialize command and wait at least 100 us extra
    // Bypass lcd busy checking at this point
    LcdReady = TRUE;
    sendLcdCommand(CMD_INITIALIZE);
    _delay_ms(1);

    // Send initialize command one final time
    // Bypass ready checking at this point
    LcdReady = TRUE;
    sendLcdCommand(CMD_INITIALIZE);

    // Now we can use ready checking by the busy flag

    // Send function set command with configuration
    sendLcdCommand(CMD_FUNC_SET |
                   FLAG_FUNC_8BIT |
                   FLAG_FUNC_MULT_LINES |
                   FLAG_FUNC_5X8_FONT);

    // Turn display off
    sendLcdCommand(CMD_DISP_CTRL);

    // Clear display
    sendLcdCommand(CMD_CLEAR_DISP);

    // Set desired entry mode
    sendLcdCommand(CMD_ENTRY_MODE |
                   FLAG_ENTRY_MODE_INC);

    // Turn display on, with desired cursor mode
    sendLcdCommand(CMD_DISP_CTRL |
                   FLAG_DISP_CTRL_ON | FLAG_DISP_CTRL_CURSOR);

    // Set DDRAM address to 0
    sendLcdCommand(CMD_SET_DDRAM_ADDR);

    // Thats it!
}

void Lcd_clear(void)
{
    sendLcdCommand(CMD_CLEAR_DISP);
}

void Lcd_setCursor(uint8_t row, uint8_t column)
{
    uint8_t addr;

    if ((row >= 4u) || (column >= 20u))
    {
        Err_raise(ERR_MODULE_LCD, __LINE__);
    }
    else
    {
        // The row address must be as follows:
        // Row 0 : 0
        // Row 1 : 64
        // Row 2 : 20
        // Row 3 : 84
        addr = (row & 1u) ? 64u : 0u;
        addr += (row & 2u) ? 20u : 0u;

        // The column is simply added
        addr += column;

        // Set DDRAM address to 0
        sendLcdCommand(CMD_SET_DDRAM_ADDR + addr);
    }
}

void Lcd_write(uint8_t c)
{
    sendLcdData(c);
}


void Lcd_writeRepeat(uint8_t c, uint8_t repeat)
{
    uint8_t i;

    for (i = 0; i < repeat; i++)
    {
        sendLcdData(c);
    }
}


uint8_t Lcd_writeString(char const *s)
{
    uint8_t data;
    uint8_t l = 0;

    while ((data = (uint8_t)(*s)) != 0u)
    {
        sendLcdData(data);
        s++;
        l++;
    }

    return l;
}

uint8_t Lcd_writeString_P(char const *s)
{
    uint8_t data;
    uint8_t l = 0;

    while ((data = (uint8_t)(pgm_read_byte(s))) != 0u)
    {
        sendLcdData(data);
        s++;
        l++;
    }

    return l;
}

