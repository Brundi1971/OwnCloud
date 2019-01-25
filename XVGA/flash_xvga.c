
#include "compiler.h"
#include "acces_porturi_xvga.h"
#include "flashc.h"
#include "flash_xvga.h"
#include "drawings_xvga.h"
#include "usart_xvga.h"
#include "configuration_xvga.h"

stored_RAM_u read_value_of_RAM;
stored_RAL_u read_value_of_RAL;
stored_RAE_u read_value_of_RAE;

__attribute__((__section__(".flash_nvram")))
static const U64 stored_RAM;
__attribute__((__section__(".flash_nvram")))
static const U64 stored_RAE;
__attribute__((__section__(".flash_nvram")))
static const U64 stored_RAL;


void read_stored_values_of_RAM(void)
{
    read_value_of_RAM.U64_data = read_RAM_from_flash();
    if ((read_value_of_RAM.tData.flags[0] == 'O') && (read_value_of_RAM.tData.flags[1] == 'K'))
    {
        sNewRAM.wLgX = read_value_of_RAM.tData.wLgX;
        sNewRAM.swOffsetX = read_value_of_RAM.tData.swOffsetX;
        sNewRAM.swOffsetY = read_value_of_RAM.tData.swOffsetY;
    }
    else
    {
        sNewRAM.wLgX = DEFAULT_RAM_LGX;
        sNewRAM.swOffsetX = 0;
        sNewRAM.swOffsetY = 0;
    }
    sNewRAM.wRPM = (U16)((50 * FACTOR_DE_SCALARE_RPM) / sNewRAM.wLgX);
    sNewRAE.wRPM = sNewRAM.wRPM;
    sNewRAL.wRPM = sNewRAM.wRPM;
    sNewRStandard.wRPM = sNewRAM.wRPM;
    sNewRGrid.wRPM = sNewRAM.wRPM;

    sNewRAM.wPozAX = DISPLAY_RESOLUTION_X_90 / 2 + sNewRAM.swOffsetX;
    sNewRAM.wPozAY = DISPLAY_RESOLUTION_Y_90 / 2 - sNewRAM.swOffsetY - sNewRAM.wLgX / 2;
    sNewRAM.wPozBX = DISPLAY_RESOLUTION_X_90 / 2 + sNewRAM.swOffsetX;
    sNewRAM.wPozBY = DISPLAY_RESOLUTION_Y_90 / 2 - sNewRAM.swOffsetY;
}

void read_stored_values_of_RAE(void)
{
    read_value_of_RAE.U64_data = read_RAE_from_flash();
    if ((read_value_of_RAE.tData.flags[0] == 'O') && (read_value_of_RAE.tData.flags[1] == 'K'))
    {
        sNewRAE.wOriginX = read_value_of_RAE.tData.wOriginX;
        sNewRAE.wOriginY = read_value_of_RAE.tData.wOriginY;
    }
    else
    {
        sNewRAE.wOriginX = DEFAULT_RAE_POZ_X;
        sNewRAE.wOriginY = DEFAULT_RAE_POZ_Y;
    }
    sNewRAE.wPozX = sNewRAE.wOriginX;
    sNewRAE.wPozY = sNewRAE.wOriginY;
}

void read_stored_values_of_RAL(void)
{
    read_value_of_RAL.U64_data = read_RAL_from_flash();
    if ((read_value_of_RAL.tData.flags[0] == 'O') && (read_value_of_RAL.tData.flags[1] == 'K'))
    {
        sNewRAL.wOriginX = read_value_of_RAL.tData.wOriginX;
        sNewRAL.wOriginY = read_value_of_RAL.tData.wOriginY;
    }
    else
    {
        sNewRAL.wOriginX = DEFAULT_RAL_POZ_X;
        sNewRAL.wOriginY = DEFAULT_RAL_POZ_Y;
    }
    sNewRAL.wPozX = sNewRAL.wOriginX;
    sNewRAL.wPozY = sNewRAL.wOriginY;
}


void store_values_position_of_refernce(void)
{
    switch(CmdCode)
    {

        case DISPLAY_RAE:
                           if ((NewCmdCode == DISPLAY_RAM) || (NewCmdCode == DISPLAY_RAL) || (NewCmdCode == DISPLAY_STANDARD))
                           {
                               read_value_of_RAE.tData.flags[0] = 'O';
                               read_value_of_RAE.tData.flags[1] = 'K';
                               read_value_of_RAE.tData.wOriginX = sNewRAE.wPozX;
                               read_value_of_RAE.tData.wOriginY = sNewRAE.wPozY;
                               write_RAE_to_flash(read_value_of_RAE.U64_data);
                           }
                           break;
         case DISPLAY_RAL:
                           if ((NewCmdCode == DISPLAY_RAM) || (NewCmdCode == DISPLAY_RAE) || (NewCmdCode == DISPLAY_STANDARD))
                           {
                               read_value_of_RAL.tData.flags[0] = 'O';
                               read_value_of_RAL.tData.flags[1] = 'K';
                               read_value_of_RAL.tData.wOriginX = sNewRAL.wPozX;
                               read_value_of_RAL.tData.wOriginY = sNewRAL.wPozY;
                               write_RAL_to_flash(read_value_of_RAL.U64_data);
                           }
                           break;
         case DISPLAY_RAM:
                           if ((NewCmdCode == DISPLAY_RAL) || (NewCmdCode == DISPLAY_RAE) || (NewCmdCode == DISPLAY_STANDARD))
                           {
                               read_value_of_RAM.tData.flags[0] = 'O';
                               read_value_of_RAM.tData.flags[1] = 'K';
                               read_value_of_RAM.tData.wLgX = sNewRAM.wLgX;
                               read_value_of_RAM.tData.swOffsetX = sNewRAM.swOffsetX;
                               read_value_of_RAM.tData.swOffsetY = sNewRAM.swOffsetY;
                               write_RAM_to_flash(read_value_of_RAM.U64_data);
                           }
                           break;
        default:break;
    }

}

U64 read_RAM_from_flash(void) {return stored_RAM;}
U64 read_RAE_from_flash(void) {return stored_RAE;}
U64 read_RAL_from_flash(void) {return stored_RAL;}
void write_RAM_to_flash(U64 data) {flashc_memset64((void *)&stored_RAM, data, 8, TRUE);}
void write_RAE_to_flash(U64 data) {flashc_memset64((void *)&stored_RAE, data, 8, TRUE);}
void write_RAL_to_flash(U64 data) {flashc_memset64((void *)&stored_RAL, data, 8, TRUE);}
