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

#define CMD_INITIALIZE  0b00110000

// LCD command definitions

#define CMD_CLEAR_DISP        0b00000001

#define CMD_RETURN_HOME       0b00000010

#define CMD_ENTRY_MODE        0b00000100
#define FLAG_ENTRY_MODE_INC   0b00000010
#define FLAG_ENTRY_MODE_SHIFT 0b00000001

#define CMD_DISP_CTRL         0b00001000
#define FLAG_DISP_CTRL_ON     0b00000100
#define FLAG_DISP_CTRL_CURSOR 0b00000010
#define FLAG_DISP_CTRL_BLINK  0b00000001

#define CMD_SHIFT_CURSOR      0b00010000
#define CMD_SHIFT_DISPLAY     0b00011000
#define FLAG_SHIFT_RIGHT      0b00000100
#define FLAG_SHIFT_LEFT       0b00000000

#define CMD_FUNC_SET          0b00100000
#define FLAG_FUNC_8BIT        0b00010000
#define FLAG_FUNC_MULT_LINES  0b00001000
#define FLAG_FUNC_5X8_FONT    0b00000100

#define CMD_SET_CGRAM_ADDR    0b01000000
#define CMD_SET_DDRAM_ADDR    0b10000000


// Busy status defines
#define FLAG_BUSY             0b10000000



/////////////////////////   Variables    /////////////////////////

static bool_t lcdReady = FALSE;


/////////////////////////   Prototypes   /////////////////////////


// Low level LCD IO functions
static void SendLcdCommand(uint8_t data);
static void SendLcdData(uint8_t data);

static uint8_t ReadLcdStatus(void);
//static uint8_t ReadLcdData(void);

static bool_t lcd_CheckReady();

//////////////////////// Private functions ////////////////////////


static void SendLcdCommand(uint8_t data)
{
    // Wait for Lcd to be ready
    while (!lcd_CheckReady())
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
    lcdReady = FALSE;
}


static void SendLcdData(uint8_t data)
{
    // Wait for Lcd to be ready
    while (!lcd_CheckReady())
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
    lcdReady = FALSE;
}


static uint8_t ReadLcdStatus(void)
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
static bool_t lcd_CheckReady()
{
    if (lcdReady == FALSE)
    {
        if ((ReadLcdStatus() & FLAG_BUSY) == 0u)
        {
            lcdReady = TRUE;
        }
    }

    return lcdReady;
}


//////////////////////// Public functions ////////////////////////

void lcd_Initialize(void)
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
    lcdReady = TRUE;
    SendLcdCommand(CMD_INITIALIZE);
    _delay_ms(6);

    // Send initialize command and wait at least 100 us extra
    // Bypass lcd busy checking at this point
    lcdReady = TRUE;
    SendLcdCommand(CMD_INITIALIZE);
    _delay_ms(1);

    // Send initialize command one final time
    // Bypass ready checking at this point
    lcdReady = TRUE;
    SendLcdCommand(CMD_INITIALIZE);

    // Now we can use ready checking by the busy flag

    // Send function set command with configuration
    SendLcdCommand(CMD_FUNC_SET |
                   FLAG_FUNC_8BIT |
                   FLAG_FUNC_MULT_LINES |
                   FLAG_FUNC_5X8_FONT);

    // Turn display off
    SendLcdCommand(CMD_DISP_CTRL);

    // Clear display
    SendLcdCommand(CMD_CLEAR_DISP);

    // Set desired entry mode
    SendLcdCommand(CMD_ENTRY_MODE |
                   FLAG_ENTRY_MODE_INC);

    // Turn display on, with desired cursor mode
    SendLcdCommand(CMD_DISP_CTRL |
                   FLAG_DISP_CTRL_ON | FLAG_DISP_CTRL_CURSOR);

    // Set DDRAM address to 0
    SendLcdCommand(CMD_SET_DDRAM_ADDR);

    // Thats it!
}

void lcd_Clear(void)
{
    SendLcdCommand(CMD_CLEAR_DISP);
}

void lcd_CursorSet(uint8_t row, uint8_t column)
{
    uint8_t addr;

    if ((row >= 4u) || (column >= 20u))
    {
        err_Raise(ERR_MODULE_LCD, __LINE__);
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
        SendLcdCommand(CMD_SET_DDRAM_ADDR + addr);
    }
}

void lcd_Write(uint8_t c)
{
    SendLcdData(c);
}


void lcd_WriteRepeat(uint8_t c, uint8_t repeat)
{
    uint8_t i;

    for (i = 0; i < repeat; i++)
    {
        SendLcdData(c);
    }
}


uint8_t lcd_WriteString(char const *s)
{
    uint8_t data;
    uint8_t l = 0;

    while ((data = (uint8_t)(*s)) != 0u)
    {
        SendLcdData(data);
        s++;
        l++;
    }

    return l;
}

uint8_t lcd_WriteString_P(char const *s)
{
    uint8_t data;
    uint8_t l = 0;

    while ((data = (uint8_t)(pgm_read_byte(s))) != 0u)
    {
        SendLcdData(data);
        s++;
        l++;
    }

    return l;
}

