
#include "config.h"
#include "can_drv.h"

//------------------------------------------------------------------------------
//! This function clears the Mailbox content.
//! It reset CANSTMOB, CANCDMOB, CANIDTx & CANIDMx and clears data FIFO of
//! MOb[0] upto MOb[LAST_MOB_NB].
//!
//! @warning: This version doesn't clears the data FIFO
//------------------------------------------------------------------------------
void can_clear_all_mob(void)
{
U8  mob_number;
/*
    U8  data_index;
*/

    for (mob_number = 0; mob_number < NB_MOB; mob_number++)
    {
        CANPAGE = (mob_number << 4);    //! Page index
        Can_clear_mob();                //! All MOb Registers=0
/*
        for (data_index = 0; data_index < NB_DATA_MAX; data_index++)
        {
            CANMSG = 0;                 //! MOb data FIFO
        }
*/
    }
}

//------------------------------------------------------------------------------
//! This function returns the number of the first MOb available or 0xFF if
//! no MOb is available.
//!
//! @return Handle of MOb.
//!          - MOb[0] upto MOb[LAST_MOB_NB]
//!          - 0xFF if no MOb
//------------------------------------------------------------------------------
U8 can_get_mob_free(void)
{
    U8 mob_number, page_saved;

    page_saved = CANPAGE;
    for (mob_number = 0; mob_number < NB_MOB; mob_number++)
    {
        Can_set_mob(mob_number);
        if ((CANCDMOB & 0xC0) == 0x00) //! Disable configuration
        {
            CANPAGE = page_saved;
            return (mob_number);
        }
    }
    CANPAGE = page_saved;
    return (NO_MOB);
}

//------------------------------------------------------------------------------
//! This function returns information "MOB completed its job"
//! if one of the RXOK or TXOK Flag is set or "MOB not completed its job
//! if no RXOK and TXOK flags are set.
//! Previously, this function checks if the MOb is configured or not and in
//!  case of the MOB not configured, the function returns "MOB_DISABLE".
//!
//! @return MOb Status.
//!          -  MOB_NOT_COMPLETED
//!          -  MOB_TX_COMPLETED
//!          -  MOB_RX_COMPLETED
//!          -  MOB_RX_DLC_WARNING
//!          -  MOB_DISABLE
//!          or should be a combination of the following errors
//!          -  MOB_ACK_ERROR
//!          -  MOB_FORM_ERROR
//!          -  MOB_CRC_ERROR
//!          -  MOB_STUFF_ERROR
//!          -  MOB_BIT_ERROR
//------------------------------------------------------------------------------
U8 can_get_mob_status(void)
{
    U8 mob_status, canstmob_copy;

    // Test if MOb ENABLE or DISABLE
    if ((CANCDMOB & 0xC0) == 0x00) {return(MOB_DISABLE);}

    canstmob_copy = CANSTMOB; // Copy for test integrity

    // If MOb is ENABLE, test if MOb is COMPLETED
    // - MOb Status = 0x20 then MOB_RX_COMPLETED
    // - MOb Status = 0x40 then MOB_TX_COMPLETED
    // - MOb Status = 0xA0 then MOB_RX_COMPLETED_DLCW
    mob_status = canstmob_copy & ((1<<DLCW)|(1<<TXOK)|(1<<RXOK));
    if ( (mob_status==MOB_RX_COMPLETED) ||   \
         (mob_status==MOB_TX_COMPLETED) ||   \
         (mob_status==MOB_RX_COMPLETED_DLCW) ) { return(mob_status); }

    // If MOb is ENABLE & NOT_COMPLETED, test if MOb is in ERROR
    // - MOb Status bit_0 = MOB_ACK_ERROR
    // - MOb Status bit_1 = MOB_FORM_ERROR
    // - MOb Status bit_2 = MOB_CRC_ERROR
    // - MOb Status bit_3 = MOB_STUFF_ERROR
    // - MOb Status bit_4 = MOB_BIT_ERROR
    mob_status = canstmob_copy & ERR_MOB_MSK;
    if (mob_status != 0) { return(mob_status); }

    // If CANSTMOB = 0 then MOB_NOT_COMPLETED
    return(MOB_NOT_COMPLETED);
}

//------------------------------------------------------------------------------
//! This function copy the data from the selected MOb to the address
//! passed as parameter.
//!
//! @param CAN message data address.
//------------------------------------------------------------------------------
void can_get_data(U8* p_can_message_data)
{
    U8 data_index;

    for (data_index = 0; data_index < (Can_get_dlc()); data_index++)
    {
        *(p_can_message_data + data_index) = CANMSG;
    }
}


























