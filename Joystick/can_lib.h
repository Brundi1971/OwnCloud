
#ifndef _CAN_LIB_H_
#define _CAN_LIB_H_

#include "can_drv.h"

#define CAN_CMD_REFUSED                     0xFF
#define CAN_CMD_ACCEPTED                    0x00
#define CAN_STATUS_COMPLETED                0x00
#define CAN_STATUS_NOT_COMPLETED            0x01
#define CAN_STATUS_ERROR                    0x02

typedef enum {
  CMD_NONE,
  CMD_TX,
  CMD_TX_DATA,
  CMD_TX_REMOTE,
  CMD_RX, 
  CMD_RX_DATA, 
  CMD_RX_REMOTE,
  CMD_RX_MASKED,
  CMD_RX_DATA_MASKED,
  CMD_RX_REMOTE_MASKED, 
  CMD_REPLY,        
  CMD_REPLY_MASKED,
  CMD_ABORT
} can_cmd_t; 

typedef union{
  U32 ext;
  U16 std;
  U8  tab[4];
} can_id_t; 

typedef struct{
  Bool rtr;
  Bool ide;
} can_ctrl_t;

// ----------

// 3) id    :  initialize by the application in transmission
//             complete by the library in reception.
typedef  struct{
  U8         handle; 
  can_cmd_t  cmd; 
  can_id_t   id;
  U8         dlc;  
  U8*        pt_data; 
  U8         status; 
  can_ctrl_t ctrl;   
} st_cmd_t;

void can_init(void);
U8 can_cmd (st_cmd_t *);
U8 can_get_status (st_cmd_t *); 

#endif // _CAN_LIB_H_
