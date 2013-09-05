/*
 * pedals.c
 *
 *  Created on: May 11, 2013
 *      Author: lars
 */

#include "common.h"
#include "hal.h"
#include "midiio.h"
#include "midimessage.h"
#include "midigenerics.h"
#include "menu.h"
#include "util.h"
#include <avr/pgmspace.h>
#include <string.h>

// DATA PROCESSING
//
// A variable pedal is sampled by midimixfix using a voltage divider into a 10-bit ADC:
//
//   VCC ---+
//          |
//         10k
//          |
//          +----  ADC
//          |
//       R_pedal
//          |
//   GND ---+
//
// The circuit maps the pedal resistance non-linearly to the voltage seen by the ADC:
//
//   V_ADC = VCC * R_pedal / (10k + R_pedal)
//
// If we want to derive the actual R_pedal value without the non-linearity, we need to find:
//
//   R_pedal = 10 * V_ADC / (VCC - V_ADC)
//
// To calculate R_pedal using above formula, a look up table is used. The table simply has
// one 16-bit entry for each possible ADC value, and is scaled to have unit 1/64 ohms.
// The scaling is selected to enable a convenient gain calculation followed by >> 8.
//
// Python to generate the table:
//
// from numpy import *
//
// a = range(1024)
//
// for i in xrange(1000):
//   a[i] = int(round(10000.0 / 64.0 * (i/1024.0) / (1.0-(i/1024.0))))
//
// array(a).reshape(-1,8)


#define MODE_ENABLED       0x01
#define MODE_DIGITAL       0x02
#define MODE_INVERTED      0x04

typedef struct
{
    uint8_t Mode;
    uint8_t Gain;
    int8_t  Offset;
    uint8_t Controller;
    uint8_t Channel;
} pedalSetup_t;

typedef struct
{
    uint8_t State; // The state of the digital / analog processing
    uint8_t Value; // The latest output value for this pedal
} pedalState_t;


#define DIGITAL_HIGH_TRIGGER  683u  // 2/3 of full ADC gain
#define DIGITAL_LOW_TRIGGER   341u  // 1/3 of full ADC gain

#define ANALOG_STICKY 1

#define INVERT_TABLE_MAX 1024

pedalSetup_t PedalSetup[2];
pedalState_t PedalState[2];

static uint16_t const InvertTable[INVERT_TABLE_MAX] PROGMEM =
{
       0u,   0u,   0u,   0u,   1u,   1u,   1u,   1u,
       1u,   1u,   2u,   2u,   2u,   2u,   2u,   2u,
       2u,   3u,   3u,   3u,   3u,   3u,   3u,   4u,
       4u,   4u,   4u,   4u,   4u,   5u,   5u,   5u,
       5u,   5u,   5u,   6u,   6u,   6u,   6u,   6u,
       6u,   7u,   7u,   7u,   7u,   7u,   7u,   8u,
       8u,   8u,   8u,   8u,   8u,   9u,   9u,   9u,
       9u,   9u,   9u,  10u,  10u,  10u,  10u,  10u,
      10u,  11u,  11u,  11u,  11u,  11u,  11u,  12u,
      12u,  12u,  12u,  12u,  13u,  13u,  13u,  13u,
      13u,  13u,  14u,  14u,  14u,  14u,  14u,  15u,
      15u,  15u,  15u,  15u,  15u,  16u,  16u,  16u,
      16u,  16u,  17u,  17u,  17u,  17u,  17u,  17u,
      18u,  18u,  18u,  18u,  18u,  19u,  19u,  19u,
      19u,  19u,  20u,  20u,  20u,  20u,  20u,  21u,
      21u,  21u,  21u,  21u,  22u,  22u,  22u,  22u,
      22u,  23u,  23u,  23u,  23u,  23u,  24u,  24u,
      24u,  24u,  24u,  25u,  25u,  25u,  25u,  25u,
      26u,  26u,  26u,  26u,  26u,  27u,  27u,  27u,
      27u,  27u,  28u,  28u,  28u,  28u,  29u,  29u,
      29u,  29u,  29u,  30u,  30u,  30u,  30u,  30u,
      31u,  31u,  31u,  31u,  32u,  32u,  32u,  32u,
      32u,  33u,  33u,  33u,  33u,  34u,  34u,  34u,
      34u,  34u,  35u,  35u,  35u,  35u,  36u,  36u,
      36u,  36u,  37u,  37u,  37u,  37u,  37u,  38u,
      38u,  38u,  38u,  39u,  39u,  39u,  39u,  40u,
      40u,  40u,  40u,  41u,  41u,  41u,  41u,  42u,
      42u,  42u,  42u,  43u,  43u,  43u,  43u,  44u,
      44u,  44u,  44u,  45u,  45u,  45u,  45u,  46u,
      46u,  46u,  46u,  47u,  47u,  47u,  47u,  48u,
      48u,  48u,  48u,  49u,  49u,  49u,  49u,  50u,
      50u,  50u,  50u,  51u,  51u,  51u,  52u,  52u,
      52u,  52u,  53u,  53u,  53u,  53u,  54u,  54u,
      54u,  55u,  55u,  55u,  55u,  56u,  56u,  56u,
      57u,  57u,  57u,  57u,  58u,  58u,  58u,  59u,
      59u,  59u,  59u,  60u,  60u,  60u,  61u,  61u,
      61u,  61u,  62u,  62u,  62u,  63u,  63u,  63u,
      64u,  64u,  64u,  64u,  65u,  65u,  65u,  66u,
      66u,  66u,  67u,  67u,  67u,  68u,  68u,  68u,
      68u,  69u,  69u,  69u,  70u,  70u,  70u,  71u,
      71u,  71u,  72u,  72u,  72u,  73u,  73u,  73u,
      74u,  74u,  74u,  75u,  75u,  75u,  76u,  76u,
      76u,  77u,  77u,  77u,  78u,  78u,  78u,  79u,
      79u,  79u,  80u,  80u,  80u,  81u,  81u,  81u,
      82u,  82u,  83u,  83u,  83u,  84u,  84u,  84u,
      85u,  85u,  85u,  86u,  86u,  87u,  87u,  87u,
      88u,  88u,  88u,  89u,  89u,  90u,  90u,  90u,
      91u,  91u,  91u,  92u,  92u,  93u,  93u,  93u,
      94u,  94u,  95u,  95u,  95u,  96u,  96u,  97u,
      97u,  97u,  98u,  98u,  99u,  99u,  99u, 100u,
     100u, 101u, 101u, 101u, 102u, 102u, 103u, 103u,
     103u, 104u, 104u, 105u, 105u, 106u, 106u, 106u,
     107u, 107u, 108u, 108u, 109u, 109u, 110u, 110u,
     110u, 111u, 111u, 112u, 112u, 113u, 113u, 114u,
     114u, 114u, 115u, 115u, 116u, 116u, 117u, 117u,
     118u, 118u, 119u, 119u, 120u, 120u, 121u, 121u,
     122u, 122u, 122u, 123u, 123u, 124u, 124u, 125u,
     125u, 126u, 126u, 127u, 127u, 128u, 128u, 129u,
     129u, 130u, 130u, 131u, 132u, 132u, 133u, 133u,
     134u, 134u, 135u, 135u, 136u, 136u, 137u, 137u,
     138u, 138u, 139u, 139u, 140u, 141u, 141u, 142u,
     142u, 143u, 143u, 144u, 145u, 145u, 146u, 146u,
     147u, 147u, 148u, 149u, 149u, 150u, 150u, 151u,
     151u, 152u, 153u, 153u, 154u, 154u, 155u, 156u,
     156u,    157u,    157u,    158u,    159u,    159u,    160u,    161u,
     161u,    162u,    162u,    163u,    164u,    164u,    165u,    166u,
     166u,    167u,    168u,    168u,    169u,    170u,    170u,    171u,
     172u,    172u,    173u,    174u,    174u,    175u,    176u,    176u,
     177u,    178u,    178u,    179u,    180u,    181u,    181u,    182u,
     183u,    183u,    184u,    185u,    186u,    186u,    187u,    188u,
     189u,    189u,    190u,    191u,    192u,    192u,    193u,    194u,
     195u,    195u,    196u,    197u,    198u,    199u,    199u,    200u,
     201u,    202u,    202u,    203u,    204u,    205u,    206u,    207u,
     207u,    208u,    209u,    210u,    211u,    212u,    212u,    213u,
     214u,    215u,    216u,    217u,    218u,    218u,    219u,    220u,
     221u,    222u,    223u,    224u,    225u,    226u,    227u,    227u,
     228u,    229u,    230u,    231u,    232u,    233u,    234u,    235u,
     236u,    237u,    238u,    239u,    240u,    241u,    242u,    243u,
     244u,    245u,    246u,    247u,    248u,    249u,    250u,    251u,
     252u,    253u,    254u,    255u,    256u,    257u,    258u,    259u,
     260u,    262u,    263u,    264u,    265u,    266u,    267u,    268u,
     269u,    270u,    272u,    273u,    274u,    275u,    276u,    277u,
     279u,    280u,    281u,    282u,    283u,    285u,    286u,    287u,
     288u,    289u,    291u,    292u,    293u,    294u,    296u,    297u,
     298u,    300u,    301u,    302u,    304u,    305u,    306u,    308u,
     309u,    310u,    312u,    313u,    314u,    316u,    317u,    319u,
     320u,    321u,    323u,    324u,    326u,    327u,    329u,    330u,
     332u,    333u,    335u,    336u,    338u,    339u,    341u,    342u,
     344u,    345u,    347u,    348u,    350u,    352u,    353u,    355u,
     357u,    358u,    360u,    362u,    363u,    365u,    367u,    368u,
     370u,    372u,    374u,    375u,    377u,    379u,    381u,    382u,
     384u,    386u,    388u,    390u,    392u,    394u,    395u,    397u,
     399u,    401u,    403u,    405u,    407u,    409u,    411u,    413u,
     415u,    417u,    419u,    421u,    423u,    426u,    428u,    430u,
     432u,    434u,    436u,    439u,    441u,    443u,    445u,    448u,
     450u,    452u,    454u,    457u,    459u,    462u,    464u,    466u,
     469u,    471u,    474u,    476u,    479u,    481u,    484u,    486u,
     489u,    492u,    494u,    497u,    499u,    502u,    505u,    508u,
     510u,    513u,    516u,    519u,    522u,    525u,    528u,    530u,
     533u,    536u,    539u,    542u,    546u,    549u,    552u,    555u,
     558u,    561u,    564u,    568u,    571u,    574u,    578u,    581u,
     584u,    588u,    591u,    595u,    598u,    602u,    606u,    609u,
     613u,    617u,    620u,    624u,    628u,    632u,    636u,    640u,
     644u,    648u,    652u,    656u,    660u,    664u,    668u,    673u,
     677u,    681u,    686u,    690u,    695u,    699u,    704u,    709u,
     713u,    718u,    723u,    728u,    733u,    738u,    743u,    748u,
     753u,    758u,    763u,    769u,    774u,    779u,    785u,    790u,
     796u,    802u,    808u,    813u,    819u,    825u,    831u,    838u,
     844u,    850u,    856u,    863u,    869u,    876u,    883u,    890u,
     896u,    903u,    910u,    918u,    925u,    932u,    940u,    947u,
     955u,    963u,    971u,    979u,    987u,    995u,   1003u,   1012u,
    1020u,   1029u,   1038u,   1047u,   1056u,   1065u,   1075u,   1084u,
    1094u,   1104u,   1114u,   1124u,   1134u,   1145u,   1155u,   1166u,
    1177u,   1188u,   1200u,   1211u,   1223u,   1235u,   1247u,   1260u,
    1272u,   1285u,   1298u,   1312u,   1325u,   1339u,   1353u,   1368u,
    1382u,   1397u,   1412u,   1428u,   1444u,   1460u,   1476u,   1493u,
    1510u,   1528u,   1546u,   1564u,   1583u,   1602u,   1622u,   1642u,
    1662u,   1683u,   1704u,   1726u,   1749u,   1771u,   1795u,   1819u,
    1844u,   1869u,   1895u,   1922u,   1949u,   1977u,   2006u,   2036u,
    2066u,   2097u,   2129u,   2163u,   2197u,   2232u,   2268u,   2305u,
    2344u,   2383u,   2424u,   2467u,   2510u,   2556u,   2602u,   2651u,
    2701u,   2753u,   2807u,   2863u,   2921u,   2981u,   3044u,   3109u,
    3177u,   3248u,   3322u,   3399u,   3480u,   3565u,   3653u,   3746u,
    3844u,   3946u,   4054u,   4168u,   4288u,   4415u,   4550u,   4692u,
    4844u,   5005u,   5177u,   5361u,   5558u,   5770u,   5998u,   6244u,
    6510u,   6800u,   7116u,   7463u,   7844u,   8265u,   8733u,   9256u,
    9844u,  10510u,  11272u,  12151u,  13177u,  14389u,  15844u,  17622u,
   19844u,  22701u,  26510u,  31844u,  39844u,  53177u,  65535u,  65535u
};



static uint8_t ProcessAdcValue(int8_t offset, uint8_t gain, uint16_t adc_value);



static uint8_t ProcessAdcValue(int8_t offset, uint8_t gain, uint16_t adc_value)
{
    uint16_t x;

    // First look up adc_value in invert table:
    if (adc_value < INVERT_TABLE_MAX)
    {
        x = pgm_read_word(&(InvertTable[adc_value]));
    }
    else
    {
        x = 65535u;
    }

    // Perform gain calculation in 32 bit domain:

    //x = (uint16_t) (((uint32_t)x * (uint32_t)gain) >> 13);   // 11.52 us
    x = (uint16_t) (((uint32_t)x * (uint32_t)gain) >> 8);      // 7.24 us for entire function

    // Add offset in 16 bit signed domain:
    int16_t s;

    s = ((int16_t)x) + (int16_t)(offset);

    // Truncate to 0:127 and we are done
    if (s < 0)
    {
        return 0;
    }
    else if (s > 127)
    {
        return 127;
    }
    else
    {
        return (uint8_t)s;
    }
}


void pedals_initialize(void)
{
    memset(&PedalSetup, 0, sizeof(pedalSetup_t) * 2);
}

// The digital pedal processing includes a schmitt triggering function of the ADC value
static uint8_t handleDigitalPedal_ISR(uint8_t p)
{
    uint16_t value = hal_adcGetValue_ISR(p);

    // Implement 1/3, 2/3 schmitt trigger digital mode
    if (PedalState[p].State < 64)
    {
        PedalState[p].State = (value < DIGITAL_HIGH_TRIGGER) ? 0 : 127;
    }
    else
    {
        PedalState[p].State = (value < DIGITAL_LOW_TRIGGER) ? 0 : 127;
    }

    if (PedalSetup[p].Mode & MODE_INVERTED)
    {
        return 127 - PedalState[p].State;
    }
    else
    {
        return PedalState[p].State;
    }
}

// The analog processing first transforms the ADC value including gain / offset.
// The processed value is compared to the last value and a "sticky" algorithm checks
// if the new value should be adopted or not. The purpose is to avoid unnecessary midi
// transmissions toggling back and forth between two adjacent controller values.
static uint8_t handleAnalogPedal_ISR(uint8_t p)
{
    uint8_t new_value;
    uint8_t state;
    int8_t diff;

    // Apply gain / offset analog transformation
    new_value = ProcessAdcValue(PedalSetup[p].Offset, PedalSetup[p].Gain, hal_adcGetValue_ISR(p));

    // CurrentState encode the last value, and if we are considered stable:
    state = PedalState[p].State;

    diff = (int8_t)new_value - (int8_t)(state & 127u);

    if (diff == 0)
    {
        // No change, make sure stable flag is set
        state |= 128;
    }
    else
    {
        if (state & 128)
        {
            // There is a change, and we are supposed to be stable, is change large enough?

            if (diff < 0)
            {
                diff = -diff;
            }

            if (diff > ANALOG_STICKY)
            {
                // Yes. Set new state and don't set stable bit
                state = new_value;
            }
            else
            {
                // Change not large enough. Don't do anything.
            }
        }
        else
        {
            // We are not stable, simply adopt new value
            state = new_value;
        }
    }

    // Write back new state
    PedalState[p].State = state;

    if (PedalSetup[p].Mode & MODE_INVERTED)
    {
        return 127 - (state & 127);
    }
    else
    {
        return (state & 127);
    }
}

void pedals_handleTick_ISR(void)
{
    uint8_t new_value;

    if (PedalSetup[0].Mode & MODE_ENABLED)
    {
        if (PedalSetup[0].Mode & MODE_DIGITAL)
        {
            new_value = handleDigitalPedal_ISR(0);
        }
        else
        {
            new_value = handleAnalogPedal_ISR(0);
        }

        if (new_value != PedalState[0].Value)
        {
            uint8_t index;

            PedalState[0].Value = new_value;

            // Generate midi message
            index = midiio_msgNew_ISR(MIDIMSG_SOURCE_GENERATED | MIDIMSG_FLAG_MSG_OK,
                                      MIDI_STATUS_CTRL_CHANGE | PedalSetup[0].Channel);

            midiio_msgAddData_ISR(index, PedalSetup[0].Controller);
            midiio_msgAddData_ISR(index, new_value);
            midiio_msgFinish_ISR(index, 0u);
        }
    }

    if (PedalSetup[1].Mode & MODE_ENABLED)
    {
        if (PedalSetup[1].Mode & MODE_DIGITAL)
        {
            new_value = handleDigitalPedal_ISR(1);
        }
        else
        {
            new_value = handleAnalogPedal_ISR(1);
        }

        if (new_value != PedalState[1].Value)
        {
            uint8_t index;

            PedalState[1].Value = new_value;

            // Generate midi message
            index = midiio_msgNew_ISR(MIDIMSG_SOURCE_GENERATED | MIDIMSG_FLAG_MSG_OK,
                                      MIDI_STATUS_CTRL_CHANGE | PedalSetup[1].Channel);

            // TODO the message won't get processed by filters?

            midiio_msgAddData_ISR(index, PedalSetup[1].Controller);
            midiio_msgAddData_ISR(index, new_value);
            midiio_msgFinish_ISR(index, 0u);
        }
    }

    // Profiling. Analog pedal processing, min: 9.6 us, max: 31.6 us
    // Profiling. Digital pedal processing, min: 4.8 us, max: 26.4 us

}

void pedals_handleMainLoop(void)
{

}


uint8_t pedals_menuGetSubCount(uint8_t pedal)
{
    uint8_t ret = 0;

    if (PedalSetup[pedal].Mode & MODE_ENABLED)
    {
        if (PedalSetup[pedal].Mode & MODE_DIGITAL)
        {
            ret = 2;
        }
        else
        {
            ret = 3;
        }
    }

    return ret;
}

void pedals_menuGetText(uint8_t pedal, char *dest, uint8_t item)
{
    if (item == 0)
    {
        // Pedal-1       (OFF)
        // Pedal-1   (DIGITAL)
        // Pedal-1    (ANALOG)

        util_writeFormat_P(dest, PSTR("Pedal-%i"), pedal + 1);

        // Lookup pedal mode in mode string table:
        switch (PedalSetup[pedal].Mode & (MODE_ENABLED | MODE_DIGITAL))
        {
        case 0:
            util_copyString_P(dest + 14, PSTR("(OFF)"));
            break;

        case MODE_ENABLED:
            util_copyString_P(dest + 11, PSTR("(ANALOG)"));
            break;

        case (MODE_ENABLED | MODE_DIGITAL):
            util_copyString_P(dest + 10, PSTR("(DIGITAL)"));
            break;
        }
    }
    else if (item == 1)
    {
        //  Ch1 CC: SustainP
        dest = util_writeFormat_P(dest, PSTR("Ch%i "), PedalSetup[pedal].Channel + 1);
        dest = util_writeFormat_P(dest, PSTR("CC: %c"), PedalSetup[pedal].Controller);

    }
    else if (item == 2)
    {
        //  Invert:1 Outp:XXX
        dest = util_writeFormat_P(dest, PSTR("Invert:%i "),
                (PedalSetup[pedal].Mode & MODE_INVERTED) ? 1 : 0);

        dest = util_writeFormat_P(dest, PSTR("Outp:%i "), PedalState[pedal].Value);

    }
    else if (item == 3)
    {
        //  Gain:XXX Offs:XXXX
        dest = util_writeFormat_P(dest, PSTR("Gain:%U "), PedalSetup[pedal].Gain);
        dest = util_writeFormat_P(dest, PSTR("Offs:%I "), PedalSetup[pedal].Offset);
    }
}

uint8_t pedals_menuHandleEvent(uint8_t pedal, uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret;
    int8_t knob_gained;

    // Are we changing the edit_mode level?
    if (user_event == MENU_EVENT_SELECT)
    {
        edit_mode++;
        knob_delta = 0;
    }
    else if (user_event == MENU_EVENT_BACK)
    {
        edit_mode--;
        knob_delta = 0;
    }

    if (user_event == MENU_EVENT_TURN_PUSH)
    {
        knob_gained = knob_delta * 10;
    }
    else
    {
        knob_gained = knob_delta;
    }


    // Deal with edit_mode combined with item
    switch (item * 4 + edit_mode)
    {
    case 1:  // 0 * 4 + 1:
        // We are editing pedal mode
        if (knob_delta > 0)
        {
            // Successively set MODE_ENABLED, then MODE_DIGITAL flags:
            PedalSetup[pedal].Mode |= (PedalSetup[pedal].Mode & MODE_ENABLED) ? MODE_DIGITAL : MODE_ENABLED;
        }
        else if (knob_delta < 0)
        {
            // Successively remove MODE_DIGITAL, then MODE_ENABLED flags:
            PedalSetup[pedal].Mode &= ~((PedalSetup[pedal].Mode & MODE_DIGITAL) ? MODE_DIGITAL : MODE_ENABLED);
        }

        ret = 17 | MENU_UPDATE_ALL;
        break;

    case 5:  // 1 * 4 + 1
        // Channel
        PedalSetup[pedal].Channel = util_boundedAddUint8(PedalSetup[pedal].Channel, 0, 15, knob_gained);
        ret = 2;
        break;

    case 6:  // 1 * 4 + 2
        // Controller
        PedalSetup[pedal].Controller = util_boundedAddUint8(PedalSetup[pedal].Controller, 0, 127, knob_gained);
        ret = 8;
        break;

    case 9: // 2 * 4 + 1
        // Invert
        if (knob_delta > 0)
        {
            PedalSetup[pedal].Mode |= MODE_INVERTED;
        }
        else if (knob_delta < 0)
        {
            PedalSetup[pedal].Mode &= ~MODE_INVERTED;
        }

        ret = 7 | MENU_UPDATE_ALL;
        break;

    case 13: // 3 * 4 + 1
        // Gain
        PedalSetup[pedal].Gain = util_boundedAddUint8(PedalSetup[pedal].Gain, 0, 255, knob_gained);
        ret = 5 | MENU_UPDATE_ALL;
        break;

    case 14: // 3 * 4 + 2
        // Offset
        PedalSetup[pedal].Offset = util_boundedAddInt8(PedalSetup[pedal].Offset, -128, 127, knob_gained);
        ret = 10 | MENU_UPDATE_ALL;
        break;

    default:
        // We don't support this edit mode
        ret = MENU_EDIT_MODE_UNAVAIL;
        break;
    }


    return ret;
}


uint8_t pedals_configGetSize(void)
{
    return sizeof(pedalSetup_t) * 2;
}

void pedals_configSave(uint8_t *dest)
{
    memcpy(dest, &(PedalSetup[0]), sizeof(pedalSetup_t) * 2);
}

void pedals_configLoad(uint8_t *dest)
{
    memcpy(&(PedalSetup[0]), dest, sizeof(pedalSetup_t) * 2);
}


