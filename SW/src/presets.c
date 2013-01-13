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


// HINT:
// To read out eeprom for investigation, using AVRDUDE:
//     avrdude -pm644p -cavrisp2 -Pusb -U eeprom:r:data.eep:r
// To investigate the data use
//     hexdump -C -v data.eep


char presets_StrTitle[] PROGMEM = "Preset ";
char presets_StrMod[]   PROGMEM = "mod. ";
char presets_StrSave[]  PROGMEM = "Save ";
char presets_StrLoad[]  PROGMEM = "Load ";

char presets_StrBackAborts[]  PROGMEM = "back aborts";

// Total number of preset slots in EEPROM
#define PRESETS_SLOTS  12

// Current state of loaded preset
uint8_t presetsCurrent;

bool_t  psConfigModified;
bool_t  psConfigMustBeChecked;


// State of menu
bool_t  psMenuVisible = FALSE;
uint8_t psMenuSelected;


// Buffer for saving / loading config
#define PRESETS_BUFFER_SIZE 70
uint8_t presetsBuffer[PRESETS_BUFFER_SIZE];

uint16_t presetRecordSize;


// EEPROM Layout
//////////////////

// Starting from position 0 is the EEPROM header:
typedef struct
{
    uint8_t Sig[3];    // Should be "MMF"
    uint8_t VerMajor;  // Major version
    uint8_t VerMinor;  // Minor version
    uint8_t PresetSlots;
    uint8_t EntityCount;
    uint8_t EntitySizes[MENUE_ENTITY_COUNT];
} eeprom_header_t;

#define EEPROM_CHKSUM_SIZE   1

#define EEPROM_CHKSUM_RESET  0xBB

// Directly after the header comes the preset slots
// Each preset slot takes RecordSize bytes + one byte checksum

void presets_CalculateRecordSize(void)
{
    uint8_t i;
    uint8_t size;

    presetRecordSize = 0;

    for (i = 0; i < MENUE_ENTITY_COUNT; i++)
    {
        size = menue_ConfigGetSize(i);

        if (size > PRESETS_BUFFER_SIZE)
        {
            // TODO raise SERIOUs error here
        }

        presetRecordSize += size;
    }
}

void presets_FillEepromHeader(eeprom_header_t *header)
{
    uint8_t i;

    header->Sig[0] = 'M';
    header->Sig[1] = 'M';
    header->Sig[2] = 'F';
    header->VerMajor = BUILD_VERSION_MAJOR;
    header->VerMinor = BUILD_VERSION_MINOR;
    header->PresetSlots = PRESETS_SLOTS;
    header->EntityCount = MENUE_ENTITY_COUNT;

    for (i = 0; i < MENUE_ENTITY_COUNT; i++)
    {
        header->EntitySizes[i] = menue_ConfigGetSize(i);
    }
}

// Check if we can accept eeprom header.
bool_t presets_CheckEepromHeader(void)
{
    bool_t match = TRUE;
    uint16_t i;

    // Start by filling out expected values in buffer
    presets_FillEepromHeader((eeprom_header_t *)presetsBuffer);

    // It must match exactly what we got in eeprom
    for (i = 0; i < sizeof(eeprom_header_t); i++)
    {
        if (eeprom_read_byte((uint8_t*)i) != presetsBuffer[i])
        {
            match = FALSE;
        }
    }

    return match;
}

void presets_WriteEepromHeader(void)
{
    uint16_t i;

    // Start by filling out header data in buffer
    presets_FillEepromHeader((eeprom_header_t *)presetsBuffer);

    // Write to eeprom
    for (i = 0; i < sizeof(eeprom_header_t); i++)
    {
        eeprom_write_byte((uint8_t*)i, presetsBuffer[i]);
    }
}


// 1 based. Slot 1 is first preset
void presets_Save(uint8_t slot)
{
    uint16_t addr;
    uint8_t chksum;
    uint8_t i;
    uint8_t j;
    uint8_t size;

    // Calculate address of preset
    addr = sizeof(eeprom_header_t) +
            (uint16_t)(slot - 1) * (presetRecordSize + EEPROM_CHKSUM_SIZE);

    chksum = EEPROM_CHKSUM_RESET;

    for (i = 0; i < MENUE_ENTITY_COUNT; i++)
    {
        // Let entity write its data into our buffer
        menue_ConfigSave(i, presetsBuffer);
        size = menue_ConfigGetSize(i);

        for (j = 0; j < size; j++)
        {
            // Write data to eeprom
            eeprom_write_byte((uint8_t*)addr, presetsBuffer[j]);
            chksum += presetsBuffer[j];
            addr++;
        }
    }

    // Finish with checksum
    eeprom_write_byte((uint8_t*)addr, chksum);

    presetsCurrent = slot;
    psConfigMustBeChecked = FALSE;
    psConfigModified = FALSE;
}

// 1 based. Slot 1 is first preset
void presets_Load(uint8_t slot)
{
    uint16_t addr;
    uint8_t chksum;
    uint8_t i;
    uint8_t j;
    uint8_t size;

    // Calculate address of preset
    addr = sizeof(eeprom_header_t) +
           (uint16_t)(slot - 1) * (presetRecordSize + EEPROM_CHKSUM_SIZE);

    chksum = EEPROM_CHKSUM_RESET;

    // First we verify checksum

    for (i = 0; i < MENUE_ENTITY_COUNT; i++)
    {
        size = menue_ConfigGetSize(i);

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
               (uint16_t)(slot - 1) * (presetRecordSize + EEPROM_CHKSUM_SIZE);

        for (i = 0; i < MENUE_ENTITY_COUNT; i++)
        {
            size = menue_ConfigGetSize(i);

            for (j = 0; j < size; j++)
            {
                presetsBuffer[j] = eeprom_read_byte((uint8_t*)addr);
                addr++;
            }

            menue_ConfigLoad(i, presetsBuffer);
        }

        // Thats it!
        presetsCurrent = slot;
        psConfigMustBeChecked = FALSE;
        psConfigModified = FALSE;
    }
    else
    {
        // TODO raise error
    }
}


// 1 based. Slot 1 is first preset
// Compares preset slot with current setup and returns true if there are differences
bool_t presets_CheckForChanges(uint8_t slot)
{
    uint16_t addr;
    uint8_t chksum;
    uint8_t i;
    uint8_t j;
    uint8_t size;
    bool_t changes = FALSE;

    // Calculate address of preset
    addr = sizeof(eeprom_header_t) +
            (uint16_t)(slot - 1) * (presetRecordSize + EEPROM_CHKSUM_SIZE);

    chksum = EEPROM_CHKSUM_RESET;

    for (i = 0; (i < MENUE_ENTITY_COUNT) && (changes == FALSE); i++)
    {
        // Let entity write its data into our buffer
        menue_ConfigSave(i, presetsBuffer);
        size = menue_ConfigGetSize(i);

        for (j = 0; (j < size) && (changes == FALSE); j++)
        {
            // Compare with data in eeprom
            if (eeprom_read_byte((uint8_t*)addr) != presetsBuffer[j])
            {
                changes = TRUE;
            }

            chksum += presetsBuffer[j];
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
void presets_Initialize(void)
{
    presetsCurrent = 1; // TODO implement having no preset loaded / saved, and implement a load defaults!
    psConfigModified = FALSE;
    psConfigMustBeChecked = FALSE;

    psMenuVisible = FALSE;

    presets_CalculateRecordSize();

    if (presets_CheckEepromHeader())
    {
        // EEPROM is accepted, Load preset 1
        presets_Load(presetsCurrent);
    }
    else
    {
        // EEPROM is blank or corrupted

        // Save default values into all preset slots(!)
        // This may take some time actually
        uint8_t i;

        for (i = 1; i <= PRESETS_SLOTS; i++)
        {
            presets_Save(i);
        }

        presetsCurrent = 1;

        // Write in the header now that we should be in a consistent state again
        presets_WriteEepromHeader();
    }

}


void presets_ConfigMayChangeNotify(uint8_t entity_index)
{
    psConfigMustBeChecked = TRUE;
}


// MENU implementation


uint8_t presets_GetSubMenuCount(void)
{
    return psMenuVisible ? 2 : 0;
}

void presets_GetMenuText(char *dest, uint8_t item)
{
    switch (item)
    {
    case 0:
        // Write ex.: "Preset 10 mod. (+)"
        util_StrCpy_P(dest, presets_StrTitle);
        util_StrWriteInt8LA(dest + 7, presetsCurrent);

        // Check if any of the menu entities config has changed
        if (psConfigMustBeChecked)
        {
            psConfigMustBeChecked = FALSE;
            psConfigModified = presets_CheckForChanges(presetsCurrent);
        }

        if (psConfigModified)
        {
            util_StrCpy_P(dest + ((presetsCurrent > 9) ? 10 : 9), presets_StrMod);
        }

        if (psMenuVisible)
        {
            dest = util_StrCpy_P(dest + 16, pstr_MinusParentheses);
        }
        else
        {
            dest = util_StrCpy_P(dest + 16, pstr_PlusParentheses);
        }
        break;

    case 1: // Load
        dest = util_StrCpy_P(dest, presets_StrLoad);

        if (psMenuSelected != 0)
        {
            util_StrWriteInt8LA(dest, psMenuSelected);
            //util_StrCpy_P(dest + 2, presets_StrBackAborts);
        }
        break;

    case 2: // Save
        dest = util_StrCpy_P(dest, presets_StrSave);

        if (psMenuSelected != 0)
        {
            util_StrWriteInt8LA(dest, psMenuSelected);
            //util_StrCpy_P(dest + 2, presets_StrBackAborts);
        }
        break;
    }

}

uint8_t presets_MenuEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

    if (user_event == MENU_EVENT_SELECT)
    {
        if (item == 0)
        {
            // This toggles showing sub items
            psMenuVisible = !psMenuVisible;
            ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
        }
        else
        {
            if (edit_mode == 0)
            {
                // Go into number Save / Load selection
                ret = 5;
                psMenuSelected = presetsCurrent;
            }
            else if (edit_mode == 1)
            {
                // Actually save or load preset
                if (item == 1)
                {
                    // LOAD the preset!
                    presets_Load(psMenuSelected);
                }
                else if (item == 2)
                {
                    // SAVE the preset!
                    presets_Save(psMenuSelected);
                }

                // Step out of menu, and inform menu to update all
                psMenuSelected = 0;
                ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
            }
        }
    }
    else if (user_event == MENU_EVENT_BACK)
    {
        // Just make sure menuselected is 0
        psMenuSelected = 0;
    }
    else if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
    {
        if (edit_mode == 1)
        {
            psMenuSelected = util_BoundedAddInt8(psMenuSelected, 1, PRESETS_SLOTS, knob_delta);
            ret = 5;
        }
    }

    return ret;
}
