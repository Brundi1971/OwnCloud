

#include "gpio.h"
#include "usart.h"
#include "usart_xvga.h"
#include "board.h"
#include "acces_porturi_xvga.h"
#include "timer_xvga.h"
#include "drawings_xvga.h"
#include "configuration_xvga.h"
#include "flash_xvga.h"
#include "math.h"

DStandard sStandard, sNewStandard;
DFrame sFrame, sNewFrame;
//volatile DGrid sGrid, sNewGrid;
DCross sCross, sNewCross;

DRAM sRAM, sNewRAM;
DRAL sRAL, sNewRAL;
DRAE sRAE, sNewRAE;
DRStandard sRStandard, sNewRStandard;
DRGrid sNewRGrid, sRGrid;


U8 CDA_RAM_X, CDA_RAM_Y, ETAL_RAM_X;
U8 CDA_RAL_X, CDA_RAL_Y;
U8 CDA_RAE_X, CDA_RAE_Y;
S16 CDA_RO_X, CDA_RO_Y;

U8 ReceiverBufferRS232[DIMENSIUNE_RS232_RECEIVER_BUFFER];
U8 TransmissionBufferRS232[DIMENSIUNE_RS232_TRANSMISSON_BUFFER];
U8 ByteIndex_ReceivedFromRS232 = 0;
U8 TransmissionIndex;
U8 RS232Scheduler = CHECK_THE_FIRST_CHARACTER;
U8 MainScheduler = MSP_PREPARE_THE_RECEIVER;
U8 MessageLength;
U8 CodEroare;
U8 CmdCode = 255, NewCmdCode;
U8 bTest, bNewTest;

U8 DrawFiguresScheduler;
U8 DRALScheduler;
U8 DRAMScheduler;
U8 DDScheduler;
U8 DGScheduler;
U8 DTScheduler;
U8 DRAEScheduler;
U8 RefreshOfStandardData;
U8 ToggleOnOffCursor = CURSOR_ON;
U8 bMODE;


void init_usart(void)
{
  static const gpio_map_t USART_GPIO_MAP =
  {
      {XVGA_USART_RX_PIN, XVGA_USART_RX_FUNCTION},
      {XVGA_USART_TX_PIN, XVGA_USART_TX_FUNCTION}
  };

  static const usart_options_t USART_OPTIONS =
  {
      .baudrate     = USART_BAUDRATE,//57600,
      .charlength   = 8,
      .paritytype   = USART_NO_PARITY,
      .stopbits     = USART_1_STOPBIT,
      .channelmode  = USART_NORMAL_CHMODE
  };

  gpio_enable_module(USART_GPIO_MAP,
                     sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

  if (USART_SUCCESS == usart_init_rs232(XVGA_USART, &USART_OPTIONS, PBA_HZ))
  {;}
}

void reset_rs232_errors(volatile avr32_usart_t *usart)
{
	usart->CR.rststa = 1;
}


U8 receive_data_from_RS232(void)
{
    int Result, c;

    Result = usart_read_char(XVGA_USART,&c);
    if (Result == USART_SUCCESS)
    {
        if (ByteIndex_ReceivedFromRS232 != DIMENSIUNE_RS232_RECEIVER_BUFFER)
            ReceiverBufferRS232[ByteIndex_ReceivedFromRS232++] = (U8)c;
    }
    else if (Result == USART_RX_ERROR){reset_rs232_errors(XVGA_USART);}

    switch(RS232Scheduler)
    {
        case CHECK_THE_FIRST_CHARACTER:
                                          if (ByteIndex_ReceivedFromRS232 >= 1)
                                          {TimeOutForRS232 = 0; RS232Scheduler = CHECK_THE_TIME_OUT;} break;
        case CHECK_THE_TIME_OUT:          if (TimeOutForRS232 >= TIME_OUT_FOR_RS232) return TRUE;break;
        default:                          MainScheduler = MSP_PREPARE_THE_RECEIVER;break;
    }
    return FALSE;
}

U8 check_corectness_of_telegram(void)
{
    int i;
    U8 CheckSum = 0;
    CodEroare = 0;

    if(ByteIndex_ReceivedFromRS232 >= 3)
    {
        if (ReceiverBufferRS232[0] == 0x0a)
        {
            MessageLength = ReceiverBufferRS232[1];
            NewCmdCode = ReceiverBufferRS232[2];
            if (ByteIndex_ReceivedFromRS232 == MessageLength)
            {
                 CheckSum = 0;
                 for (i = 0; i <= MessageLength - 2; i++) CheckSum += ReceiverBufferRS232[i];
                 if(CheckSum == ReceiverBufferRS232[MessageLength-1])
                 {
                     switch(NewCmdCode)
                     {
                         case DISPLAY_STATUS:
                                                  //sNewRGrid.StartMoving = STOP_MOVING;
                                                  CodEroare |= (1 << BIT_STATUS_OK);
                                                  return TRUE;
                                                  break;
                         case DISPLAY_OFF:
                                                  sNewRGrid.StartMoving = STOP_MOVING;
                                                  CodEroare |= (1 << BIT_STATUS_OK);
                                                  bMODE = 0;
                                                  return TRUE;
                                                  break;
                         case DISPLAY_RAE:
                                                  sNewRGrid.StartMoving = STOP_MOVING;
                                                  //sNewCross.bLg_brat = ReceiverBufferRS232[3];
                                                  //sNewCross.wPoz_x = (ReceiverBufferRS232[5] << 8) | ReceiverBufferRS232[4];
                                                  //sNewCross.wPoz_y = (ReceiverBufferRS232[7] << 8) | ReceiverBufferRS232[6];
                                                  CDA_RAE_X = ReceiverBufferRS232[3];
                                                  CDA_RAE_Y = ReceiverBufferRS232[4];

                                                  switch(CDA_RAE_X)
                                                  {
                                                      case 0: break;
                                                      case 1: if (sNewRAE.wPozX + 1 <= sNewRAM.wPozAX + FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)//((sNewRAE.wPozX + 1 > sNewRAM.wPozAX - FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) &&
                                                                 sNewRAE.wPozX = sNewRAE.wPozX + 1;break;
                                                      case 2: if (sNewRAE.wPozX - 1 >= sNewRAM.wPozAX - FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)// && (sNewRAE.wPozX - 1 < sNewRAM.wPozAX + FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM))
                                                                 sNewRAE.wPozX = sNewRAE.wPozX - 1;break;
                                                      case 3: if (sNewRAE.wPozX + 5 <= sNewRAM.wPozAX + FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) //(sNewRAE.wPozX + 5 > sNewRAM.wPozAX - FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) //&&
                                                                 sNewRAE.wPozX = sNewRAE.wPozX + 5;break;
                                                      case 4: if (sNewRAE.wPozX - 5 >= sNewRAM.wPozAX - FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)//(sNewRAE.wPozX - 5 < sNewRAM.wPozAX + FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) // &&
                                                                 sNewRAE.wPozX = sNewRAE.wPozX - 5;break;
                                                      default: break;
                                                  }

                                                  switch(CDA_RAE_Y)
                                                  {
                                                      case 0: break;
                                                      case 1: if (sNewRAE.wPozY - 1 >= sNewRAM.wPozAY - FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) //&& (sNewRAE.wPozY - 1 < sNewRAM.wPozAY + FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM))
                                                                 sNewRAE.wPozY = sNewRAE.wPozY - 1;break;
                                                      case 2: if (sNewRAE.wPozY + 1 <= sNewRAM.wPozAY + FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)//((sNewRAE.wPozY + 1 > sNewRAM.wPozAY - FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) &&
                                                                 sNewRAE.wPozY = sNewRAE.wPozY + 1;break;
                                                      case 3: if (sNewRAE.wPozY - 5 >= sNewRAM.wPozAY - FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)// && (sNewRAE.wPozY - 4 < sNewRAM.wPozAY + FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM))
                                                                 sNewRAE.wPozY = sNewRAE.wPozY - 5;break;
                                                      case 4: if (sNewRAE.wPozY + 5 <= sNewRAM.wPozAY + FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) //((sNewRAE.wPozY + 5 > sNewRAM.wPozAY - FACTOR_LIMITA_RAE * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) &&
                                                                 sNewRAE.wPozY = sNewRAE.wPozY + 5;break;
                                                      default: break;

                                                  }
                                                  sNewRAE.wBratX = (U16)((BRAT_RAE_X * FACTOR_DE_SCALARE_RPM) / sNewRAE.wRPM);
                                                  sNewRAE.wBratY = (U16)((BRAT_RAE_Y * FACTOR_DE_SCALARE_RPM) / sNewRAE.wRPM);

                                                  bMODE = 1;
                                                  RAEStatus = CROSS_STEP1;
                                                  DRAEScheduler = CROSS_STEP1;
                                                  FillScheduler = FILL_STEP1;
                                                  CodEroare |= (1 << BIT_STATUS_OK);
                                                  return TRUE;
                                                  break;
                         case DISPLAY_RAL:
                                                  sNewRGrid.StartMoving = STOP_MOVING;
                                                  //sNewCross.bLg_brat = ReceiverBufferRS232[3];
                                                  //sNewCross.wPoz_x = (ReceiverBufferRS232[5] << 8) | ReceiverBufferRS232[4];
                                                  //sNewCross.wPoz_y = (ReceiverBufferRS232[7] << 8) | ReceiverBufferRS232[6];
                                                  CDA_RAL_X = ReceiverBufferRS232[3];
                                                  CDA_RAL_Y = ReceiverBufferRS232[4];

                                                  switch(CDA_RAL_X)
                                                  {
                                                      case 0: break;
                                                      case 1: if (sNewRAL.wPozX + 1 < sNewRAM.wPozBX + FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)//((sNewRAL.wPozX + 1 > sNewRAM.wPozBX - FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) &&
                                                                 sNewRAL.wPozX = sNewRAL.wPozX + 1;break;
                                                      case 2: if (sNewRAL.wPozX - 1 > sNewRAM.wPozBX - FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)// && (sNewRAL.wPozX - 1 < sNewRAM.wPozBX + FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM))
                                                                 sNewRAL.wPozX = sNewRAL.wPozX - 1;break;
                                                      case 3: if (sNewRAL.wPozX + 5 < sNewRAM.wPozBX + FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)//((sNewRAL.wPozX + 5 > sNewRAM.wPozBX - FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) &&
                                                                 sNewRAL.wPozX = sNewRAL.wPozX + 5;break;
                                                      case 4: if (sNewRAL.wPozX - 5 > sNewRAM.wPozBX - FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)// && (sNewRAL.wPozX - 5 < sNewRAM.wPozBX + FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM))
                                                                 sNewRAL.wPozX = sNewRAL.wPozX - 5;break;
                                                      default: break;
                                                  }

                                                  switch(CDA_RAL_Y)
                                                  {
                                                      case 0: break;
                                                      case 1: if (sNewRAL.wPozY - 1 > sNewRAM.wPozBY - FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)// && (sNewRAL.wPozY - 1 < sNewRAM.wPozBY + FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM))
                                                                 sNewRAL.wPozY = sNewRAL.wPozY - 1;break;
                                                      case 2: if (sNewRAL.wPozY + 1 < sNewRAM.wPozBY + FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)//((sNewRAL.wPozY + 1 > sNewRAM.wPozBY - FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) &&
                                                                 sNewRAL.wPozY = sNewRAL.wPozY + 1;break;
                                                      case 3: if (sNewRAL.wPozY - 5 > sNewRAM.wPozBY - FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)// && (sNewRAL.wPozY - 5 < sNewRAM.wPozBY + FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM))
                                                                 sNewRAL.wPozY = sNewRAL.wPozY - 5;break;
                                                      case 4: if (sNewRAL.wPozY + 5 < sNewRAM.wPozBY + FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM)//((sNewRAL.wPozY + 5 > sNewRAM.wPozBY - FACTOR_LIMITA_RAL * FACTOR_DE_SCALARE_RPM/ sNewRAM.wRPM) &&
                                                                 sNewRAL.wPozY = sNewRAL.wPozY + 5;break;
                                                      default: break;
                                                  }

                                                  sNewRAL.wBrat = (U16)((BRAT_RAL * FACTOR_DE_SCALARE_RPM) / sNewRAL.wRPM);

                                                  bMODE = 2;
                                                  CrossStatus = CROSS_STEP1;
                                                  DRALScheduler = CROSS_STEP1;
                                                  FillScheduler = FILL_STEP1;
                                                  CodEroare |= (1 << BIT_STATUS_OK);
                                                  return TRUE;
                                                  break;
                         case DISPLAY_RAM:
                                                  sNewRGrid.StartMoving = STOP_MOVING;
                                                  CDA_RAM_X = ReceiverBufferRS232[3];
                                                  CDA_RAM_Y = ReceiverBufferRS232[4];
                                                  ETAL_RAM_X = ReceiverBufferRS232[5];
                                                  //sNewFrame.bPoz_x = 100;
                                                  //sNewFrame.bPoz_y = 100;

                                                  switch(CDA_RAM_X)
                                                  {
                                                      case 1:    if (((2*sNewRAM.swOffsetX + sNewRAM.wLgX + 1) <= DEFAULT_RAM_MAXIMUM_X) && ((2*sNewRAM.swOffsetX - sNewRAM.wLgX + 1) >= -DEFAULT_RAM_MAXIMUM_X))
                                                                       sNewRAM.swOffsetX = sNewRAM.swOffsetX + 1; break;
                                                      case 2:    if (((2*sNewRAM.swOffsetX + sNewRAM.wLgX - 1) <= DEFAULT_RAM_MAXIMUM_X) && ((2*sNewRAM.swOffsetX - sNewRAM.wLgX - 1) >= -DEFAULT_RAM_MAXIMUM_X))
                                                                       sNewRAM.swOffsetX = sNewRAM.swOffsetX - 1; break;
                                                      case 3:    if (((2*sNewRAM.swOffsetX + sNewRAM.wLgX + 4) <= DEFAULT_RAM_MAXIMUM_X) && ((2*sNewRAM.swOffsetX - sNewRAM.wLgX + 4) >= -DEFAULT_RAM_MAXIMUM_X))
                                                                       sNewRAM.swOffsetX = sNewRAM.swOffsetX + 4; break;
                                                      case 4:    if (((2*sNewRAM.swOffsetX + sNewRAM.wLgX - 4) <= DEFAULT_RAM_MAXIMUM_X) && ((2*sNewRAM.swOffsetX - sNewRAM.wLgX - 4) >= -DEFAULT_RAM_MAXIMUM_X))
                                                                       sNewRAM.swOffsetX = sNewRAM.swOffsetX - 4; break;
                                                      default:   break;
                                                  }

                                                  switch(CDA_RAM_Y)
                                                  {
                                                      case 1:    if (((2*sNewRAM.swOffsetY + 2*sNewRAM.wLgX + 1) <= DEFAULT_RAM_MAXIMUM_Y) && ((2*sNewRAM.swOffsetY - 2*sNewRAM.wLgX + 1) >= -DEFAULT_RAM_MAXIMUM_Y))
                                                                       sNewRAM.swOffsetY = sNewRAM.swOffsetY + 1; break;
                                                      case 2:    if (((2*sNewRAM.swOffsetY + 2*sNewRAM.wLgX - 1) <= DEFAULT_RAM_MAXIMUM_Y) && ((2*sNewRAM.swOffsetY - 2*sNewRAM.wLgX - 1) >= -DEFAULT_RAM_MAXIMUM_Y))
                                                                       sNewRAM.swOffsetY = sNewRAM.swOffsetY - 1; break;
                                                      case 3:    if (((2*sNewRAM.swOffsetY + 2*sNewRAM.wLgX + 4) <= DEFAULT_RAM_MAXIMUM_Y) && ((2*sNewRAM.swOffsetY - 2*sNewRAM.wLgX + 4) >= -DEFAULT_RAM_MAXIMUM_Y))
                                                                       sNewRAM.swOffsetY = sNewRAM.swOffsetY + 4; break;
                                                      case 4:    if (((2*sNewRAM.swOffsetY + 2*sNewRAM.wLgX - 4) <= DEFAULT_RAM_MAXIMUM_Y) && ((2*sNewRAM.swOffsetY - 2*sNewRAM.wLgX - 4) >= -DEFAULT_RAM_MAXIMUM_Y))
                                                                       sNewRAM.swOffsetY = sNewRAM.swOffsetY - 4; break;
                                                  default:   break;
                                                  }

                                                  switch(ETAL_RAM_X)
                                                  {
                                                      case 1:    if (((2*abs(sNewRAM.swOffsetX) + sNewRAM.wLgX + 2) <= DEFAULT_RAM_MAXIMUM_X) && ((2*abs(sNewRAM.swOffsetY) + 2*sNewRAM.wLgX + 2) <= DEFAULT_RAM_MAXIMUM_Y))
                                                                 sNewRAM.wLgX = sNewRAM.wLgX + 2; break;
                                                      case 2:    if ((sNewRAM.wLgX - 2) >= DEFAULT_RAM_MINIM_LGX)
                                                                 sNewRAM.wLgX = sNewRAM.wLgX - 2; break;
                                                      case 3:    if (((2*abs(sNewRAM.swOffsetX) + sNewRAM.wLgX + 8) <= DEFAULT_RAM_MAXIMUM_X) && ((2*abs(sNewRAM.swOffsetY) + 2*sNewRAM.wLgX + 8) <= DEFAULT_RAM_MAXIMUM_Y))
                                                                 sNewRAM.wLgX = sNewRAM.wLgX + 8; break;
                                                      case 4:    if ((sNewRAM.wLgX - 8) >= DEFAULT_RAM_MINIM_LGX)
                                                                 sNewRAM.wLgX = sNewRAM.wLgX - 8; break;
                                                      default:   break;
                                                  }
                                                  sNewRAM.wRPM = (50 * FACTOR_DE_SCALARE_RPM /sNewRAM.wLgX);
                                                  sNewRAE.wRPM = sNewRAM.wRPM;
                                                  sNewRAL.wRPM = sNewRAM.wRPM;
                                                  sNewRStandard.wRPM = sNewRAM.wRPM;
                                                  sNewRGrid.wRPM = sNewRAM.wRPM;

                                                  sNewRAM.wPozAX = DISPLAY_RESOLUTION_X_90 / 2 + sNewRAM.swOffsetX;
                                                  sNewRAM.wPozAY = DISPLAY_RESOLUTION_Y_90 / 2 - sNewRAM.swOffsetY - sNewRAM.wLgX / 2;
                                                  sNewRAM.wPozBX = DISPLAY_RESOLUTION_X_90 / 2 + sNewRAM.swOffsetX;
                                                  sNewRAM.wPozBY = DISPLAY_RESOLUTION_Y_90 / 2 - sNewRAM.swOffsetY;

                                                  bMODE = 3;
                                                  FrameScheduler =  FRAME_STEP1;
                                                  DRAMScheduler = FRAME_STEP1;
                                                  FillScheduler = FILL_STEP1;
                                                  CodEroare |= (1 << BIT_STATUS_OK);
                                                  return TRUE;
                                                  break;
                         case DISPLAY_STANDARD:
                                                  sNewRGrid.StartMoving = STOP_MOVING;
                                                  sNewRStandard.cTm[0] = ReceiverBufferRS232[3];
                                                  sNewRStandard.cTm[1] = ReceiverBufferRS232[4];
                                                  sNewRStandard.cTm[2] = ReceiverBufferRS232[5];
                                                  sNewRStandard.cErr[0] = 'E';
                                                  sNewRStandard.cErr[1] = 'R';
                                                  sNewRStandard.cErr[2] = 'R';
                                                  sNewRStandard.cErr[3] = '=';
                                                  sNewRStandard.cErr[4] = 0x30 + (ReceiverBufferRS232[6] >> 4);
                                                  sNewRStandard.cErr[5] = 0x30 + (ReceiverBufferRS232[6] & 0x0f);
                                                  //sNewRStandard.wPozRoX = ReceiverBufferRS232[7] | (ReceiverBufferRS232[8] << 8);
                                                  //sNewRStandard.wPozRoY = ReceiverBufferRS232[9] | (ReceiverBufferRS232[10] << 8);
                                                  CDA_RO_X = ReceiverBufferRS232[7] | (ReceiverBufferRS232[8] << 8);
                                                  CDA_RO_Y = ReceiverBufferRS232[9] | (ReceiverBufferRS232[10] << 8);
                                                  sNewRStandard.cDataX[0] = 0x30 + (ReceiverBufferRS232[11] >> 4);
                                                  sNewRStandard.cDataX[1] = 0x30 + (ReceiverBufferRS232[11] & 0x0f);
                                                  sNewRStandard.cDataX[2] = 0x30 + (ReceiverBufferRS232[12] >> 4);
                                                  sNewRStandard.cDataX[3] = 0x30 + (ReceiverBufferRS232[12] & 0x0f);
                                                  sNewRStandard.cDataY[0] = 0x30 + (ReceiverBufferRS232[13] >> 4);
                                                  sNewRStandard.cDataY[1] = 0x30 + (ReceiverBufferRS232[13] & 0x0f);
                                                  sNewRStandard.cDataY[2] = 0x30 + (ReceiverBufferRS232[14] >> 4);
                                                  sNewRStandard.cDataY[3] = 0x30 + (ReceiverBufferRS232[14] & 0x0f);
                                                  sNewRStandard.bFlags = ReceiverBufferRS232[15];

                                                  sNewRStandard.wPozX = (DISPLAY_RESOLUTION_X_90 - sNewRAM.wLgX)/2;
                                                  sNewRStandard.wPozY = DISPLAY_RESOLUTION_Y_90/2 - sNewRAM.wLgX;
                                                  sNewRStandard.swOffsetX = sNewRAM.swOffsetX;
                                                  sNewRStandard.swOffsetY = sNewRAM.swOffsetY;

                                                  //SCALARE DIN MIIMI IN PIXELI - CDA_RO_X este transformat din zecimi de miimi in pixeli
                                                  sNewRStandard.wPozRoX = sNewRAE.wPozX + CDA_RO_X * FACTOR_DE_SCALARE_RPM / (10 * sNewRStandard.wRPM) ;
                                                  sNewRStandard.wPozRoY = sNewRAE.wPozY - CDA_RO_Y * FACTOR_DE_SCALARE_RPM / (10 * sNewRStandard.wRPM);

                                                  //DIMENSIUNILE REPERULUI DE OCHIRE ESTE SCALAT IN FUNCTIE DE PERIMETRUL RPM
                                                  sNewRStandard.bLgRoX = (U16)((RO_LG_X * FACTOR_DE_SCALARE_RPM) / sNewRStandard.wRPM);
                                                  sNewRStandard.bLgRoY = (U16)((RO_LG_Y * FACTOR_DE_SCALARE_RPM) / sNewRStandard.wRPM);

                                                  bMODE = 4;
                                                  RefreshOfStandardData = NO_REFRESH;
                                                  StandardScheduler = STANDARD_STEP1;
                                                  DDScheduler = STANDARD_STEP1;
                                                  CursorScheduler = CURSOR_STEP1;
                                                  FillScheduler = FILL_STEP1;
                                                  CodEroare |= (1 << BIT_STATUS_OK);
                                                  return TRUE;
                                                  break;
                         case DISPLAY_GRID:
                                                  //sNewGrid.bViteza_gm = ReceiverBufferRS232[3];
                                                  //sNewGrid.bSign = ReceiverBufferRS232[4];

                                                  sNewRGrid.bViteza_gm = (ReceiverBufferRS232[3] * FACTOR_DE_SCALARE_RPM) /( 10 * sNewRGrid.wRPM);
                                                  sNewRGrid.bSign = ReceiverBufferRS232[4];
                                                  sNewRGrid.wClearance = (GRID_CLEARANCE * FACTOR_DE_SCALARE_RPM) / sNewRGrid.wRPM;
                                                  sNewRGrid.wPozX = DISPLAY_RESOLUTION_X_90 / 2 - sNewRAM.wLgX / 2 + sNewRAM.swOffsetX + 15 * FACTOR_DE_SCALARE_RPM / sNewRGrid.wRPM;

                                                  bMODE = 5;
                                                  GridScheduler = GRID_STEP1;
                                                  DGScheduler = GRID_STEP1;
                                                  FillScheduler = FILL_STEP1;
                                                  CodEroare |= (1 << BIT_STATUS_OK);
                                                  return TRUE;
                                                  break;
                         case DISPLAY_TEST:
                                                  sNewRGrid.StartMoving = STOP_MOVING;
                                                  bNewTest = ReceiverBufferRS232[3];

                                                  bMODE = 6;
                                                  DTScheduler = TEST_STEP1;
                                                  CodEroare |= (1 << BIT_STATUS_OK);
                                                  return TRUE;
                                                  break;
                         default:
                                                  CodEroare |= (1 << BIT_CMD_CODE_ERROR);
                                                  return FALSE;
                                                  break;
                     }
                 }else {CodEroare |= (1 << BIT_CKS_ERROR);}
            }else {CodEroare |= (1 << BIT_LG_ERROR);}
        }
    }
    return FALSE;
}



U8 draw_figures(void)
{

    switch(NewCmdCode)
    {
    case DISPLAY_OFF:
                              if (CmdCode != NewCmdCode)
                              {
                                  CmdCode = NewCmdCode;
                                  switch_off_display();
                                  set_start_memory_address_cursor_and_switch_off_the_cursor(LOCATION_NO_OF_CURSOR_1);
                              }
                              return TRUE;
                              break;
    case DISPLAY_RAL:         if (display_RAL()) return TRUE;break;
    case DISPLAY_RAE:         if (display_RAE()) return TRUE;break;
    case DISPLAY_RAM:         if (display_RAM()) return TRUE;break;
    case DISPLAY_STANDARD:    if (display_standard()) return TRUE;break;
    case DISPLAY_GRID:        if (display_grid())return TRUE;break;
    case DISPLAY_TEST:        if (display_test())return TRUE;break;
    default:                  return TRUE;break;
    }
    return FALSE;
}


void load_transmission_buffer(U8 os, U8 code)
{
    TransmissionBufferRS232[0] = 0x0A;
    TransmissionBufferRS232[1] = 0x0B;
    TransmissionBufferRS232[2] = NewCmdCode;
    TransmissionBufferRS232[3] = os;

    switch(NewCmdCode)
    {
       case DISPLAY_STATUS:      break;
       case DISPLAY_OFF:
                                 TransmissionBufferRS232[4] = 0;
                                 TransmissionBufferRS232[5] = 0;
                                 TransmissionBufferRS232[6] = 0;
                                 TransmissionBufferRS232[7] = 0;
                                 TransmissionBufferRS232[8] = 0;
                                 TransmissionBufferRS232[9] = 0;
                                 break;
       case DISPLAY_RAE:
                                 // PARAMETRII SUNT SCALATI PENTRU A SE AFISA ZECIMI DE MIIMI
                                 TransmissionBufferRS232[4] = 0;
                                 TransmissionBufferRS232[5] = (((sNewRAE.wPozX - sNewRAM.wPozAX) * 10 * sNewRAE.wRPM) / FACTOR_DE_SCALARE_RPM);
                                 TransmissionBufferRS232[6] = (((sNewRAE.wPozX - sNewRAM.wPozAX) * 10 * sNewRAE.wRPM) / FACTOR_DE_SCALARE_RPM) >> 8;
                                 TransmissionBufferRS232[7] = (((sNewRAM.wPozAY - sNewRAE.wPozY) * 10 * sNewRAE.wRPM) / FACTOR_DE_SCALARE_RPM);
                                 TransmissionBufferRS232[8] = (((sNewRAM.wPozAY - sNewRAE.wPozY) * 10 * sNewRAE.wRPM) / FACTOR_DE_SCALARE_RPM) >> 8;
                                 TransmissionBufferRS232[9] = 0;
                                 break;
       case DISPLAY_RAL:
                                 // PARAMETRII SUNT SCALATI PENTRU A SE AFISA ZECIMI DE MIIMI
                                 TransmissionBufferRS232[4] = 0;
                                 TransmissionBufferRS232[5] = (((sNewRAL.wPozX - sNewRAM.wPozBX) * 10 * sNewRAL.wRPM) / FACTOR_DE_SCALARE_RPM);
                                 TransmissionBufferRS232[6] = (((sNewRAL.wPozX - sNewRAM.wPozBX) * 10 * sNewRAL.wRPM) / FACTOR_DE_SCALARE_RPM) >> 8;
                                 TransmissionBufferRS232[7] = (((sNewRAM.wPozBY - sNewRAL.wPozY) * 10 * sNewRAL.wRPM) / FACTOR_DE_SCALARE_RPM);
                                 TransmissionBufferRS232[8] = (((sNewRAM.wPozBY - sNewRAL.wPozY) * 10 * sNewRAL.wRPM) / FACTOR_DE_SCALARE_RPM) >> 8;
                                 TransmissionBufferRS232[9] = 0;
                                 break;
       case DISPLAY_RAM:
                                 TransmissionBufferRS232[4] = 0;
                                 TransmissionBufferRS232[5] = sNewRAM.swOffsetX;
                                 TransmissionBufferRS232[6] = sNewRAM.swOffsetX >> 8;
                                 TransmissionBufferRS232[7] = sNewRAM.swOffsetY;
                                 TransmissionBufferRS232[8] = sNewRAM.swOffsetY >> 8;
                                 TransmissionBufferRS232[9] = (sNewRAM.wRPM)/100;
                                 break;
       case DISPLAY_STANDARD:
                                 TransmissionBufferRS232[4] = 0;
                                 TransmissionBufferRS232[5] = (sNewRAE.wPozX - sNewRStandard.wPozRoX);
                                 TransmissionBufferRS232[6] = (sNewRAE.wPozX - sNewRStandard.wPozRoX) >> 8;
                                 TransmissionBufferRS232[7] = (sNewRAE.wPozY - sNewRStandard.wPozRoY);
                                 TransmissionBufferRS232[8] = (sNewRAE.wPozY - sNewRStandard.wPozRoY) >> 8;
                                 TransmissionBufferRS232[9] = 0;
                                 break;
       case DISPLAY_GRID:
                                 TransmissionBufferRS232[4] = 0;
                                 TransmissionBufferRS232[5] = sNewRGrid.bViteza_gm;
                                 TransmissionBufferRS232[6] = 0;
                                 TransmissionBufferRS232[7] = 0;
                                 TransmissionBufferRS232[8] = 0;
                                 TransmissionBufferRS232[9] = 0;
                                 break;
       case DISPLAY_TEST:
                                 TransmissionBufferRS232[4] = 0;
                                 TransmissionBufferRS232[5] = 0;
                                 TransmissionBufferRS232[6] = 0;
                                 TransmissionBufferRS232[7] = 0;
                                 TransmissionBufferRS232[8] = 0;
                                 TransmissionBufferRS232[9] = 0;
                                 break;
       default:                  break;
    }
    TransmissionBufferRS232[10] = (U8)(0x15 + TransmissionBufferRS232[2] + TransmissionBufferRS232[3]+
                                              TransmissionBufferRS232[4] + TransmissionBufferRS232[5]+
                                              TransmissionBufferRS232[6] + TransmissionBufferRS232[7]+
                                              TransmissionBufferRS232[8] + TransmissionBufferRS232[9]);

}

void load_transmission_buffer_with_status(U8 os, U8 mode)
{
    TransmissionBufferRS232[0] = 0x0A;
    TransmissionBufferRS232[1] = 0x0B;
    TransmissionBufferRS232[2] = 0x00;
    TransmissionBufferRS232[3] = os;
    TransmissionBufferRS232[4] = mode;
    TransmissionBufferRS232[5] = ((10*sNewRAM.wRPM*(sNewRAL.wPozX - sNewRAE.wPozX)) / FACTOR_DE_SCALARE_RPM)>>8;
    TransmissionBufferRS232[6] = ((10*sNewRAM.wRPM*(sNewRAL.wPozX - sNewRAE.wPozX)) / FACTOR_DE_SCALARE_RPM);
    TransmissionBufferRS232[7] = ((10*sNewRAM.wRPM*(sNewRAL.wPozY - sNewRAE.wPozY)) / FACTOR_DE_SCALARE_RPM)>>8;
    TransmissionBufferRS232[8] = ((10*sNewRAM.wRPM*(sNewRAL.wPozY - sNewRAE.wPozY)) / FACTOR_DE_SCALARE_RPM);
    TransmissionBufferRS232[9] = 0;
    TransmissionBufferRS232[10] = (U8)(0x15 + os + mode + TransmissionBufferRS232[5]+
                                       TransmissionBufferRS232[6]+TransmissionBufferRS232[7]+
                                       TransmissionBufferRS232[8]+TransmissionBufferRS232[9]);
}

U8 send_data_to_RS232(void)
{
    int c;

    c = TransmissionBufferRS232[TransmissionIndex];
    if (usart_write_char(XVGA_USART,c) == USART_SUCCESS)
    {
        if (TransmissionIndex++ == TransmissionBufferRS232[1] - 1) return TRUE;
    }
    return FALSE;
}


U8 display_RAL(void)
{
    switch(DRALScheduler)
    {
        case CROSS_STEP1:
            if (NewCmdCode != CmdCode)
            {
                if (fill_memory_view_with_color(START_ADDRESS_PANEL1,DISPLAY_BACKGROUND_COLOR))
                {
                    store_values_position_of_refernce();
                    CmdCode = NewCmdCode;
                    set_start_memory_address_cursor_and_switch_off_the_cursor(LOCATION_NO_OF_CURSOR_1);
                    set_start_address_of_image(0);
                    switch_on_display();
                    DRALScheduler = CROSS_STEP2;
                }
            }else DRALScheduler = CROSS_STEP3;
            break;
        case CROSS_STEP2:
            if (draw_the_RAL_90(START_ADDRESS_PANEL1, &sNewRAL, DISPLAY_FOREGROUND_COLOR)) DRALScheduler = CROSS_STEP6;
            break;
        case CROSS_STEP3:
            if ((sRAL.wPozX != sNewRAL.wPozX) || (sRAL.wPozY != sNewRAL.wPozY)) DRALScheduler = CROSS_STEP4;
            else {switch_on_display();return TRUE;}
            break;
        case CROSS_STEP4:
            if (draw_the_RAL_90(START_ADDRESS_PANEL1, &sRAL, DISPLAY_BACKGROUND_COLOR)) {DRALScheduler = CROSS_STEP5;CrossStatus = CROSS_STEP1;}
            break;
        case CROSS_STEP5:
            if (draw_the_RAL_90(START_ADDRESS_PANEL1, &sNewRAL, DISPLAY_FOREGROUND_COLOR)) DRALScheduler = CROSS_STEP6;
            break;
        case CROSS_STEP6:
            //switch_on_display();
            sRAL.wPozX = sNewRAL.wPozX;
            sRAL.wPozY = sNewRAL.wPozY;
            sRAL.wBrat = sNewRAL.wBrat;
            return TRUE;
            break;
        default:
            DRALScheduler = CROSS_STEP1;
            break;
    }
    return FALSE;
}

U8 display_RAE(void)
{
    switch(DRAEScheduler)
    {
        case CROSS_STEP1:
            if (NewCmdCode != CmdCode)
            {
                if (fill_memory_view_with_color(START_ADDRESS_PANEL1,DISPLAY_BACKGROUND_COLOR))
                {
                    store_values_position_of_refernce();
                    CmdCode = NewCmdCode;
                    set_start_memory_address_cursor_and_switch_off_the_cursor(LOCATION_NO_OF_CURSOR_1);
                    set_start_address_of_image(0);
                    switch_on_display();
                    DRAEScheduler = CROSS_STEP2;
                }
            }else DRAEScheduler = CROSS_STEP3;
            break;
        case CROSS_STEP2:
            if (draw_the_RAE_90(START_ADDRESS_PANEL1, &sNewRAE, DISPLAY_FOREGROUND_COLOR)) DRAEScheduler = CROSS_STEP6;
            break;
        case CROSS_STEP3:
            if ((sRAE.wPozX != sNewRAE.wPozX) || (sRAE.wPozY != sNewRAE.wPozY)) DRAEScheduler = CROSS_STEP4;
            else {switch_on_display();return TRUE;}
            break;
        case CROSS_STEP4:
            if (draw_the_RAE_90(START_ADDRESS_PANEL1, &sRAE, DISPLAY_BACKGROUND_COLOR)) {DRAEScheduler = CROSS_STEP5;RAEStatus = CROSS_STEP1;}
            break;
        case CROSS_STEP5:
            if (draw_the_RAE_90(START_ADDRESS_PANEL1, &sNewRAE, DISPLAY_FOREGROUND_COLOR)) DRAEScheduler = CROSS_STEP6;
            break;
        case CROSS_STEP6:
            //switch_on_display();
            sRAE.wPozX = sNewRAE.wPozX;
            sRAE.wPozY = sNewRAE.wPozY;
            sRAE.wBratX = sNewRAE.wBratX;
            sRAE.wBratY = sNewRAE.wBratY;
            return TRUE;
            break;
        default:
        	DRAEScheduler = CROSS_STEP1;
            break;
    }
	return FALSE;
}


U8 display_RAM(void)
{
    switch(DRAMScheduler)
    {
        case FRAME_STEP1:
            if (NewCmdCode != CmdCode)
            {
                if (fill_memory_view_with_color(START_ADDRESS_PANEL1,DISPLAY_BACKGROUND_COLOR))
                {
                    store_values_position_of_refernce();
                    CmdCode = NewCmdCode;
                    //sFrame.bPoz_x = sNewFrame.bPoz_x;
                    //sFrame.bPoz_y = sNewFrame.bPoz_y;
                    set_start_memory_address_cursor_and_switch_off_the_cursor(LOCATION_NO_OF_CURSOR_1);
                    set_start_address_of_image(0);
                    switch_on_display();
                    DRAMScheduler = FRAME_STEP2;
                }
            }else DRAMScheduler = FRAME_STEP3;
            break;
        case FRAME_STEP2:
            if (draw_the_frame_90(START_ADDRESS_PANEL1, &sNewRAM, DISPLAY_FOREGROUND_COLOR)) DRAMScheduler = FRAME_STEP6;//{set_start_address_of_image(0);switch_on_display();return TRUE;}
            break;
        case FRAME_STEP3:
            if ((sRAM.wLgX != sNewRAM.wLgX) || (sRAM.swOffsetX != sNewRAM.swOffsetX ) || (sRAM.swOffsetY != sNewRAM.swOffsetY)) DRAMScheduler = FRAME_STEP4;
            else {switch_on_display();return TRUE;}
            break;
        case FRAME_STEP4:
            if (draw_the_frame_90(START_ADDRESS_PANEL1, &sRAM, DISPLAY_BACKGROUND_COLOR)) {DRAMScheduler = FRAME_STEP5;FrameScheduler = FRAME_STEP1;}
            break;
        case FRAME_STEP5:
            if (draw_the_frame_90(START_ADDRESS_PANEL1, &sNewRAM, DISPLAY_FOREGROUND_COLOR)) DRAMScheduler = FRAME_STEP6;
            break;
        case FRAME_STEP6:
            //switch_on_display();
            sRAM.wLgX = sNewRAM.wLgX;
            sRAM.swOffsetX = sNewRAM.swOffsetX;
            sRAM.swOffsetY = sNewRAM.swOffsetY;

            sRAM.wPozAX =  sNewRAM.wPozAX;
            sRAM.wPozAY =  sNewRAM.wPozAY;
            sRAM.wPozBX =  sNewRAM.wPozBX;
            sRAM.wPozBY =  sNewRAM.wPozBY;
            return TRUE;
            break;
        default:
            DRAMScheduler = FRAME_STEP1;
            break;
    }
    return FALSE;
}


U8 display_standard(void)
{
    switch(DDScheduler)
    {
        case STANDARD_STEP1:
            if (NewCmdCode != CmdCode)
            {
                if (fill_memory_view_with_color(START_ADDRESS_PANEL1,DISPLAY_BACKGROUND_COLOR))
                {
                    store_values_position_of_refernce();
                    CmdCode = NewCmdCode;
                    TimerForClipping = 0;
                    RefreshOfStandardData |= ((1 << BIT_REFRESH_OF_FRAME_POSITION));
                    DDScheduler = STANDARD_STEP2;
                }
            }else DDScheduler = STANDARD_STEP5;
            break;
        case STANDARD_STEP2:
            // afiseaza fereastra standard
            if (draw_the_standard_90(START_ADDRESS_PANEL1, &sNewRStandard, DISPLAY_FOREGROUND_COLOR, RefreshOfStandardData)) {set_start_address_of_image(0);switch_on_display();DDScheduler = STANDARD_STEP3;}
            break;
        case STANDARD_STEP3:
            // deseneaza cursorul im memorie
            if (draw_frame_cursor(MEMORY_ADDRESS_OF_CURSOR_1,&sNewRStandard))DDScheduler = STANDARD_STEP4; //return TRUE;
            break;
        case STANDARD_STEP4:
            set_position_of_cursor(sNewRStandard.wPozRoY - MAX_CURSOR_RESOLUTION/2,sNewRStandard.wPozRoX - MAX_CURSOR_RESOLUTION/2);
            // afiseaza sau nu cursorul in functie de valoarea flagului
            if (sNewRStandard.bFlags & (1 << BIT_DISPLAY_RO)) set_start_memory_address_cursor_and_switch_on_the_cursor(LOCATION_NO_OF_CURSOR_1);
            DDScheduler = STANDARD_STEP9;
        	break;
        case STANDARD_STEP5:
            if ((sRStandard.wPozX != sNewRStandard.wPozX) || (sRStandard.wPozY != sNewRStandard.wPozY))
                 RefreshOfStandardData |= ((1 << BIT_REFRESH_OF_FRAME_POSITION) | (1 << BIT_REFRESH_OF_ERR));
            if ((sRStandard.cTm[0] != sNewRStandard.cTm[0]) || (sRStandard.cTm[1] != sNewRStandard.cTm[1]) ||
                (sRStandard.cTm[2] != sNewRStandard.cTm[2])) RefreshOfStandardData |= BIT_REFRESH_OF_TM;
            if ((sRStandard.cErr[4] != sNewRStandard.cErr[4]) || (sRStandard.cErr[5] != sNewRStandard.cErr[5]))
                 RefreshOfStandardData |= (1 << BIT_REFRESH_OF_ERR);
            if ((sRStandard.cDataX[0] != sNewRStandard.cDataX[0]) || (sRStandard.cDataX[1] != sNewRStandard.cDataX[1]) ||
                (sRStandard.cDataX[2] != sNewRStandard.cDataX[2]) || (sRStandard.cDataX[3] != sNewRStandard.cDataX[3]))
                 RefreshOfStandardData |= (1 << BIT_REFRESH_OF_X);
            if ((sRStandard.cDataY[0] != sNewRStandard.cDataY[0]) || (sRStandard.cDataY[1] != sNewRStandard.cDataY[1]) ||
                (sRStandard.cDataY[2] != sNewRStandard.cDataY[2]) || (sRStandard.cDataY[3] != sNewRStandard.cDataY[3]))
                RefreshOfStandardData |= (1 << BIT_REFRESH_OF_Y);

            if ((sRStandard.bFlags & MSK_READY_AND_ERR_FLAGS) != (sNewRStandard.bFlags & MSK_READY_AND_ERR_FLAGS))
            	RefreshOfStandardData |= BIT_REFRESH_OF_FLAGS;

            // ecranul trebuie actualizat
            if (RefreshOfStandardData != NO_REFRESH)
            {DDScheduler = STANDARD_STEP7; if (RefreshOfStandardData & (1 << BIT_REFRESH_OF_FRAME_POSITION)){DDScheduler = STANDARD_STEP6; DeleteScheduler = DELETE_STEP1;}}
            // nici o data nu s-a schimbat; ecranul nu trebuie actualizat
            else {DDScheduler = STANDARD_STEP8;}

            break;
        case STANDARD_STEP6:
            if (delete_character_of_standard_view(START_ADDRESS_PANEL1,&sRStandard, DISPLAY_BACKGROUND_COLOR)) DDScheduler = STANDARD_STEP7;
            break;
        case STANDARD_STEP7:
            if (draw_the_standard_90(START_ADDRESS_PANEL1, &sNewRStandard, DISPLAY_FOREGROUND_COLOR, RefreshOfStandardData)) DDScheduler = STANDARD_STEP8;
            break;
        case STANDARD_STEP8:
        	// trateaza pozitia cursorului
            if ((sRStandard.wPozRoX != sNewRStandard.wPozRoX) || (sRStandard.wPozRoY != sNewRStandard.wPozRoY))
                {set_position_of_cursor(sNewRStandard.wPozRoY - MAX_CURSOR_RESOLUTION/2,sNewRStandard.wPozRoX - MAX_CURSOR_RESOLUTION/2);}

            if (((sRStandard.bFlags & (1 << BIT_DISPLAY_RO)) != (sNewRStandard.bFlags & (1 << BIT_DISPLAY_RO))) && (sNewRStandard.bFlags & (1 << BIT_DISPLAY_RO)))
            {set_start_memory_address_cursor_and_switch_on_the_cursor(LOCATION_NO_OF_CURSOR_1);}
            else if (((sRStandard.bFlags & (1 << BIT_DISPLAY_RO)) != (sNewRStandard.bFlags & (1 << BIT_DISPLAY_RO))) && (!(sNewRStandard.bFlags & (1 << BIT_DISPLAY_RO))))
            {set_start_memory_address_cursor_and_switch_off_the_cursor(LOCATION_NO_OF_CURSOR_1);}

            if (((sRStandard.bFlags & (1 << BIT_FLASHING_RO)) != (sNewRStandard.bFlags & (1 << BIT_FLASHING_RO))) && (sNewRStandard.bFlags & (1 << BIT_DISPLAY_RO)))
            {set_start_memory_address_cursor_and_switch_on_the_cursor(LOCATION_NO_OF_CURSOR_1);}

            if (((sRStandard.bFlags & (1 << BIT_FLASHING_RO)) != (sNewRStandard.bFlags & (1 << BIT_FLASHING_RO))) && (sNewRStandard.bFlags & (1 << BIT_FLASHING_RO)))
            TimerForClipping = 0;

            DDScheduler = STANDARD_STEP9;
            break;
        case STANDARD_STEP9:
            //switch_on_display();

            sRStandard.cTm[0] = sNewRStandard.cTm[0];
            sRStandard.cTm[1] = sNewRStandard.cTm[1];
            sRStandard.cTm[2] = sNewRStandard.cTm[2];
            sRStandard.cErr[0] = sNewRStandard.cErr[0];
            sRStandard.cErr[1] = sNewRStandard.cErr[1];
            sRStandard.cErr[2] = sNewRStandard.cErr[2];
            sRStandard.cErr[3] = sNewRStandard.cErr[3];
            sRStandard.cErr[4] = sNewRStandard.cErr[4];
            sRStandard.cErr[5] = sNewRStandard.cErr[5];
            sRStandard.wPozRoX = sNewRStandard.wPozRoX;
            sRStandard.wPozRoY = sNewRStandard.wPozRoY;
            sRStandard.cDataX[0] = sNewRStandard.cDataX[0];
            sRStandard.cDataX[1] = sNewRStandard.cDataX[1];
            sRStandard.cDataX[2] = sNewRStandard.cDataX[2];
            sRStandard.cDataX[3] = sNewRStandard.cDataX[3];
            sRStandard.cDataY[0] = sNewRStandard.cDataY[0];
            sRStandard.cDataY[1] = sNewRStandard.cDataY[1];
            sRStandard.cDataY[2] = sNewRStandard.cDataY[2];
            sRStandard.cDataY[3] = sNewRStandard.cDataY[3];
            sRStandard.wPozX = sNewRStandard.wPozX;
            sRStandard.wPozY = sNewRStandard.wPozY;
            sRStandard.bFlags = sNewRStandard.bFlags;
            return TRUE;
            break;
        default:
            DDScheduler = STANDARD_STEP1;
            break;
    }
    return FALSE;
}


U8 display_grid(void)
{
   switch(DGScheduler)
    {
        case GRID_STEP1:
            if (NewCmdCode != CmdCode)
            {
                if (fill_memory_view_with_color(START_ADDRESS_PANEL1,DISPLAY_BACKGROUND_COLOR))
                {
                    CmdCode = NewCmdCode;
                    set_start_memory_address_cursor_and_switch_off_the_cursor(LOCATION_NO_OF_CURSOR_1);
                    DGScheduler = GRID_STEP2;
                }
            }else DGScheduler = GRID_STEP3;
            break;
        case GRID_STEP2:
            if (draw_the_grid_90(START_ADDRESS_PANEL1, &sNewRGrid, DISPLAY_FOREGROUND_COLOR))
            {if (sNewRGrid.bSign == 0) sNewRGrid.AdresaGrid = sNewRGrid.wClearance * DISPLAY_RESOLUTION_Y_90 / 2 ;
            else sNewRGrid.AdresaGrid = 0;
            set_start_address_of_image(sNewRGrid.AdresaGrid);
            sNewRGrid.ScalarSpeed_100us = 10000/sNewRGrid.bViteza_gm;
            sRGrid.ScalarSpeed_100us = sNewRGrid.ScalarSpeed_100us;
            clock_100us = 0;
            sNewRGrid.StartMoving = START_MOVING;
            switch_on_display();DGScheduler = GRID_STEP4;}//return TRUE;}
            break;
        case GRID_STEP3:
        	if (sRGrid.bViteza_gm != sNewRGrid.bViteza_gm) {clock_100us = 0;DGScheduler = GRID_STEP4;}
        	else if (sRGrid.bSign != sNewRGrid.bSign) {sRGrid.bSign = sNewRGrid.bSign; return TRUE;}
        	else return TRUE;
        	break;
        case GRID_STEP4:
            sRGrid.bViteza_gm = sNewRGrid.bViteza_gm;
            sRGrid.bSign = sNewRGrid.bSign;
            sNewRGrid.ScalarSpeed_100us = 10000/sNewRGrid.bViteza_gm;
            sRGrid.ScalarSpeed_100us = sNewRGrid.ScalarSpeed_100us;
            sRGrid.wClearance = sNewRGrid.wClearance;
            return TRUE;
            break;
        default:
            DGScheduler = GRID_STEP1;
            break;
    }
    return FALSE;
}

void move_grid(void)
{
    if ((NewCmdCode == DISPLAY_GRID) && (sNewRGrid.StartMoving == START_MOVING ) && (clock_100us == sRGrid.ScalarSpeed_100us))
    {
    	clock_100us = 0; sRGrid.ScalarSpeed_100us = sNewRGrid.ScalarSpeed_100us;
        if ((sNewRGrid.bSign == 0) && (sNewRGrid.AdresaGrid != 0)) sNewRGrid.AdresaGrid -= 400;
        else if ((sNewRGrid.bSign == 0) && (sNewRGrid.AdresaGrid == 0)) sNewRGrid.AdresaGrid = sNewRGrid.wClearance * DISPLAY_RESOLUTION_Y_90 / 2;
        else if ((sNewRGrid.bSign != 0) && (sNewRGrid.AdresaGrid != sNewRGrid.wClearance * DISPLAY_RESOLUTION_Y_90 / 2)) sNewRGrid.AdresaGrid += 400;
        else if ((sNewRGrid.bSign != 0) && (sNewRGrid.AdresaGrid == sNewRGrid.wClearance * DISPLAY_RESOLUTION_Y_90 / 2)) sNewRGrid.AdresaGrid = 0;
        set_start_address_of_image(sNewRGrid.AdresaGrid);
    }
}

U8 display_test(void)
{
    switch(DTScheduler)
    {
        case TEST_STEP1:
            if (NewCmdCode != CmdCode)
            {
                 if (fill_memory_view_with_color(START_ADDRESS_PANEL1,bNewTest))
                 {
                     CmdCode = NewCmdCode;
                     bTest = bNewTest;
                     set_start_memory_address_cursor_and_switch_off_the_cursor(LOCATION_NO_OF_CURSOR_1);
                     switch_on_display();
                     return TRUE;
                 }
            }else DTScheduler = TEST_STEP2;
            break;
        case TEST_STEP2:
            if (bTest != bNewTest)
            {
                if (fill_memory_view_with_color(START_ADDRESS_PANEL1,bNewTest))
                {
                    bTest = bNewTest;
                    return TRUE;
                }
            }else return TRUE;
            break;
    }
    return FALSE;
}


