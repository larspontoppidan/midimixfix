/*
 * stringutil.c
 *
 *  Created on: Dec 21, 2012
 *      Author: lars
 */

#include "common.h"
#include "util.h"
#include "pgmstrings.h"

int16_t sutil_TenthsTab[5] = {10000, 1000, 100, 10, 1};

char *util_strWriteInt16(char *dest, int16_t value)
{
    uint8_t p;
    uint8_t c;

    bool_t numbernow = FALSE;

    if (value < 0)
    {
        *(dest++) = '-';
        value = -value;
    }
    else
    {
        *(dest++) = ' ';
    }

    for (p = 0; p < 5; p++)
    {
        if (value < sutil_TenthsTab[p])
        {
            if (numbernow || (p == 4))
            {
                *(dest++) = '0';
            }
            else
            {
                *(dest++) = ' ';
            }
        }
        else
        {
            c = '0';

            while (value >= sutil_TenthsTab[p])
            {
                c++;
                value -= sutil_TenthsTab[p];
            }

            *(dest++) = c;

            numbernow = 1;
        }
    }

    return dest;
}


char *util_strWriteInt16LA(char *dest, int16_t value)
{
    uint8_t p;
    uint8_t c;

    bool_t numbernow = FALSE;

    if (value < 0)
    {
        *(dest++) = '-';
        value = -value;
    }

    for (p = 0; p < 5; p++)
    {
        if (value < sutil_TenthsTab[p])
        {
            if (numbernow || (p == 4))
            {
                *(dest++) = '0';
            }
        }
        else
        {
            c = '0';

            while (value >= sutil_TenthsTab[p])
            {
                c++;
                value -= sutil_TenthsTab[p];
            }

            *(dest++) = c;

            numbernow = 1;
        }
    }

    return dest;
}


char * util_strWriteUint8(char *dest, uint8_t value)
{
    uint8_t c;
    bool_t numbernow = FALSE;

    c = 0u;
    while (value >= 100u)
    {
        c++;
        value -= 100u;
    }

    if (c != 0u)
    {
        *(dest++) = c + '0';
        numbernow = TRUE;
    }
    else
    {
        *(dest++) = ' ';
    }

    c = 0u;
    while (value >= 10u)
    {
        c++;
        value -= 10u;
    }

    if (numbernow || (c != 0u))
    {
        *(dest++) = c + '0';
    }
    else
    {
        *(dest++) = ' ';
    }

    *(dest++) = value + '0';

    return dest;
}


char *util_strWriteInt8LA(char *dest, int8_t x)
{
    uint8_t value;
    uint8_t c;

    bool_t numbernow = FALSE;

    if (x < 0)
    {
        *(dest++) = '-';
        value = (uint8_t)(-x);
    }
    else
    {
        value = (uint8_t)x;
    }

    c = 0u;
    while (value >= 100u)
    {
        c++;
        value -= 100u;
    }

    if (c != 0u)
    {
        *(dest++) = c + '0';
        numbernow = TRUE;
    }

    c = 0u;
    while (value >= 10u)
    {
        c++;
        value -= 10u;
    }

    if (numbernow || (c != 0u))
    {
        *(dest++) = c + '0';
    }

    *(dest++) = value + '0';

    return dest;
}


char *util_strWriteHex(char *dest, uint8_t x)
{
    uint8_t c;

    c = (x >> 4);

    if (c <= 9)
    {
        *(dest++) = '0' + c;
    }
    else
    {
        *(dest++) = ('A' - 10) + c;
    }

    c = x & 15;

    if (c <= 9)
    {
        *(dest++) = '0' + c;
    }
    else
    {
        *(dest++) = ('A' - 10) + c;
    }

    return dest;
}


char *util_strCpy_P(char *dest, PGM_P src)
{
    char c;

    while(1)
    {
        c = pgm_read_byte(src);

        if (c == 0u)
            break;

        *(dest++) = c;
        src++;
    }

    return dest;
}

int8_t util_boundedAddInt8(int8_t value, int8_t min, int8_t max, int8_t add)
{
    // Work in 16 bit signed space to avoid rollover
    int16_t x;

    x = (int16_t)value + (int16_t)add;

    if (x < (int16_t)min)
    {
        x = min;
    }
    else if (x > (int16_t)max)
    {
        x = max;
    }

    return (int8_t)x;
}


// Specialized function for writing right aligned component status
//
// Writes the following to dest[0] to dest[4]:
// "(OFF)"   value = 0
// "  (1)"   value = 1
// "  (2)"   value = 2
// " (10)"   value = 10
// "(200)"   value = 200

void util_strWriteNumberParentheses(char *dest, uint8_t value)
{
    if (value == 0)
    {
        util_strCpy_P(dest, pstr_OffParentheses);
    }
    else
    {
        if (value < 10)
        {
            // One digit
            dest[0] = ' ';
            dest[1] = ' ';
            dest[2] = '(';
            dest[3] = value + '0';
        }
        else if (value < 100)
        {
            // Two digits
            dest[0] = ' ';
            dest[1] = '(';
            util_strWriteInt8LA(&(dest[2]), value);
        }
        else
        {
            // Three digits
            dest[0] = '(';
            util_strWriteInt16LA(&(dest[1]), (int16_t)value);
        }

        dest[4] = ')';
    }

}


/*
 *
void lcd_WriteBin(uint8_t x)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if ((x & 0x80u) != 0u)
        {
            lcd_Write('1');
        }
        else
        {
            lcd_Write('0');
        }

        x = x * 2u;
    }
}

 */
