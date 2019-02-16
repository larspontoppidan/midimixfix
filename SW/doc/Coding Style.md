


Larsp's Favorite C Coding Style
===============================

Author:   Lars Ole Pontoppidan
Date:     2013-05-03
Version:  1.0


Introduction
------------

This document defines guidelines for formatting, naming and for good practices writing C source code. Following the guideline should result in a consistent, easily readable and maintainable code base with a reduced likelihood of bugs compared to not using a consistent coding style. The intended use is for small to medium scale microcontroller based projects.

The guideline is primarily intended to serve as tool for the programmer, assisting in making  good naming and design decisions quickly. It is not intented to be a very detailed and strict specification that requires a lot of effort to follow. 

To achieve this, the guideline is kept short and simple. If, however, requirements are in the way of achieving the most readable and highest quality code, it may be acceptable to deviate from specific requirements on a limited basis.


Language compliance
-------------------

Good code compiles cleanly using current and future versions of compilers and is as portable as possible. The language compliance rules try to achieve this by discouraging the use of obsolete and nonstandard constructs and by configuring the compiler to help checking the code for problems as much as possible.

1. The source code must compile with no warnings and no errors, even on the highest warning level of the compiler (options -Wall -Wextra -Wextra -pedantic for gcc). The -Werror option is recommended because modules  warnings often disappear on subsequent builds that doesn't recompile all code modules, which can lead to ignored warnings.

2. The source code may use ISO-C99 features when it benefits code quality. Rarely used C99 features must be avoided, in particular those not supported by the gcc compiler.

3. Specific compiler language features such as the "GNU-ISMs", must be avoided.

4. Ancient C language constructs such as digraphs, trigraphs and implicit function declarations must be avoided.


Indentation and curly brackets
------------------------------

The indentation and bracket style defined here focus on readability over code density, and is inspired by modern curly bracket languages such as C# and Java, more than by traditional C dogma. 

5. Tab characters must never be used. They are not necessary and only add potential problems.

6. Four spaces must be used for indentation.

7. Blocks are written with curly brackets on seperate lines and both on same indentation level as the preceeding control statement. 

8. Statements in blocks must be indented by the four spaces of indentation.

9. Always use blocks after control statements. Single statements without the curly brackets block must be avoided. 

10. Lines must not exceed 100 characters. Consider breaking lines that exceed 80 characters if it increases readability. Indent after the line break such that  statements are aligned and readability is maximized.

11. Apply white space to make the code easy to read and comprehend. Align variables, declarations. Make space between operators. Don't make space between every single line of code. Remember, no tabs.




See "Appendix B. Source file example" for an example.


Integer types
-------------

Writing software close to the hardware on resource limited microcontrollers requires tight control of the data types used.

11. Explicit bit-width integer types must be used instead of the built-in types in C language. For example, use "uint8_t" and "int16_t" from "stdint.h" instead of "unsigned char" and "int". 

    - Exception: The "char" type may be preferred for character data.

12. A custom "bool_t" type alias must be used for boolean variables, to clearly communicate the intension of the variable. An example definition suitable for 8-bit microcontrollers could be: "typedef uint8_t bool_t;"

13. Boolean truth assignments must use TRUE and FALSE defines with the value 1 and 0.

14. When testing boolean variables, respect the C convention of regarding all non-zero values as true. Never test on equality with TRUE. 


Code modules and files
----------------------

15. All code must be partitioned in code modules. A code module is a source file and a header file with the same name except for the ending, which must be ".c" and ".h". There are exceptions where it doesn't make sense to have 1:1 correspondance between source and header files.

16. Use all lower case for filenames. 

17. Encapsulation. Code modules must solve a single task or have a single purpose. They must publicly expose only a minimal and easy to understand interface. Functions and variables that are not required to be public must be private.

18. A header file defines publicly exposed declarations for the code module, typically public functions.

19. Only publicly exposed variables should use the "extern" keyword. Not functions.

20. All header files must have an include guard similar to: "#ifdef SOME_HEADER_H ... #endif". See Appendix A for a suggested syntax.

21. Generally avoid exposing variables as public. Prefer GetX and SetX functions instead. This is good practice because it allows the getters and setters to perform dataprocessing or syncronisation tasks before accessing the value.


22. A source file must be composed with the following sections:

    - A. File header
    - B. Header file includes
    - C. Defines. (Preprocessor defines, const variables, enums, typedefs)
    - D. Private variables
    - E. Prototypes for private functions
    - F. Implementation of private functions
    - G. Implementation of public functions

23. Never include ".c" files

24. Make a code module dependant on as few other header files as possible.

25. Private variables MUST be declared static. This is necessary to make the private variables private and to avoid name collisions between code modules.

26. Private functions MUST be declared static. This is necessary to make the private functions private and to avoid name collisions between code modules.


Casing and prefixes
-------------------

This casing and prefix style is intended be practical and easy to use, while conveying information about scope and type of declarations. The style attempts to be roughly compatible with C library declarations. 

Preprocessor macros:
27.   Module scope:    UPPER_CASE
28.   Public:          MODNAME_UPPER_CASE
   
Type definitions:
29.   Module scope:    lowerCamelCase_t
30.   Public:          modname_lowerCamelCase_t
   
Function definitions:
31.   Module scope:    lowerCamelCase
32.   Public:          modname_lowerCamelCase
      
Variables, arrays, struct instances:
33.   Function scope:  lower_case
34.   Module scope:    UpperCamelCase
35.   Struct members:  UpperCamelCase
36.   Public:          modname_UpperCamelCase

Declarations may be decorated with further suffixes, such as _ISR or _P etc.

See Appendixes for examples of the casing and prefix style.

HUSK: Function pointer bør bruge typedef
HUSK: Structs bør bruge typedef
HUSK: Static variable inden i funktioner er DUMT
HUSK: Extern er ikke nodvendig i h fil (default man ikke bruger static)
HUSK: Prefer single return statement
HUSK: Avoid magic constats

Selection of names
------------------

37. Function names must describe an action and include a verb.

38. Variable names must nouns.

39. Use plural form for array names.

    - Example: "static curveFilter_t CurveFilters[CURVE_FILTER_COUNT];
    - Example: "static uint8_t QueueElements[STATUS_BUFFER_SIZE];"

40. Avoid using abbreviated words except for very common abbreviations.

41. Names must not exceed 32 characters. Find a shorter name or consider common word abbreviations.

42. Avrgcc specific: Functions working with program memory locations must be suffixed with _P.

43. Good practice: Function names must end with a suffix according to the scope they are allowed to be called from:

    - Only main thread: _MAIN
    - Only ISR sources: _ISR
    - Both ISR and main thread calls: _SAFE 
    - Not applicable to define scope: no suffix


Comments
--------

44. Don't comment the obvious. Comment the intention of the code. If heaps of comments are required to explain some code, prefer to find a better implementation.

45. Use // for comments. Single and multi line comments using /* */ must be avoided.

46. Function comments: Simple and obvious functions doesn't need comments. Functions that are not obvious should be sufficiently described for new readers to understand purpose and usage. Use for example the function comment style demonstated in Appendix.

47. Public functions should be  in the header file must be commented only in the header file.

48. Private functions must be commented in the source file.

48. The header file must include a description on the purpose and how to use the code module.

49. The comment for functions must describe purpose, input values and return values. 

50. Use for example the commenting style used in Appendix A and B.


See Appendix A and B for a suggested white space style.


Various good coding practices
-----------------------------

53. All microcontroller specifics, such as SFR (special function register) accesses, ISR (interrupt service routinges) should be performed in a hal code module. The hal module should only perform a simple abstraction, or limited lowlevel processing.

54. Remember to use volatile for variables accessed by both ISR and main thread functions. Use necessary means to avoid race conditions, deadlocks etc.

55. Use const on function parameters if they are pointers, and the function doesn't touch the data that is pointed on.

56. Avoid using function scope static variables. It hides code module state for the reader. Use a file scope private variable instead.

57. When defining function pointers, first define the function pointer type with a typedef and a telling name. This will hugely increase readability.

58. Do bounds checking of array indexes when writing in arrays. If possible, verify integrity of pointers before writing to them.

59. Prefer inline functions over preprocessor macros.


APPENDIX A. Header file example
-------------------------------



APPENDIX B. Source file example
-------------------------------

//
// Filename    : codestyledemo.c
// Code module : Code style demonstration module
// Project     : Midimixfix
// URL         : http://larsee.dk
//
// --------------------------------  LICENSE  -----------------------------------
//
// Copyright (c) 2013, Lars Ole Pontoppidan (Larsp)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// *  Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// *  Neither the name of the original author (Lars Ole Pontoppidan) nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
// ------------------------------  DESCRIPTION  ---------------------------------
//
// This code module is not really a code module that makes any sense, but it 
// demonstrates how to apply Larsp's Favorite C Coding Style (LFCCS).
//
// Implements:
// *  This
// *  That
// *  But not that
//


// -------------------------------  INCLUDES  -----------------------------------

#include "common.h"
#include "codestyledemo.h"
#include <string.h>

// --------------------------  TYPES AND CONSTANTS  -----------------------------

typedef struct
{
    uint8_t midiStatus;
    uint8_t midiParam[2];
} message_t;

#define MESSAGES_MAX 20

// Menu system defines
#define EDIT_MODE_ROOT    0
#define EDIT_MODE_CHANNEL 1


// ----------------------------  LOCAL VARIABLES  -------------------------------

static uint8_t   messageCount;
static message_t messages[MESSAGES_MAX];

static char titleString[] PROGMEM = "Generate msg. ";


// ------------------------------  PROTOTYPES  ----------------------------------


static void ResetMessage(uint8_t msg);
static void EditChannel(uint8_t msg, int8_t delta);



// ---------------------------  PRIVATE FUNCTIONS  ------------------------------


static void ResetMessage(uint8_t msg)
{
    // Default to program change on channel 1 patch 1
    Messages[msg].MidiStatus = MIDI_STATUS_PROG_CHANGE;
    Messages[msg].MidiParam[0] = 0;
}

static void EditChannel(uint8_t msg, int8_t delta)
{
    uint8_t chan;

    // Modify lower nibble of midi status with delta:

    chan = Messages[msg].MidiStatus & MIDI_CHANNEL_MASK;
    chan = util_boundedAddInt8(chan, 0, 15, delta);
    Messages[msg].MidiStatus = (Messages[msg].MidiStatus & (~MIDI_CHANNEL_MASK)) | chan;
}


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void cstyle_initialize(void)
{
    uint8_t i;

    MessageCount = 0;

    for (i = 0; i < MESSAGES_MAX; i++)
    {
        ResetMessage(i);
    }
}

uint8_t genmsg_menuGetSubCount(void)
{
    return MessageCount;
}

void genmsg_menuGetText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        util_strCpy_P(dest, TitleString);
        util_strWriteNumberParentheses(&(dest[14]), MessageCount);
    }
    else
    {
        // Write the decoded message. Move from message to mmsg_t
        mmsg_t msg;
        msg.flags = MMSG_FLAG_MSG_OK;
        msg.midi_status = Messages[item-1].MidiStatus;
        msg.midi_data[0] = Messages[item-1].MidiParam[0];
        msg.midi_data[1] = Messages[item-1].MidiParam[1];
        msg.midi_data[2] = Messages[item-1].MidiParam[2];
        msg.midi_data_len = mmsg_dataCountGet(msg.midi_status);

        mmsg_WriteMsgParsed(dest, &msg);
    }
}



// fooBar is intended to demonstrate coding style and not much else.
//
// Beware that the code may become sentient and take over your computer.
//
// Parameters:     
//    bar:                   This is where you buy beer
//    a_long_variable_name:  Not used
//    a_similar_variable:    Not used either
//    
// Return value:    The fooBar calculated
//   
uint8_t fooBar(uint8_t bar, uint32_t a_long_variable_name, 
                            uint32_t a_similar_variable)
{
    UNUSED(a_long_variable_name);
    UNUSED(another_long_similar_variable);

    uint16_t i = 0;
    uint8_t ret = 0;    

    // The following statement is genius    
    if (bar == SPECIAL_NUMBER)
    {
        uint8_t j;
        
        //  The following loop is pure nonsense  
        for (j = 2; j < 42; j++)
        {
            switch (i)
            {
            case 0:
                i = 4 + (2 * 2);
                break;
                
            case 1:
                // Do nothing because this of this or that
                break;
            
            default:
                if (checkOtherStuff(i * j))
                {
                    ret = j;
                }
            }
        }
    }
    else if (bar == ANOTHER_NUMBER)
    {
        ret = bar;
    }
    else
    {
        // Don't do anything because of this or that.
    }
    
    return ret;        
}

