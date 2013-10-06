/*
 * presets.c
 *
 *  Created on: Jan 12, 2013
 *      Author: lars
 */




#include "common.h"
#include "menu.h" // for MENU_ defines TODO could be avoided
#include "util.h"
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "pgmstrings.h"
#include "menuentities.h"
#include "errors.h"


// HINT:
// To read out eeprom for investigation, using AVRDUDE:
//     avrdude -pm644p -cavrisp2 -Pusb -U eeprom:r:data.eep:r
// To investigate the data use
//     hexdump -C -v data.eep


static char const StrTitle[] PROGMEM = "Preset";
static char const StrMod[]   PROGMEM = "mod.";
static char const StrSave[]  PROGMEM = "Save ";
static char const StrLoad[]  PROGMEM = "Load ";

// static char const StrBackAborts[]  PROGMEM = "back aborts";

// Total number of preset slots in EEPROM
#define PRESET_SLOTS_COUNT  12

// Current state of loaded preset
static uint8_t CurrentPreset;

static bool_t  ConfigModified;
static bool_t  ConfigMustBeChecked;


// State of menu
static bool_t  MenuVisible = FALSE;
static uint8_t MenuSelected;


// Buffer for saving / loading config
#define PRESETS_BUFFER_SIZE 82
static uint8_t PresetsBuffer[PRESETS_BUFFER_SIZE];

static uint16_t PresetRecordSize;


// EEPROM Layout
//////////////////

// Starting from position 0 is the EEPROM header:
typedef struct
{
    uint8_t Sig[4];    // Should be "MMF"
    uint8_t VerMajor;  // Major version
    uint8_t VerMinor;  // Minor version
    uint8_t PresetSlots;
    uint8_t EntityCount;
    uint8_t EntitySizes[MENUE_ENTITY_COUNT];
} eeprom_header_t;

#define EEPROM_CHKSUM_SIZE   1

#define EEPROM_CHKSUM_RESET  0xBB

#define SIG_1   'M'
#define SIG_2   'M'
#define SIG_3   'F'
#define SIG_4   1

// Directly after the header comes the preset slots
// Each preset slot takes RecordSize bytes + one byte checksum

static void calculateRecordSize(void)
{
    uint8_t i;
    uint8_t size;

    PresetRecordSize = 0;

    for (i = 0; i < MENUE_ENTITY_COUNT; i++)
    {
        size = menuents_configGetSize(i);

        if (size > PRESETS_BUFFER_SIZE)
        {
            err_raise(ERR_MODULE_PRESETS, __LINE__); // Whoops, presets buffer too small!!
        }

        PresetRecordSize += size;
    }
}

static void fillEepromHeader(eeprom_header_t *header)
{
    uint8_t i;

    header->Sig[0] = SIG_1;
    header->Sig[1] = SIG_2;
    header->Sig[2] = SIG_3;
    header->Sig[3] = SIG_4;
    header->VerMajor = BUILD_VERSION_MAJOR;
    header->VerMinor = BUILD_VERSION_MINOR;
    header->PresetSlots = PRESET_SLOTS_COUNT;
    header->EntityCount = MENUE_ENTITY_COUNT;

    for (i = 0; i < MENUE_ENTITY_COUNT; i++)
    {
        header->EntitySizes[i] = menuents_configGetSize(i);
    }
}

// Check if we can accept eeprom header.
static bool_t checkEepromHeader(void)
{
    bool_t match = TRUE;
    uint16_t i;

    // Start by filling out expected values in buffer
    fillEepromHeader((eeprom_header_t *)PresetsBuffer);

    // It must match exactly what we got in eeprom
    for (i = 0; i < sizeof(eeprom_header_t); i++)
    {
        if (eeprom_read_byte((uint8_t*)i) != PresetsBuffer[i])
        {
            match = FALSE;
        }
    }

    return match;
}

static void writeEepromHeader(void)
{
    uint16_t i;

    // Start by filling out header data in buffer
    fillEepromHeader((eeprom_header_t *)PresetsBuffer);

    // Write to eeprom
    for (i = 0; i < sizeof(eeprom_header_t); i++)
    {
        eeprom_write_byte((uint8_t*)i, PresetsBuffer[i]);
    }
}


// 1 based. Slot 1 is first preset
static void savePreset(uint8_t slot)
{
    uint16_t addr;
    uint8_t chksum;
    uint8_t i;
    uint8_t j;
    uint8_t size;

    // Calculate address of preset
    addr = sizeof(eeprom_header_t) +
            (uint16_t)(slot - 1) * (PresetRecordSize + EEPROM_CHKSUM_SIZE);

    chksum = EEPROM_CHKSUM_RESET;

    for (i = 0; i < MENUE_ENTITY_COUNT; i++)
    {
        // Let entity write its data into our buffer
        menuents_configSave(i, PresetsBuffer);
        size = menuents_configGetSize(i);

        for (j = 0; j < size; j++)
        {
            // Write data to eeprom
            eeprom_write_byte((uint8_t*)addr, PresetsBuffer[j]);
            chksum += PresetsBuffer[j];
            addr++;
        }
    }

    // Finish with checksum
    eeprom_write_byte((uint8_t*)addr, chksum);

    CurrentPreset = slot;
    ConfigMustBeChecked = FALSE;
    ConfigModified = FALSE;
}

// 1 based. Slot 1 is first preset
static void loadPreset(uint8_t slot)
{
    uint16_t addr;
    uint8_t chksum;
    uint8_t i;
    uint8_t j;
    uint8_t size;

    // Calculate address of preset
    addr = sizeof(eeprom_header_t) +
           (uint16_t)(slot - 1) * (PresetRecordSize + EEPROM_CHKSUM_SIZE);

    chksum = EEPROM_CHKSUM_RESET;

    // First we verify checksum

    for (i = 0; i < MENUE_ENTITY_COUNT; i++)
    {
        size = menuents_configGetSize(i);

        for (j = 0; j < size; j++)
        {
            chksum += eeprom_read_byte((uint8_t*)addr);
            addr++;
        }
    }

    // Does it match?
    if (eeprom_read_byte((uint8_t*)addr) == chksum)
    {
        // OK, then let us load preset
        addr = sizeof(eeprom_header_t) +
               (uint16_t)(slot - 1) * (PresetRecordSize + EEPROM_CHKSUM_SIZE);

        for (i = 0; i < MENUE_ENTITY_COUNT; i++)
        {
            size = menuents_configGetSize(i);

            for (j = 0; j < size; j++)
            {
                PresetsBuffer[j] = eeprom_read_byte((uint8_t*)addr);
                addr++;
            }

            menuents_configLoad(i, PresetsBuffer);
        }

        // Thats it!
        CurrentPreset = slot;
        ConfigMustBeChecked = FALSE;
        ConfigModified = FALSE;
    }
    else
    {
        err_raise(ERR_MODULE_PRESETS, __LINE__); // Unexpected checksum error in EEPROM!
    }
}


// 1 based. Slot 1 is first preset
// Compares preset slot with current setup and returns true if there are differences
static bool_t checkForChanges(uint8_t slot)
{
    uint16_t addr;
    uint8_t chksum;
    uint8_t i;
    uint8_t j;
    uint8_t size;
    bool_t changes = FALSE;

    // Calculate address of preset
    addr = sizeof(eeprom_header_t) +
            (uint16_t)(slot - 1) * (PresetRecordSize + EEPROM_CHKSUM_SIZE);

    chksum = EEPROM_CHKSUM_RESET;

    for (i = 0; (i < MENUE_ENTITY_COUNT) && (changes == FALSE); i++)
    {
        // Let entity write its data into our buffer
        menuents_configSave(i, PresetsBuffer);
        size = menuents_configGetSize(i);

        for (j = 0; (j < size) && (changes == FALSE); j++)
        {
            // Compare with data in eeprom
            if (eeprom_read_byte((uint8_t*)addr) != PresetsBuffer[j])
            {
                changes = TRUE;
            }

            chksum += PresetsBuffer[j];
            addr++;
        }
    }

    // Also compare checksum
    if (eeprom_read_byte((uint8_t*)addr) != chksum)
    {
        changes = TRUE;
    }

    return changes;
}
// Important! The presets initialize function must be the last one to call in the
// initialization procedure. During initialization, a preset may be loaded into
// the components. If a component is initialized later, it may overwrite with
// default values.
void presets_initialize(void)
{
    CurrentPreset = 1; // TODO implement having no preset loaded / saved, and implement a load defaults!
    ConfigModified = FALSE;
    ConfigMustBeChecked = FALSE;

    MenuVisible = FALSE;

    calculateRecordSize();

    if (checkEepromHeader())
    {
        // EEPROM is accepted, Load preset 1
        loadPreset(CurrentPreset);
    }
    else
    {
        // EEPROM is blank or corrupted

        // Save default values into all preset slots(!)
        // This may take some time actually
        uint8_t i;

        for (i = 1; i <= PRESET_SLOTS_COUNT; i++)
        {
            savePreset(i);
        }

        CurrentPreset = 1;

        // Write in the header now that we should be in a consistent state again
        writeEepromHeader();
    }

}


void presets_notifyConfigMayChange(uint8_t entity_index)
{
    UNUSED(entity_index);

    ConfigMustBeChecked = TRUE;
}


// MENU implementation


uint8_t presets_menuGetSubCount(void)
{
    return MenuVisible ? 2 : 0;
}

void presets_menuGetText(char *dest, uint8_t item)
{
    switch (item)
    {
    case 0:
        // Write ex.: "Preset 10 (mod) (+)"
        util_copyString_P(dest, StrTitle);
        util_writeInt8LA(dest + 7, CurrentPreset);

        // Check if any of the menu entities config has changed
        if (ConfigMustBeChecked)
        {
            ConfigMustBeChecked = FALSE;
            ConfigModified = checkForChanges(CurrentPreset);
        }

        if (ConfigModified)
        {
            util_copyString_P(dest + ((CurrentPreset > 9) ? 10 : 9), StrMod);
        }

        if (MenuVisible)
        {
            dest = util_copyString_P(dest + 16, pstr_MinusParentheses);
        }
        else
        {
            dest = util_copyString_P(dest + 16, pstr_PlusParentheses);
        }
        break;

    case 1: // Load
        dest = util_copyString_P(dest, StrLoad);

        if (MenuSelected != 0)
        {
            util_writeInt8LA(dest, MenuSelected);
            //util_strCpy_P(dest + 2, presets_StrBackAborts);
        }
        break;

    case 2: // Save
        dest = util_copyString_P(dest, StrSave);

        if (MenuSelected != 0)
        {
            util_writeInt8LA(dest, MenuSelected);
            //util_strCpy_P(dest + 2, presets_StrBackAborts);
        }
        break;
    }

}

uint8_t presets_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

    if (user_event == MENU_EVENT_SELECT)
    {
        if (item == 0)
        {
            // This toggles showing sub items
            MenuVisible = !MenuVisible;
            ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
        }
        else
        {
            if (edit_mode == 0)
            {
                // Go into number Save / Load selection
                ret = 5;
                MenuSelected = CurrentPreset;
            }
            else if (edit_mode == 1)
            {
                // Actually save or load preset
                if (item == 1)
                {
                    // LOAD the preset!
                    loadPreset(MenuSelected);
                }
                else if (item == 2)
                {
                    // SAVE the preset!
                    savePreset(MenuSelected);
                }

                // Step out of menu, and inform menu to update all
                MenuSelected = 0;
                ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
            }
        }
    }
    else if (user_event == MENU_EVENT_BACK)
    {
        // Just make sure menuselected is 0
        MenuSelected = 0;
    }
    else if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
    {
        if (edit_mode == 1)
        {
            MenuSelected = util_boundedAddInt8(MenuSelected, 1, PRESET_SLOTS_COUNT, knob_delta);
            ret = 5;
        }
    }

    return ret;
}
