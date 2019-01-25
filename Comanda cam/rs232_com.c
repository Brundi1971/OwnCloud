
#include "config.h"
#include "rs232_com.h"
#include "uart_lib.h"
#include "rs485_com.h"
#include "read_resolvers.h"
#include "string.h"

/* variabilele de TimeOut pentru functia de citire a serialei RS232 */
volatile U16 TimeOutForRS232_Contor;
volatile U16 TimeOutForRS232_Reference;
/* variabile pentru functia de citire a serialei RS232 */
volatile U8 ByteIndex_ReceivedFromRS232;
volatile U8 NbOfBytes_HaveToBeReceivedFromRS232;
volatile U8 ReceivingBufferRS232[NB_MAX_OF_BYTES_RECEIVED_FROM_RS232+1];    /* bufferul de intrare */

/* variabila in care se memoreaza comanda bruta de la master */
volatile U8 MasterCommandForCamera = 0;
volatile U8 Memory_MasterCommandForCamera = 0;

volatile U8 CameraMotorsSpeed = 1;                  /* 50% */
volatile U8 VisibleCameraFunctionsSelect = 0;       /* zoom pt Visible */
volatile U8 ThermalCameraFunctionsSelect = 0;       /* zoom pt Termal*/
volatile U8 TypeCameraSelected = 0;                 /* Selectata Visible */
volatile U8 ThermalCameraOnOff = 1;                 /* Termal On */

volatile U8 VisibleDataZoom[4];
volatile U8 VisibleDataFocus[4];
volatile U8 ThermalDataZoom[4];
volatile U8 ThermalDataFocus[4];
volatile U8 CS1, CS2, CS3;
volatile U8 DataTemperatura;
volatile U8 DataPresiune;

/* variabile pentru functia de transmitere a datelor pe seriala */
volatile U8 NbOfBytes_SentToRS232;
volatile U16 TransmissionBufferPointer;             /* pointer catre string-ul de iesire */
volatile U8 TransmissionPointer;
volatile U8 TransmissionBufferRS232[30];
volatile U16 ReplyBufferPointer;
volatile U8 Status_TransmitCommand;

volatile U8 Command_stack[20];
volatile U8 Command_no;

volatile U8 Semafor = ROSU;
volatile U8 Nr_incercari;
volatile U8 CameraErrorType = 0;
volatile U16 Contor_booting_cameras = 0;
volatile U8 Flag_booting_cameras = 0;
volatile U16 Contor_RepeatAGC;


/* General Temperature sensor */
U8 ReadTemperatureSensor[] PROGMEM = {0x66, 0x05, 0x54, 0x00, 0x37};
U8 ReadTemperatureSensorReplyHeader[] PROGMEM = {0x66, 0x05, 0x54};

/* General Presure Sensor */
U8 ReadPresureSensor[] PROGMEM = {0x66, 0x05, 0x50, 0x00, 0x33};
U8 ReadPresureSensorReplyHeader[] PROGMEM = {0x66, 0x05, 0x50};

/* Thermal Temperature sensor */
U8 ThermalDetectorTemperature[] PROGMEM = {0x66, 0x08, 0x04, 0xc0, 0x54, 0x54, 0xc0, 0x6a};
U8 ThermalDetectorTemperaturerReplyHeader[] PROGMEM = {0x66, 0x08, 0x04, 0xc0, 0x54};

/* Pornirea si oprirea ambelor camere video */
U8 BothCamerasPowerUp[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 1, 1, 0x00, 0x22};
U8 BothCamerasPowerUpReply[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 1, 1, 0x00, 0x22};

U8 BothCamerasPowerDown[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 0, 0, 0x00, 0x22};
U8 BothCamerasPowerDownReply[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 0, 0, 0x00, 0x22};

/*=========================================================================================================*/
/* Visible Camera Messages */
/*=========================================================================================================*/
U8 SelectVisible_2[] PROGMEM = {0x66, 0x05, 0x56, 0x03, 0x36};
U8 SelectVisible[] PROGMEM = {0x66, 0x05, 0x55, 0x03, 0x35};
U8 SelectVisibleReply_2[] PROGMEM = {0x66, 0x05, 0x56, 0x03, 0x36};
U8 SelectVisibleReply[] PROGMEM = {0x66, 0x05, 0x55, 0x03, 0x35};
U8 VisiblePowerUp[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 1, 0, 0x00, 0x23};
U8 VisiblePowerUpReply[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 1, 0, 0x00, 0x23};
U8 VisiblePowerDown[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 0, 0, 0x00, 0x22};
U8 VisiblePowerDownReply[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 0, 0, 0x00, 0x22};
U8 VisibleOn[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x6b, 0x00, 0xd1, 0xc0, 0xb3};
U8 VisibleOnReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x6b, 0x19, 0xc0, 0x33};
U8 VisibleOff[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x6c, 0x00, 0xd2, 0xc0, 0xb7};
U8 VisibleOffReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x6c, 0x1a, 0xc0, 0x37};

U8 VisibleSetMCSpeed25[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x64, 0x19, 0xe3, 0xc0, 0x97};
U8 VisibleSetMCSpeed25Reply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x64, 0x12, 0xc0, 0x37};
U8 VisibleSetMCSpeed50[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x64, 0x32, 0xfc, 0xc0, 0xa3};
U8 VisibleSetMCSpeed50Reply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x64, 0x12, 0xc0, 0x37};
U8 VisibleSetMCSpeed75[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x64, 0x4b, 0x15, 0xc0, 0x33};
U8 VisibleSetMCSpeed75Reply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x64, 0x12, 0xc0, 0x37};
U8 VisibleSetMCSpeed100[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x64, 0x64, 0x2e, 0xc0, 0x27};
U8 VisibleSetMCSpeed100Reply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x64, 0x12, 0xc0, 0x37};

U8 VisibleStopMotors[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x73, 0x00, 0xd9, 0xc0, 0xa3};
U8 VisibleStopMotorsReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x73, 0x21, 0xc0, 0x13};
U8 VisibleZoomNarrow[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x7a, 0x6f, 0x4f, 0xc0, 0x53};
U8 VisibleZoomNarrowReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x7a, 0x28, 0xc0, 0x13};
U8 VisibleZoomWide[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x7a, 0x69, 0x49, 0xc0, 0x53};
U8 VisibleZoomWideReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x7a, 0x28, 0xc0, 0x13};
U8 VisibleFocusNear[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x78, 0x69, 0x47, 0xc0, 0x5f};
U8 VisibleFocusNearReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x78, 0x26, 0xc0, 0x1f};
U8 VisibleFocusFar[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x78, 0x6f, 0x4d, 0xc0, 0x53};
U8 VisibleFocusFarReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x78, 0x26, 0xc0, 0x1f};
U8 VisibleAGCSetPointUp[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x75, 0x01, 0xdc, 0xc0, 0xa1};
U8 VisibleAGCSetPointUpReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x75, 0x23, 0xc0, 0x17};
U8 VisibleAGCSetPointDown[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x65, 0x01, 0xcc, 0xc0, 0xa1};
U8 VisibleAGCSetPointDownReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x65, 0x13, 0xc0, 0x37};
U8 VisibleAGCSetPointSave[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x77, 0x00, 0xdd, 0xc0, 0xa3};
U8 VisibleAGCSetPointSaveReply[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x48, 0x77, 0x25, 0xc0, 0x13};
U8 VisibleHitNearFocusLimit[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x41, 0x00, 0xa7, 0xc0, 0xef};
U8 VisibleHitFarFocusLimit[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x42, 0x00, 0xa8, 0xc0, 0xe3};
U8 VisibleHitNearZoomLimit[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x44, 0x00, 0xaa, 0xc0, 0xe7};
U8 VisibleHitFarZoomLimit[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x45, 0x00, 0xab, 0xc0, 0xe7};

U8 VisibleQueryZoomPosition[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x7a, 0x67, 0x47, 0xc0, 0x53};
U8 VisibleQueryZoomPositionReplyHeader[] PROGMEM = {0x66, 0x14, 0x03, 0xc0, 0x66, 0x48, 0x7a, 0x28, 0xc0, 0xc0, 0x66, 0x7a, 0x67};
U8 VisibleQueryFocusPosition[] PROGMEM = {0x66, 0x0a, 0x03, 0xc0, 0x66, 0x78, 0x67, 0x45, 0xc0, 0x53};
U8 VisibleQueryFocusPositionReplyHeader[] PROGMEM = {0x66, 0x14, 0x03, 0xc0, 0x66, 0x48, 0x78, 0x26, 0xc0, 0xc0, 0x66, 0x78, 0x67};
/*=========================================================================================================*/
/* Thermal Camera Messages */
/*=========================================================================================================*/
U8 SelectThermal_2[] PROGMEM = {0x66, 0x05, 0x56, 0x00, 0x35};
U8 SelectThermal[] PROGMEM = {0x66, 0x05, 0x55, 0x00, 0x36};
U8 SelectThermalReply_2[] PROGMEM = {0x66, 0x05, 0x56, 0x00, 0x35};
U8 SelectThermalReply[] PROGMEM = {0x66, 0x05, 0x55, 0x00, 0x36};
U8 ThermalPowerUp[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 0, 1, 0x00, 0x23};
U8 ThermalPowerUpReply[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 0, 1, 0x00, 0x23};
U8 ThermalaPowerDown[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 0, 0, 0x00, 0x22};
U8 ThermalPowerDownReply[] PROGMEM = {0x66, 0x09, 0x4d, 0, 0, 0, 0, 0x00, 0x22};

U8 ThermalSetMCSpeed25[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x64, 0x19, 0xe3, 0xdb, 0xdc, 0xad, 0xc0, 0x72};
U8 ThermalSetMCSpeed25Reply[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x64, 0x12, 0xdb, 0xdc, 0x0d, 0xc0, 0x72};
U8 ThermalSetMCSpeed50[] PROGMEM = {0x66, 0x10, 0x04, 0x55, 0xc0, 0xdb, 0xdc, 0x66, 0x64, 0x32, 0xfc, 0xdb, 0xdc, 0x99, 0xc0, 0x72};
U8 ThermalSetMCSpeed50Reply[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x64, 0x12, 0xdb, 0xdc, 0x0d, 0xc0, 0x72};
U8 ThermalSetMCSpeed75[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x64, 0x4b, 0x15, 0xdb, 0xdc, 0x09, 0xc0, 0x72};
U8 ThermalSetMCSpeed75Reply[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x64, 0x12, 0xdb, 0xdc, 0x0d, 0xc0, 0x72};
U8 ThermalSetMCSpeed100[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x64, 0x64, 0x2e, 0xdb, 0xdc, 0x1d, 0xc0, 0x72};
U8 ThermalSetMCSpeed100Reply[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x64, 0x12, 0xdb, 0xdc, 0x0d, 0xc0, 0x72};

U8 ThermalStopMotors[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x73, 0x00, 0xd9, 0xdb, 0xdc, 0x99, 0xc0, 0x72};
U8 ThermalStopMotorsReply[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x73, 0x21, 0xdb, 0xdc, 0x29, 0xc0, 0x72};
U8 ThermalZoomIn[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x7a, 0x69, 0x49, 0xdb, 0xdc, 0x69, 0xc0, 0x72};
U8 ThermalZoomInReply[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x7a, 0x28, 0xdb, 0xdc, 0x29, 0xc0, 0x72};
U8 ThermalZoomOut[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x7a, 0x6f, 0x4f, 0xdb, 0xdc, 0x69, 0xc0, 0x72};
U8 ThermalZoomOutReply[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x7a, 0x28, 0xdb, 0xdc, 0x29, 0xc0, 0x72};
U8 ThermalFocusIn[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x78, 0x69, 0x47, 0xdb, 0xdc, 0x65, 0xc0, 0x72};
U8 ThermalFocusInReply[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x78, 0x26, 0xdb, 0xdc, 0x25, 0xc0, 0x72};
U8 ThermalFocusOut[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x78, 0x6f, 0x4d, 0xdb, 0xdc, 0x69, 0xc0, 0x72};
U8 ThermalFocusOutReply[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x78, 0x26, 0xdb, 0xdc, 0x25, 0xc0, 0x72};

U8 ThermalQueryZoomPosition[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x7a, 0x67, 0x47, 0xdb, 0xdc, 0x69, 0xc0, 0x72};
U8 ThermalQueryZoomPositionReplyHeader[] PROGMEM = {0x66, 0x1c, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x7a, 0x28, 0xdb, 0xdc, 0xdb, 0xdc, 0x66, 0x7a, 0x67};
U8 ThermalQueryFocusPosition[] PROGMEM = {0x66, 0x10, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x78, 0x67, 0x45, 0xdb, 0xdc, 0x69, 0xc0, 0x72};
U8 ThermalQueryFocusPositionReplyHeader[] PROGMEM = {0x66, 0x1c, 0x04, 0xc0, 0x55, 0xdb, 0xdc, 0x66, 0x48, 0x78, 0x26, 0xdb, 0xdc, 0xdb, 0xdc, 0x66, 0x78, 0x67};

U8 ThermalSetPALOutputFormat[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x36, 0x00, 0x00, 0x00, 0x75, 0xc0, 0x6e};
U8 ThermalSetNTSCOutputFormat[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x36, 0x00, 0x01, 0x00, 0x74, 0xc0, 0x6e};

U8 ThermalSetColdWhite[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x35, 0x00, 0x00, 0x00, 0x76, 0xc0, 0x6e};
U8 ThermalSetHotWhite[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x35, 0x00, 0x00, 0x01, 0x77, 0xc0, 0x6e};

U8 ThermalAGCSetPointHeader[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x3e, 0x00};
U8 ThermalAGCRunConstantHeader[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x35, 0x00};
U8 ThermalAGCAdjustRegionDefault[] PROGMEM = {0x66, 0x0d, 0x04, 0xc0, 0x43, 0xa0, 0x00, 0x00, 0x00, 0x00, 0xe3, 0xc0, 0x6e};
U8 ThermalAGCAdjustRegionLowerHeader[] PROGMEM = {0x66, 0x0d, 0x04, 0xc0, 0x43, 0xa0, 0x01, 0x01};
U8 ThermalAGCAdjustRegionUpperHeader[] PROGMEM = {0x66, 0x0d, 0x04, 0xc0, 0x43, 0xa0, 0x01, 0x02};

U8 ThermalAdjustTopLeftHeader[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0xa5, 0x00};

U8 ThermalPerformSinglePointNUC[] PROGMEM = {0x66, 0x0a, 0x04, 0xc0, 0x43, 0x90, 0x01, 0x00, 0x00, 0xd2, 0xc0, 0x6e};
U8 ThermalPerformSinglePointNUCReply[] PROGMEM = {0x66, 0x0a, 0x04, 0x43, 0x91, 0x01, 0x43, 0x00, 0x90, 0x68};
U8 ThermalSingleColdNUC[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x91, 0x01, 0x43, 0x00, 0x90, 0xc0, 0x6e};
U8 ThermalSingleCalculateNUC[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x92, 0x01, 0x00, 0x00, 0xd0, 0xc0, 0x6e};

U8 ThermalPerformTwoPointNUC[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x90, 0x02, 0x00, 0x00, 0xd1, 0xc0, 0x6e};
U8 ThermalPerformTwoPointNUCReply[] PROGMEM = {0x66, 0x0a, 0x04, 0x43, 0x91, 0x01, 0x43, 0x00, 0x90, 0x68};
U8 ThermalPerformTwoColdNUC[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x91, 0x02, 0x43, 0x00, 0x93, 0xc0, 0x6e};
U8 ThermalPerformTwoHotNUC[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x91, 0x02, 0x48, 0x00, 0x98, 0xc0, 0x6e};
U8 ThermalPerformTwoCalculateNUC[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x92, 0x02, 0x00, 0x00, 0xd3, 0xc0, 0x6e};

U8 ThermalClearCurrentNUC[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x93, 0x00, 0x00, 0x00, 0xd0, 0xc0, 0x6e};

U8 ThermalLoadStoredNUC0[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x95, 0x00, 0x00, 0x00, 0xd6, 0xc0, 0x6e};
U8 ThermalLoadStoredNUC1[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x95, 0x00, 0x00, 0x01, 0xd7, 0xc0, 0x6e};

U8 ThermalSelectNUC0[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x97, 0x00, 0x00, 0x00, 0xd4, 0xc0, 0x6e};
U8 ThermalselectNUC1[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x97, 0x01, 0x00, 0x01, 0xd5, 0xc0, 0x6e};

U8 ThermalEraseNUC0[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0xa9, 0x00, 0x00, 0x00, 0xea, 0xc0, 0x6e};
U8 ThermalEraseNUC1[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0xa9, 0x01, 0x00, 0x01, 0xeb, 0xc0, 0x6e};

U8 ThermalStoreNUC0[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x94, 0x00, 0x00, 0x00, 0xd7, 0xc0, 0x6e};
U8 ThermalStoreNUC1[] PROGMEM = {0x66, 0x0c, 0x04, 0xc0, 0x43, 0x94, 0x01, 0x00, 0x01, 0xd6, 0xc0, 0x6e};

/* Visible Error messages */
U8 OverFlowError[] PROGMEM = {0x66, 0x04, 0x3f, 0x00, 0x5d};
U8 OverFlowErrorCleared[] PROGMEM = {0x66, 0x04, 0x21, 0x00, 0x43};
U8 SYN[] PROGMEM = {0x16};

/* Thermal Error messages */
U8 ByteStuffingErrorHeader[] PROGMEM = {0x66, 0x09, 0x04, 0x5a, 0x53, 0x42};
U8 LengthTooShortHeader[] PROGMEM = {0x66, 0x09, 0x04, 0x5a, 0x53, 0x53};
U8 LengthTooLongHeader[] PROGMEM = {0x66, 0x09, 0x04, 0x5a, 0x53, 0x4c};


void init_UART1(void)
{ 
    Uart1_set_baudrate(BAUDRATE_UART1);
    Uart1_enable();
    Uart1_enable_8bit();
	//set_Status_SystemCamere(0,(TypeCamera | (ThermalCameraOnOff << 1)));
	//set_Status_SystemCamere(1,(CameraMotorsSpeed << 3));
}

void set_MasterCommandForCamera(U8 Val)                  {MasterCommandForCamera = Val;}
U8 get_MasterCommandForCamera(void)                      {return MasterCommandForCamera;}

void set_VisibleDataZoom(U8 Index, U8 Val)               {VisibleDataZoom[Index] = Val;}
U8 get_VisibleDataZoom(U8 Index)                         {return VisibleDataZoom[Index];}
void set_VisibleDataFocus(U8 Index, U8 Val)              {VisibleDataFocus[Index] = Val;}
U8 get_VisibleDataFocus(U8 Index)                        {return VisibleDataFocus[Index];}

void set_ThermalDataZoom(U8 Index, U8 Val)               {ThermalDataZoom[Index] = Val;}
U8 get_ThermalDataZoom(U8 Index)                         {return ThermalDataZoom[Index];}
void set_ThermalDataFocus(U8 Index, U8 Val)              {ThermalDataFocus[Index] = Val;}
U8 get_ThermalDataFocus(U8 Index)                        {return ThermalDataFocus[Index];}

void set_DataTemperatura(U8 Val)                         {DataTemperatura = Val;}
U8 get_DataTemperatura(void)                             {return DataTemperatura;}
void set_DataPresiune(U8 Val)                            {DataPresiune = Val;}
U8 get_DataPresiune(void)                                {return DataPresiune;}
void set_CameraErrorType(U8 Val)                         {CameraErrorType = Val;}
U8 get_CameraErrorType(void)                             {return CameraErrorType;}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
====================================================================================================================================
  functiile pentru comunicatia cu sistemul de camere video 
====================================================================================================================================
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void ResetTimeOut_Contor(void) {TimeOutForRS232_Contor = 0;}
void ResetRS232Reception(void)
{
  register U8 c;

  c = Uart1_get_byte();
  ByteIndex_ReceivedFromRS232 = 0;
}

/* functie receptie date pe seriala */
void ReceiveDataFromRS232(void)                
{
  register U8 c;

  if (Uart1_rx_ready())
  {
    c = Uart1_get_byte();
    if (ByteIndex_ReceivedFromRS232 != NB_MAX_OF_BYTES_RECEIVED_FROM_RS232)
    {  
      ReceivingBufferRS232[ByteIndex_ReceivedFromRS232++] = c; 
    }
  }
}


/* functie transmitere date pe seriala */
U8 TransmiteDataToRS232(void)
{
  register U8 c;

  if (Uart1_tx_complet())
  {
     Uart1_clear_flag_tx();
     if (++TransmissionPointer != NbOfBytes_SentToRS232)
     {
        c = TransmissionBufferRS232[TransmissionPointer];
        while(!Uart1_tx_ready());Uart1_send_byte(c);
     }
     else return TRANSMISSION_OK;
   }
   return TRANSMISSION_PENDING;
}


/* format comanda de la master */
/*
TyteCameraSelected =>        0 = zi, 1 = termica
ThermalCameraOnOff =>        0 = off, 1 = on
MotorsSpeed =>               00 = speed 25%
                             01 = speed 50%
                             10 = speed 75%
                             11 = speed 100%
SelectFunctions =>           000 = zoom
                             001 = focus
                             010 = AGC
                             011 = Polarity
*/


/* functia de boot-are se lanseaza la 3 secunde dupa pornirea alimentarii */ 
void boot_cameras(void)
{
  if ((Flag_booting_cameras == 0) && (Contor_booting_cameras > 60000))
  {
  	 //set_Status_SystemCamere(0,(TypeCamera | (ThermalCameraOnOff << 1)));
	 //set_Status_SystemCamere(1,(CameraMotorsSpeed << 3));
     set_CameraErrorType(NO_ERROR);
     Flag_booting_cameras = 1;
     //set_Status_SystemCamere(0,(CameraOnOff | (TypeCamera << 1)));
     Nr_incercari = NB_OF_INCERCARI;
     Memory_MasterCommandForCamera = (1<<BIT_COMANDA_BOOTING);
     create_command_stack();
  }
}

/* citeste comanda sosita de la master si creeaza stiva de comenzi */
void read_command_from_master(void)
{
  if (Semafor == VERDE)
  {
    if (get_MasterCommandForCamera() != Memory_MasterCommandForCamera)
    {
      Nr_incercari = NB_OF_INCERCARI;
      Memory_MasterCommandForCamera = get_MasterCommandForCamera();
      create_command_stack();
      //Toggle_LED();
    }
  }
}


void create_command_stack(void)
{
  register U8 c;

  switch (Memory_MasterCommandForCamera)
  {
    case 0:
                                                              break;
    case (1<<BIT_COMANDA_BOOTING):
                                                              create_booting_command();
                                                              break;
    case (1<<BIT_CAMERA_COMMAND_INCREASE):
                                                              camera_command_increase_function();
                                                              interpret_commands_from_stack();
                                                              break;
    case (1<<BIT_CAMERA_COMMAND_DECREASE):
                                                              camera_command_decrease_function();
                                                              interpret_commands_from_stack();
                                                              break;

    case (1<<BIT_CAMERA_COMMAND_CAMERA_ON_OFF):
                                                              camera_command_switch_on_off_function();
                                                              interpret_commands_from_stack();
                                                              break;
    /* butonul albastru */
    case (1<<BIT_CAMERA_COMMAND_SEL_CAMERA_TYPE):
                                                              if (ThermalCameraOnOff) 
                                                              {
                                                                 camera_command_select_visible_or_thermal();
                                                                 interpret_commands_from_stack();
                                                              }
                                                              break;
    /* butonul operatorului */
    case (1<<BIT_CAMERA_COMMAND_SPEED):
                                                              if (CameraMotorsSpeed >= 0x03) CameraMotorsSpeed = 0;
                                                              else CameraMotorsSpeed += 1;
                                                              c = get_Status_SystemCamere(1);
                                                              c &= ~MSK_STATUS_SELECT_SPEED;
                                                              c |= (CameraMotorsSpeed << 3);
                                                              set_Status_SystemCamere(1,c);
                                                              break;
    /* butonul rosu */
    case (1<<BIT_CAMERA_COMMAND_SEL_ZOOM_FOCUS_AGC_POLARITY):
                                                              change_camera_function();
                                                              break;
  }
}


/* functia de booting */
void create_booting_command(void)
{
     CameraMotorsSpeed = 1;
     VisibleCameraFunctionsSelect = 0;
     ThermalCameraFunctionsSelect = 0;
     TypeCameraSelected = 0;
     ThermalCameraOnOff = 0;

     Command_stack[8] = BOTH_CAMERAS_POWER_UP;
     Command_stack[7] = BOTH_CAMERAS_POWER_UP_REPLY;
     //Command_stack[6] = EXECUTE_5ms_DELAY;
     Command_stack[6] = VISIBLE_CAMERA_ON;
     Command_stack[5] = VISIBLE_CAMERA_ON_REPLY;
     Command_stack[4] = SELECT_VISIBLE;
     Command_stack[3] = SELECT_VISIBLE_REPLY;
     Command_stack[2] = VISIBLE_QUERY_ZOOM;
     Command_stack[1] = VISIBLE_QUERY_ZOOM_REPLY_HEADER;
     Command_stack[0] = FINALIZE_THE_COMMAND;
     Command_no = 7;
     interpret_commands_from_stack();
}

/* functia prin care se da comanda de alimentare a camerelor video */
void camera_command_switch_on_off_function(void)
{
  switch(ThermalCameraOnOff)
  {
    case THERMAL_CAMERA_IS_SWITCHED_OFF:
                                           Command_stack[2] = BOTH_CAMERAS_POWER_UP;
                                           Command_stack[1] = BOTH_CAMERAS_POWER_UP_REPLY;
                                           Command_stack[0] = FINALIZE_THE_COMMAND;
                                           Command_no = 2;
                                           break;
    case THERMAL_CAMERA_IS_SWITCHED_ON:
                                           Command_stack[6] = VISIBLE_POWER_UP;
                                           Command_stack[5] = VISIBLE_POWER_UP_REPLY;
                                           Command_stack[4] = VISIBLE_CAMERA_ON;
                                           Command_stack[3] = VISIBLE_CAMERA_ON_REPLY;
                                           Command_stack[2] = SELECT_VISIBLE;
                                           Command_stack[1] = SELECT_VISIBLE_REPLY;
                                           Command_stack[0] = FINALIZE_THE_COMMAND;
                                           Command_no = 6;
                                           break;
  }
}

/* functia prin care este selectata iesirea unei camere sau a celeilalte */
void camera_command_select_visible_or_thermal(void)
{
    switch(TypeCameraSelected)
    {
      case VISIBLE_SELECTED:
                                           Command_stack[1] = SELECT_THERMAL;
                                           Command_stack[0] = FINALIZE_THE_COMMAND;
                                           Command_no = 1; 
                                           break;
      case THERMAL_SELECTED:
                                           Command_stack[4] = VISIBLE_CAMERA_ON;
                                           Command_stack[3] = VISIBLE_CAMERA_ON_REPLY;
                                           Command_stack[2] = SELECT_VISIBLE;
                                           Command_stack[1] = SELECT_VISIBLE_REPLY;
                                           Command_stack[0] = FINALIZE_THE_COMMAND;
                                           Command_no = 4; 
                                           break;
    }
}


/* selecteaza functia specifica pentru fiecare tip de camera */
void change_camera_function(void)
{
//  register U8 c;

  switch(TypeCameraSelected)
  {
    case VISIBLE_SELECTED:
                                 if (VisibleCameraFunctionsSelect >= 0x02) VisibleCameraFunctionsSelect = 0;
                                 else VisibleCameraFunctionsSelect += 1;
                                 switch(VisibleCameraFunctionsSelect)
                                 {
                                   case SELECT_ZOOM:
                                                        Command_stack[2] = VISIBLE_QUERY_ZOOM;
                                                        Command_stack[1] = VISIBLE_QUERY_ZOOM_REPLY_HEADER;
                                                        Command_stack[0] = FINALIZE_THE_COMMAND;
                                                        Command_no = 2;  
                                                        break;
                                   case SELECT_FOCUS:

                                                        Command_stack[2] = VISIBLE_QUERY_FOCUS;
                                                        Command_stack[1] = VISIBLE_QUERY_FOCUS_REPLY_HEADER;
                                                        Command_stack[0] = FINALIZE_THE_COMMAND;
                                                        Command_no = 2;  
                                                        break;
                                 }
                                 interpret_commands_from_stack();
                                 break;
    case THERMAL_SELECTED:
                                 if (ThermalCameraFunctionsSelect >= 0x03) ThermalCameraFunctionsSelect = 0;
                                 else ThermalCameraFunctionsSelect += 1;
                                 switch(ThermalCameraFunctionsSelect)
                                 {
                                   case SELECT_ZOOM:
                                                        Command_stack[2] = THERMAL_QUERY_ZOOM;
                                                        Command_stack[1] = THERMAL_QUERY_ZOOM_REPLY_HEADER;
                                                        Command_stack[0] = FINALIZE_THE_COMMAND;
                                                        Command_no = 2;  
                                                        break;
                                   case SELECT_FOCUS:
                                                        Command_stack[2] = THERMAL_QUERY_FOCUS;
                                                        Command_stack[1] = THERMAL_QUERY_FOCUS_REPLY_HEADER;
                                                        Command_stack[0] = FINALIZE_THE_COMMAND;
                                                        Command_no = 2;  
                                                        break;
                                 }
                                 interpret_commands_from_stack();
                                 break;
  }
}


/* functia increase */
void camera_command_increase_function(void)
{
  switch (TypeCameraSelected)
  {
    case VISIBLE_SELECTED:
                              switch(CameraMotorsSpeed)
                              {
                                  case SPEED_25:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_25;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_25_REPLY;
                                                        break;
                                  case SPEED_50:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_50;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_50_REPLY;
                                                        break;
                                  case SPEED_75:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_75;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_75_REPLY;
                                                        break;
                                  case SPEED_100:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_100;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_100_REPLY;
                                                        break;
                              }
                              switch(VisibleCameraFunctionsSelect)
                              {
                                  case SELECT_ZOOM:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_50;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_50_REPLY;
                                                        Command_stack[2] = VISIBLE_ZOOM_WIDE;
                                                        Command_stack[1] = VISIBLE_ZOOM_WIDE_REPLY;
                                                        Command_stack[0] = READ_ZOOM_AND_FOCUS;
                                                        Command_no = 4;
                                                        break;
                                  case SELECT_FOCUS:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_75;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_75_REPLY;
                                                        Command_stack[2] = VISIBLE_FOCUS_FAR;
                                                        Command_stack[1] = VISIBLE_FOCUS_FAR_REPLY;
                                                        Command_stack[0] = READ_ZOOM_AND_FOCUS;
                                                        Command_no = 4;
                                                        break;
                                  case SELECT_AGC:
                                                        Command_stack[3] = VISIBLE_AGC_SETPOINT_DOWN;
                                                        Command_stack[2] = VISIBLE_AGC_SETPOINT_DOWN_REPLY;
                                                        Command_stack[1] = EXECUTE_500ms_DELAY;
                                                        Command_stack[0] = REPEAT_COMMAND;
                                                        Command_no = 3;
                                                        break;
                              }
                              break;
    case THERMAL_SELECTED:
                              switch(CameraMotorsSpeed)
                              {
                                  case SPEED_25:
                                                        Command_stack[4] = THERMAL_SET_SPEED_25;
                                                        Command_stack[3] = THERMAL_SET_SPEED_25_REPLY;
                                                        break;
                                  case SPEED_50:
                                                        Command_stack[4] = THERMAL_SET_SPEED_50;
                                                        Command_stack[3] = THERMAL_SET_SPEED_50_REPLY;
                                                        break;
                                  case SPEED_75:
                                                        Command_stack[4] = THERMAL_SET_SPEED_75;
                                                        Command_stack[3] = THERMAL_SET_SPEED_75_REPLY;
                                                        break;
                                  case SPEED_100:
                                                        Command_stack[4] = THERMAL_SET_SPEED_100;
                                                        Command_stack[3] = THERMAL_SET_SPEED_100_REPLY;
                                                        break;
                              }
                              switch(ThermalCameraFunctionsSelect)
                              {
                                  case SELECT_ZOOM:
                                                        Command_stack[4] = THERMAL_SET_SPEED_50;
                                                        Command_stack[3] = THERMAL_SET_SPEED_50_REPLY;
                                                        Command_stack[2] = THERMAL_ZOOM_OUT;
                                                        Command_stack[1] = THERMAL_ZOOM_OUT_REPLY;
                                                        Command_stack[0] = READ_ZOOM_AND_FOCUS;
                                                        Command_no = 4;
                                                        break;
                                  case SELECT_FOCUS:
                                                        Command_stack[4] = THERMAL_SET_SPEED_25;
                                                        Command_stack[3] = THERMAL_SET_SPEED_25_REPLY;
                                                        Command_stack[2] = THERMAL_FOCUS_OUT;
                                                        Command_stack[1] = THERMAL_FOCUS_OUT_REPLY;
                                                        Command_stack[0] = READ_ZOOM_AND_FOCUS;
                                                        Command_no = 4;
                                                        break;
                                  case SELECT_AGC:
                                                        //Command_stack[2] = THERMAL_AGC_SETPOINT_UP;
                                                        //Command_stack[1] = THERMAL_AGC_SETPOINT_UP_REPLY;
                                                        //Command_stack[0] = FINALIZE_THE_COMMAND;
                                                        //Command_no = 4;
                                                        break;
                                  case SELECT_POLARITY:
                                                        Command_stack[1] = THERMAL_SET_HOT_WHITE;
                                                        Command_stack[0] = FINALIZE_THE_COMMAND;
                                                        Command_no = 1;
                                                        break;
                               }
                               break;
   }

}

/* functia decrease */
void camera_command_decrease_function(void)
{
  switch (TypeCameraSelected)
  {
    case VISIBLE_SELECTED:
                              switch(CameraMotorsSpeed)
                              {
                                  case SPEED_25:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_25;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_25_REPLY;
                                                        break;
                                  case SPEED_50:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_50;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_50_REPLY;
                                                        break;
                                  case SPEED_75:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_75;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_75_REPLY;
                                                        break;
                                  case SPEED_100:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_100;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_100_REPLY;
                                                        break;
                              }
                              switch(VisibleCameraFunctionsSelect)
                              {
                                  case SELECT_ZOOM:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_50;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_50_REPLY;
                                                        Command_stack[2] = VISIBLE_ZOOM_NARROW;
                                                        Command_stack[1] = VISIBLE_ZOOM_NARROW_REPLY;
                                                        Command_stack[0] = READ_ZOOM_AND_FOCUS;
                                                        Command_no = 4;
                                                        break;
                                  case SELECT_FOCUS:
                                                        Command_stack[4] = VISIBLE_SET_SPEED_75;
                                                        Command_stack[3] = VISIBLE_SET_SPEED_75_REPLY;
                                                        Command_stack[2] = VISIBLE_FOCUS_NEAR;
                                                        Command_stack[1] = VISIBLE_FOCUS_NEAR_REPLY;
                                                        Command_stack[0] = READ_ZOOM_AND_FOCUS;
                                                        Command_no = 4;
                                                        break;
                                  case SELECT_AGC:
                                                        Command_stack[3] = VISIBLE_AGC_SETPOINT_UP;
                                                        Command_stack[2] = VISIBLE_AGC_SETPOINT_UP_REPLY;
                                                        Command_stack[1] = EXECUTE_500ms_DELAY;
                                                        Command_stack[0] = REPEAT_COMMAND;
                                                        Command_no = 3;
                                                        break;
                               }
                               break;
    case THERMAL_SELECTED:
                              switch(CameraMotorsSpeed)
                              {
                                  case SPEED_25:
                                                        Command_stack[4] = THERMAL_SET_SPEED_25;
                                                        Command_stack[3] = THERMAL_SET_SPEED_25_REPLY;
                                                        break;
                                  case SPEED_50:
                                                        Command_stack[4] = THERMAL_SET_SPEED_50;
                                                        Command_stack[3] = THERMAL_SET_SPEED_50_REPLY;
                                                        break;
                                  case SPEED_75:
                                                        Command_stack[4] = THERMAL_SET_SPEED_75;
                                                        Command_stack[3] = THERMAL_SET_SPEED_75_REPLY;
                                                        break;
                                  case SPEED_100:
                                                        Command_stack[4] = THERMAL_SET_SPEED_100;
                                                        Command_stack[3] = THERMAL_SET_SPEED_100_REPLY;
                                                        break;
                              }
                              switch(ThermalCameraFunctionsSelect)
                              {
                                  case SELECT_ZOOM:
                                                        Command_stack[4] = THERMAL_SET_SPEED_50;
                                                        Command_stack[3] = THERMAL_SET_SPEED_50_REPLY;
                                                        Command_stack[2] = THERMAL_ZOOM_IN;
                                                        Command_stack[1] = THERMAL_ZOOM_IN_REPLY;
                                                        Command_stack[0] = READ_ZOOM_AND_FOCUS;
                                                        Command_no = 4;
                                                        break;
                                  case SELECT_FOCUS:
                                                        Command_stack[4] = THERMAL_SET_SPEED_25;
                                                        Command_stack[3] = THERMAL_SET_SPEED_25_REPLY;
                                                        Command_stack[2] = THERMAL_FOCUS_IN;
                                                        Command_stack[1] = THERMAL_FOCUS_IN_REPLY;
                                                        Command_stack[0] = READ_ZOOM_AND_FOCUS;
                                                        Command_no = 4;
                                                        break;
                                  case SELECT_AGC:
                                                        //Command_stack[2] = THERMAL_AGC_SETPOINT_DOWN;
                                                        //Command_stack[1] = THERMAL_AGC_SETPOINT_DOWN_REPLY;
                                                        //Command_stack[0] = FINALIZE_THE_COMMAND;
                                                        //Command_no = 4;
                                                        break;
                                  case SELECT_POLARITY:
                                                        Command_stack[1] = THERMAL_SET_COLD_WHITE;
                                                        Command_stack[0] = FINALIZE_THE_COMMAND;
                                                        Command_no = 1;
                                                        break;
                               }
                               break;
   }

}


/* functia prin care se comanda oprirea motoarelor */
void camera_command_stop(void)
{
  switch(TypeCameraSelected)
  {
      case VISIBLE_SELECTED:
                                   Command_stack[2] = VISIBLE_STOP_MOTORS;
                                   Command_stack[1] = VISIBLE_STOP_MOTORS_REPLY;
                                   break;
      case THERMAL_SELECTED:
                                   Command_stack[2] = THERMAL_STOP_MOTORS;
                                   Command_stack[1] = THERMAL_STOP_MOTORS_REPLY;
                                   break;
  }
  Command_stack[0] = FINALIZE_THE_COMMAND;
  Command_no = 2;
}

/* functia prin care se citeste pozitia zoom-ului sau a focus-ului */
void camera_read_position_of_zoom_or_focus(void)
{
 switch (TypeCameraSelected)
  {
    case VISIBLE_SELECTED:
                              switch(VisibleCameraFunctionsSelect)
                              {
                                  case SELECT_ZOOM:
                                                        Command_stack[2] = VISIBLE_QUERY_ZOOM;
                                                        Command_stack[1] = VISIBLE_QUERY_ZOOM_REPLY_HEADER;
                                                        break;
                                  case SELECT_FOCUS:

                                                        Command_stack[2] = VISIBLE_QUERY_FOCUS;
                                                        Command_stack[1] = VISIBLE_QUERY_FOCUS_REPLY_HEADER;
                                                        break;
                               }
                               break;
    case THERMAL_SELECTED:
                              switch(ThermalCameraFunctionsSelect)
                              {
                                  case SELECT_ZOOM:
                                                        Command_stack[2] = THERMAL_QUERY_ZOOM;
                                                        Command_stack[1] = THERMAL_QUERY_ZOOM_REPLY_HEADER;
                                                        break;
                                  case SELECT_FOCUS:
                                                        Command_stack[2] = THERMAL_QUERY_FOCUS;
                                                        Command_stack[1] = THERMAL_QUERY_FOCUS_REPLY_HEADER;
                                                        break;
                               }
                               break;
   }
   Command_stack[0] = READ_ZOOM_AND_FOCUS;
   Command_no = 2;  
}

void save_AGC_SETPOINT(void)
{
   Command_stack[2] = VISIBLE_AGC_SETPOINT_SAVE;
   Command_stack[1] = VISIBLE_AGC_SETPOINT_SAVE_REPLY;
   Command_stack[0] = FINALIZE_THE_COMMAND;
   Command_no = 2;  
}

/*=================================================================================================================*/
/* functia de interpretare a comenzilor din stiva de comenzi */
/*=================================================================================================================*/
void interpret_commands_from_stack(void)
{
  switch(Command_stack[Command_no])
  {
/* Comenzi pentru camera de zi
====================================================================================================================*/
    case FINALIZE_THE_COMMAND:      
                                              switch(Memory_MasterCommandForCamera)
                                              {
                                                case (1<<BIT_CAMERA_COMMAND_CAMERA_ON_OFF):
                                                              ThermalCameraOnOff = ((!ThermalCameraOnOff) & 0x01);
                                                              TypeCameraSelected = 0;
                                                              set_Status_SystemCamere(0,((ThermalCameraOnOff << 1) | TypeCameraSelected));
                                                              break;
                                                case (1<<BIT_CAMERA_COMMAND_SEL_CAMERA_TYPE):
                                                              TypeCameraSelected = ((!TypeCameraSelected) & 0x01);
                                                              set_Status_SystemCamere(0,((ThermalCameraOnOff << 1) | TypeCameraSelected));
                                                              break;
                                                case (1<<BIT_COMANDA_BOOTING):
                                                              ThermalCameraOnOff = THERMAL_ON;
                                                              TypeCameraSelected = VISIBLE_SELECTED;
                                                              set_Status_SystemCamere(0,((ThermalCameraOnOff << 1) | TypeCameraSelected));
                                                              Memory_MasterCommandForCamera &= ~(1<<BIT_COMANDA_BOOTING);
                                                              break;                                                           
                                              }
                                              Status_TransmitCommand = DO_NOTHING;
                                              Semafor = VERDE;
                                              set_CameraErrorType(NO_ERROR); 
											  break;
    case READ_ZOOM_AND_FOCUS:
                                              Semafor = VERDE;
                                              switch(Memory_MasterCommandForCamera)
                                              {
                                                case 0:
                                                              camera_command_stop();
                                                              interpret_commands_from_stack();
                                                              break;
                                                case (1<<BIT_CAMERA_COMMAND_INCREASE):
                                                              camera_read_position_of_zoom_or_focus();
                                                              interpret_commands_from_stack();
                                                              break;
                                                case (1<<BIT_CAMERA_COMMAND_DECREASE):
                                                              camera_read_position_of_zoom_or_focus();
                                                              interpret_commands_from_stack();
                                                              break;
                                              }
                                              break;
    case REPEAT_COMMAND:
                                              Semafor = VERDE;
                                              switch(Memory_MasterCommandForCamera)
                                              {
                                                case 0:
                                                              save_AGC_SETPOINT();
                                                              interpret_commands_from_stack();
                                                              break;
                                                case (1<<BIT_CAMERA_COMMAND_INCREASE):
                                                              create_command_stack();
                                                              break;
                                                case (1<<BIT_CAMERA_COMMAND_DECREASE):
                                                              create_command_stack();
                                                              break;
                                              }
                                              break;
/*==================================================================================================================*/

    case BOTH_CAMERAS_POWER_UP:
                                              TransmissionBufferPointer = (U16)&BothCamerasPowerUp[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&BothCamerasPowerUp[1]);
                                              load_transmission_buffer();
                                              break;
/* nefolosita */
    case BOTH_CAMERAS_POWER_DOWN:
                                              TransmissionBufferPointer = (U16)&BothCamerasPowerDown[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&BothCamerasPowerDown[1]);
                                              load_transmission_buffer();
                                              break;
    case SELECT_VISIBLE:
                                              TransmissionBufferPointer = (U16)&SelectVisible[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&SelectVisible[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_POWER_UP:
                                              TransmissionBufferPointer = (U16)&VisiblePowerUp[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisiblePowerUp[1]);
                                              load_transmission_buffer();
                                              break;
/* nefolosita */
    case VISIBLE_POWER_DOWN:
                                              TransmissionBufferPointer = (U16)&VisiblePowerDown[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisiblePowerDown[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_CAMERA_ON:
                                              TransmissionBufferPointer = (U16)&VisibleOn[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleOn[1]);
                                              load_transmission_buffer();
                                              break;
/* nefolosita */
    case VISIBLE_CAMERA_OFF:
                                              TransmissionBufferPointer = (U16)&VisibleOff[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleOff[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_STOP_MOTORS:
                                              TransmissionBufferPointer = (U16)&VisibleStopMotors[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleStopMotors[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_SET_SPEED_25:
                                              TransmissionBufferPointer = (U16)&VisibleSetMCSpeed25[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleSetMCSpeed25[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_SET_SPEED_50:
                                              TransmissionBufferPointer = (U16)&VisibleSetMCSpeed50[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleSetMCSpeed50[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_SET_SPEED_75:
                                              TransmissionBufferPointer = (U16)&VisibleSetMCSpeed75[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleSetMCSpeed75[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_SET_SPEED_100:
                                              TransmissionBufferPointer = (U16)&VisibleSetMCSpeed100[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleSetMCSpeed100[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_ZOOM_NARROW:
                                              TransmissionBufferPointer = (U16)&VisibleZoomNarrow[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleZoomNarrow[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_ZOOM_WIDE:
                                              TransmissionBufferPointer = (U16)&VisibleZoomWide[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleZoomWide[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_FOCUS_NEAR:
                                              TransmissionBufferPointer = (U16)&VisibleFocusNear[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleFocusNear[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_FOCUS_FAR:
                                              TransmissionBufferPointer = (U16)&VisibleFocusFar[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleFocusFar[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_AGC_SETPOINT_UP:
                                              TransmissionBufferPointer = (U16)&VisibleAGCSetPointUp[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleAGCSetPointUp[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_AGC_SETPOINT_DOWN:
                                              TransmissionBufferPointer = (U16)&VisibleAGCSetPointDown[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleAGCSetPointDown[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_AGC_SETPOINT_SAVE:
                                              TransmissionBufferPointer = (U16)&VisibleAGCSetPointSave[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleAGCSetPointSave[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_QUERY_ZOOM:
                                              TransmissionBufferPointer = (U16)&VisibleQueryZoomPosition[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleQueryZoomPosition[1]);
                                              load_transmission_buffer();
                                              break;
    case VISIBLE_QUERY_FOCUS:
                                              TransmissionBufferPointer = (U16)&VisibleQueryFocusPosition[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&VisibleQueryFocusPosition[1]);
                                              load_transmission_buffer();
                                              break;
/* reply
=============================================*/
    case BOTH_CAMERAS_POWER_UP_REPLY:
                                              ReplyBufferPointer = (U16)&BothCamerasPowerUpReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&BothCamerasPowerUpReply[1]);
                                              prepare_for_receiving();
                                              break;
/* nefolosita */
    case BOTH_CAMERAS_POWER_DOWN_REPLY:
                                              ReplyBufferPointer = (U16)&BothCamerasPowerDownReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&BothCamerasPowerDownReply[1]);
                                              prepare_for_receiving();
                                              break;
    case SELECT_VISIBLE_REPLY:
                                              ReplyBufferPointer = (U16)&SelectVisibleReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&SelectVisibleReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_POWER_UP_REPLY:
                                              ReplyBufferPointer = (U16)&VisiblePowerUpReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisiblePowerUpReply[1]);
                                              prepare_for_receiving();
                                              break;
/* nefolosita */
    case VISIBLE_POWER_DOWN_REPLY:
                                              ReplyBufferPointer = (U16)&VisiblePowerDownReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisiblePowerDownReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_CAMERA_ON_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleOnReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleOnReply[1]);
                                              prepare_for_receiving();
                                              break;
/* nefolosita */
    case VISIBLE_CAMERA_OFF_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleOffReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleOffReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_STOP_MOTORS_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleStopMotorsReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleStopMotorsReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_SET_SPEED_25_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleSetMCSpeed25Reply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleSetMCSpeed25Reply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_SET_SPEED_50_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleSetMCSpeed50Reply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleSetMCSpeed50Reply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_SET_SPEED_75_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleSetMCSpeed75Reply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleSetMCSpeed75Reply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_SET_SPEED_100_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleSetMCSpeed100Reply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleSetMCSpeed100Reply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_ZOOM_NARROW_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleZoomNarrowReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleZoomNarrowReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_ZOOM_WIDE_REPLY:      
                                              ReplyBufferPointer = (U16)&VisibleZoomWideReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleZoomWideReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_FOCUS_NEAR_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleFocusNearReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleFocusNearReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_FOCUS_FAR_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleFocusFarReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleFocusFarReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_AGC_SETPOINT_UP_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleAGCSetPointUpReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleAGCSetPointUpReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_AGC_SETPOINT_DOWN_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleAGCSetPointDownReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleAGCSetPointDownReply[1]);
                                              prepare_for_receiving();
                                              break;
    case VISIBLE_AGC_SETPOINT_SAVE_REPLY:
                                              ReplyBufferPointer = (U16)&VisibleAGCSetPointSaveReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleAGCSetPointSaveReply[1]);
                                              prepare_for_receiving();
                                              break;

    case VISIBLE_QUERY_ZOOM_REPLY_HEADER:
                                              ReplyBufferPointer = (U16)&VisibleQueryZoomPositionReplyHeader[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleQueryZoomPositionReplyHeader[1]);
                                              prepare_for_receiving_and_take_data();
                                              break;
    
	case VISIBLE_QUERY_FOCUS_REPLY_HEADER:
                                              ReplyBufferPointer = (U16)&VisibleQueryFocusPositionReplyHeader[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&VisibleQueryFocusPositionReplyHeader[1]);
                                              prepare_for_receiving_and_take_data();
                                              break;
/* sectiune functii camera termica */
/*=============================================================================================================================================*/


    case SELECT_THERMAL:
                                              TransmissionBufferPointer = (U16)&SelectThermal[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&SelectThermal[1]);
                                              load_transmission_buffer();
                                              break;
/* nefolosita */
    case THERMAL_POWER_UP:
                                              TransmissionBufferPointer = (U16)&ThermalPowerUp[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalPowerUp[1]);
                                              load_transmission_buffer();
                                              break;
/* nefolosita */
    case THERMAL_POWER_DOWN:
                                              TransmissionBufferPointer = (U16)&ThermalaPowerDown[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalaPowerDown[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_STOP_MOTORS:
                                              TransmissionBufferPointer = (U16)&ThermalStopMotors[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalStopMotors[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_SET_SPEED_25:
                                              TransmissionBufferPointer = (U16)&ThermalSetMCSpeed25[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalSetMCSpeed25[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_SET_SPEED_50:
                                              TransmissionBufferPointer = (U16)&ThermalSetMCSpeed50[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalSetMCSpeed50[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_SET_SPEED_75:
                                              TransmissionBufferPointer = (U16)&ThermalSetMCSpeed75[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalSetMCSpeed75[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_SET_SPEED_100:
                                              TransmissionBufferPointer = (U16)&ThermalSetMCSpeed100[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalSetMCSpeed100[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_ZOOM_IN:
                                              TransmissionBufferPointer = (U16)&ThermalZoomIn[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalZoomIn[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_ZOOM_OUT:
                                              TransmissionBufferPointer = (U16)&ThermalZoomOut[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalZoomOut[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_FOCUS_IN:
                                              TransmissionBufferPointer = (U16)&ThermalFocusIn[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalFocusIn[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_FOCUS_OUT:
                                              TransmissionBufferPointer = (U16)&ThermalFocusOut[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalFocusOut[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_SET_COLD_WHITE:
                                              TransmissionBufferPointer = (U16)&ThermalSetColdWhite[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalSetColdWhite[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_SET_HOT_WHITE:
                                              TransmissionBufferPointer = (U16)&ThermalSetHotWhite[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalSetHotWhite[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_QUERY_ZOOM:
                                              TransmissionBufferPointer = (U16)&ThermalQueryZoomPosition[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalQueryZoomPosition[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_QUERY_FOCUS:
                                              TransmissionBufferPointer = (U16)&ThermalQueryFocusPosition[0];
                                              NbOfBytes_SentToRS232 = pgm_read_byte_near(&ThermalQueryFocusPosition[1]);
                                              load_transmission_buffer();
                                              break;
/* thermal reply
====================================================*/
/* nefolosita */
    case THERMAL_POWER_UP_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalPowerUpReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalPowerUpReply[1]);
                                              prepare_for_receiving();
                                              break;
    case SELECT_THERMAL_REPLY:
                                              ReplyBufferPointer = (U16)&SelectThermalReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&SelectThermalReply[1]);
                                              prepare_for_receiving();
                                              break;
/* nefolosita */
    case THERMAL_POWER_DOWN_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalPowerDownReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalPowerDownReply[1]);
                                              prepare_for_receiving();
                                              break;
    case THERMAL_STOP_MOTORS_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalStopMotorsReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalStopMotorsReply[1]);
                                              prepare_for_receiving();
                                              break;
    case THERMAL_SET_SPEED_25_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalSetMCSpeed25Reply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalSetMCSpeed25Reply[1]);
                                              prepare_for_receiving();
                                              break;
    case THERMAL_SET_SPEED_50_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalSetMCSpeed50Reply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalSetMCSpeed50Reply[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_SET_SPEED_75_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalSetMCSpeed75Reply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalSetMCSpeed75Reply[1]);
                                              load_transmission_buffer();
                                              break;
    case THERMAL_SET_SPEED_100_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalSetMCSpeed100Reply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalSetMCSpeed100Reply[1]);
                                              prepare_for_receiving();
                                              break;
    case THERMAL_ZOOM_IN_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalZoomInReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalZoomInReply[1]);
                                              prepare_for_receiving();
                                              break;
    case THERMAL_ZOOM_OUT_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalZoomOutReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalZoomOutReply[1]);
                                              prepare_for_receiving();
                                              break;
    case THERMAL_FOCUS_IN_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalFocusInReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalFocusInReply[1]);
                                              prepare_for_receiving();
                                              break;
    case THERMAL_FOCUS_OUT_REPLY:
                                              ReplyBufferPointer = (U16)&ThermalFocusOutReply[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalFocusOutReply[1]);
                                              prepare_for_receiving();
                                              break;
    case THERMAL_QUERY_ZOOM_REPLY_HEADER:
                                              ReplyBufferPointer = (U16)&ThermalQueryZoomPositionReplyHeader[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalQueryZoomPositionReplyHeader[1]);
                                              prepare_for_receiving_and_take_data();                                              
                                              break;
    case THERMAL_QUERY_FOCUS_REPLY_HEADER:
                                              ReplyBufferPointer = (U16)&ThermalQueryFocusPositionReplyHeader[0];
                                              NbOfBytes_HaveToBeReceivedFromRS232 = pgm_read_byte_near(&ThermalQueryFocusPositionReplyHeader[1]);
                                              prepare_for_receiving_and_take_data();
                                              break;
    case EXECUTE_5ms_DELAY:
                                              TimeOutForRS232_Contor = 0;
                                              TimeOutForRS232_Reference = WAIT_AT_LEAST_5ms; 
                                              Status_TransmitCommand = INSERT_DELAY;
                                              break;
    case EXECUTE_10ms_DELAY:
                                              TimeOutForRS232_Contor = 0;
                                              TimeOutForRS232_Reference = WAIT_AT_LEAST_10ms; 
                                              Status_TransmitCommand = INSERT_DELAY;
                                              break;
    case EXECUTE_500ms_DELAY:
                                              TimeOutForRS232_Contor = 0;
                                              TimeOutForRS232_Reference = WAIT_AT_LEAST_500ms; 
                                              Status_TransmitCommand = INSERT_DELAY;
                                              break;
  }
}


void load_transmission_buffer(void)
{
  register U8 c = 0;

  while(c != NbOfBytes_SentToRS232)
  {
    TransmissionBufferRS232[c++] = pgm_read_byte_near(TransmissionBufferPointer++);
  }
  //ResetRS232Reception();
  TransmissionPointer = 0;
  Uart1_send_byte(TransmissionBufferRS232[0]);
  Status_TransmitCommand = TRANSMIT_COMMAND;
  ResetRS232Reception();
}

void prepare_for_receiving(void)
{
  ResetTimeOut_Contor();
  TimeOutForRS232_Reference = WAIT_AT_LEAST_50ms;             /* referinta pentru Time Out */
  Status_TransmitCommand = WAIT_FOR_ACKNOWLEDGE;
}

void prepare_for_receiving_and_take_data(void)
{
  ResetTimeOut_Contor();
  TimeOutForRS232_Reference = WAIT_AT_LEAST_50ms;
  Status_TransmitCommand = WAIT_FOR_ACKNOWLEDGE_AND_TAKE_DATA;
}

/* functia prin care se transmit datele pe seriala RS232 */
void perform_command(void)
{
  switch(Status_TransmitCommand)
  {
   case DO_NOTHING:
                                             break;
   case TRANSMIT_COMMAND:
                                             /* Transmite comanda catre sistemul de camere */
                                             if (TransmiteDataToRS232() == TRANSMISSION_OK)
                                             {
                                                ResetRS232Reception();
                                                Command_no--;
                                                /* interpreteaza urmatoarea comanda din stiva de comenzi */
                                                interpret_commands_from_stack();
                                             }
                                             break;
   case WAIT_FOR_ACKNOWLEDGE:
                                             /* asteapta mesajul de confirmare pentru comanda */
                                             if (TimeOutForRS232_Contor < TimeOutForRS232_Reference)
                                             {
                                               if (ByteIndex_ReceivedFromRS232 == NbOfBytes_HaveToBeReceivedFromRS232)
                                               {Status_TransmitCommand = COMPARE_STRINGS;}
                                             }
                                             else
                                             {
                                               /* s-a depasit TimeOut-ul pentru string-ului de confirmare */
                                               /* posibil camera neconectata sau comanda nu a fost recunoscuta */
                                               /* se reface stiva de comenzi de NB_OF_INCERCARI ori */ 
                                               if (--Nr_incercari == 0)
                                               {
                                                 set_CameraErrorType(CAMERA_NO_ANSWER_ERROR);
                                                 Status_TransmitCommand = DO_NOTHING; 
                                                 Semafor = VERDE;
                                               }
                                               else
                                               {
                                                  /* refacerea stivei de comenzi */ 
                                                  create_command_stack();
                                               }
                                             }
                                             break;
   case COMPARE_STRINGS:
                                             /* compara string-ul receptionat de la sistemul de camere cu string-ul de referinta, asteptat*/
                                             if (compare_strings() == OK)
                                             {
                                               Command_no--;
                                               /* interpreteaza urmatoarea comanda din stiva */
                                               interpret_commands_from_stack();
                                             } 
                                             else
                                             {
                                               /* compararea string-urilor a esuat ; posibil desincronizarea comunicatiei */
                                               /* transmite byte-ul de resincronizare(0x16) */
                                               if (compare_for_overflow_or_syn() == OK)  
                                               {
                                                 while(!Uart1_tx_ready());Uart1_send_byte(0x16);
                                                 Status_TransmitCommand = SEND_SYN;
                                               }
                                               /* daca nu este o problema de resincronizare, repeta de NB_OF_INCERCARI ori */
                                               /* stiva de comenzi */
                                               else
                                               {
                                                 if (--Nr_incercari == 0)
                                                 {
                                                   set_CameraErrorType(CAMERA_MIS_MATCHING_STRINGS);
                                                   Status_TransmitCommand = DO_NOTHING; 
                                                   Semafor = VERDE;
                                                 }
                                                 else
                                                 {
                                                    /* refacerea stivei de comenzi */ 
                                                    create_command_stack();
                                                 }
                                               }
                                             }
                                             break;
   case SEND_SYN:
                                             /* asteapta sa se transmita byte-ul de sincronizare */
                                             /* reface stiva de comenzi */
                                             if (Uart1_tx_complet())
                                             {
                                               if (--Nr_incercari == 0)
                                               {
                                                 set_CameraErrorType(CAMERA_IS_OUT_OF_SYNCHRONIZATION);
                                                 Status_TransmitCommand = DO_NOTHING; 
                                                 Semafor = VERDE;
                                               }
                                               else
                                               {
                                                  /* refacerea stivei de comenzi */ 
                                                  create_command_stack();
                                               }
                                             } 
                                             break;
   case INSERT_DELAY:
                                             if (TimeOutForRS232_Contor > TimeOutForRS232_Reference)
                                             {
                                                Command_no--;
                                                interpret_commands_from_stack();
                                             }
                                             break;
   case WAIT_FOR_ACKNOWLEDGE_AND_TAKE_DATA:
                                             /* asteapta mesajul de confirmare pentru comanda */
                                             if (TimeOutForRS232_Contor < TimeOutForRS232_Reference)
                                             {
                                               if (ByteIndex_ReceivedFromRS232 == NbOfBytes_HaveToBeReceivedFromRS232)
                                               {Status_TransmitCommand = COMPARE_STRINGS_AND_TAKE_DATA;}
                                             }
                                             else
                                             {
                                               /* s-a depasit TimeOut-ul pentru string-ului de confirmare */
                                               /* posibil camera neconectata sau comanda nu a fost recunoscuta */
                                               /* se reface stiva de comenzi de NB_OF_INCERCARI ori */ 
                                               if (--Nr_incercari == 0)
                                               {
                                                 set_CameraErrorType(CAMERA_NO_ANSWER_ERROR);
                                                 Status_TransmitCommand = DO_NOTHING; 
                                                 Semafor = VERDE;
                                               }
                                               else
                                               {
                                                 /* refacerea stivei de comenzi */ 
                                                 create_command_stack();
                                               }
                                             }
                                             break;
   case COMPARE_STRINGS_AND_TAKE_DATA:
                                            /* compara string-ul receptionat de la sistemul de camere cu string-ul de referinta, asteptat*/
                                             if (compare_strings_and_take_data() == OK)
                                             {
                                               Command_no--;
                                               /* interpreteaza urmatoarea comanda din stiva de comenzi*/
                                               interpret_commands_from_stack();     
                                             } 
                                             else
                                             {
                                               /* compararea string-urilor a esuat ; posibil desincronizarea comunicatiei */
                                               /* transmite byte-ul de resincronizare(0x16) */
                                               if (compare_for_overflow_or_syn() == OK)  
                                               {
                                                 while(!Uart1_tx_ready());Uart1_send_byte(0x16);
                                                 Status_TransmitCommand = SEND_SYN;
                                               }
                                               /* daca nu este o problema de resincronizare, repeta de NB_OF_INCERCARI ori */
                                               /* stiva de comenzi */
                                               else
                                               {
                                                 //Command_no = Recovered_Command_no;
                                                 Command_no++; /* repeta ultima comanda din stiva, este vorba de 
                                                               query pentru zoom si focus */
                                                 /* interpreteaza urmatoarea comanda anterioara din stiva de comenzi,
                                                    adica, comanda de query*/
                                                 interpret_commands_from_stack();
                                               }
                                             }
                                             break;
    }
}

/* functia de comparare a confirmarilor primite de la camera */
U8 compare_strings(void)
{
  register U8 c;

  c = 0;
  do
  { 
    if (pgm_read_byte_near(ReplyBufferPointer++) != ReceivingBufferRS232[c++]) {return KO;}
  }
  while (c != NbOfBytes_HaveToBeReceivedFromRS232);

  return OK;
}


U8 compare_strings_and_take_data(void)
{
  register U8 c, c1;

  c = 0;
  c1 = 0;
  switch (TypeCameraSelected)
  {
    case VISIBLE_SELECTED:
                           switch(VisibleCameraFunctionsSelect)
                           {
                             case SELECT_ZOOM:
                                                  do
                                                  {
                                                    if (pgm_read_byte_near(ReplyBufferPointer++) != ReceivingBufferRS232[c++]) return KO;
                                                  }
                                                  while (c != (NbOfBytes_HaveToBeReceivedFromRS232 - 7));
                                                  CS1 = ReceivingBufferRS232[17];
                                                  CS2 = ReceivingBufferRS232[19];
                                                  if (verify_the_check_sum_for_visible() == KO) return KO;
                                                  do
                                                  {
                                                    set_VisibleDataZoom(c1++, ReceivingBufferRS232[c++]);
                                                  }
                                                  while (c != (NbOfBytes_HaveToBeReceivedFromRS232 - 3));
                                                  break;
                             case SELECT_FOCUS:
                                                  do
                                                  {
                                                    if (pgm_read_byte_near(ReplyBufferPointer++) != ReceivingBufferRS232[c++])  return KO;
                                                  }
                                                  while (c != (NbOfBytes_HaveToBeReceivedFromRS232 - 7));
                                                  CS1 = ReceivingBufferRS232[17];
                                                  CS2 = ReceivingBufferRS232[19];
                                                  if (verify_the_check_sum_for_visible() == KO) return KO;
                                                  do
                                                  {
                                                    set_VisibleDataFocus(c1++, ReceivingBufferRS232[c++]);
                                                  }
                                                  while (c != (NbOfBytes_HaveToBeReceivedFromRS232 - 3));
                                                  break;
                           }
                           break;
    case THERMAL_SELECTED:
 
                           switch(ThermalCameraFunctionsSelect)
                           {
                              case SELECT_ZOOM:
                                                  do
                                                  {
                                                    if (pgm_read_byte_near(ReplyBufferPointer++) != ReceivingBufferRS232[c++]) return KO;
                                                  }
                                                  while (c != (NbOfBytes_HaveToBeReceivedFromRS232 - 10));
                                                  CS1 = ReceivingBufferRS232[22];
                                                  CS2 = ReceivingBufferRS232[25];
                                                  CS3 = ReceivingBufferRS232[27];
                                                  if (verify_the_check_sum_for_thermal() == KO) return KO;
                                                  do
                                                  {
                                                    set_ThermalDataZoom(c1++, ReceivingBufferRS232[c++]);
                                                  }
                                                  while (c != (NbOfBytes_HaveToBeReceivedFromRS232 - 6));
                                                  break;
                             case SELECT_FOCUS:
                                                  do
                                                  {
                                                    if (pgm_read_byte_near(ReplyBufferPointer++) != ReceivingBufferRS232[c++])  return KO;
                                                  }
                                                  while (c != (NbOfBytes_HaveToBeReceivedFromRS232 - 10));
                                                  CS1 = ReceivingBufferRS232[22];
                                                  CS2 = ReceivingBufferRS232[25];
                                                  CS3 = ReceivingBufferRS232[27];
                                                  if (verify_the_check_sum_for_thermal() == KO) return KO;
                                                  do
                                                  {
                                                    set_ThermalDataFocus(c1++, ReceivingBufferRS232[c++]);
                                                  }
                                                  while (c != (NbOfBytes_HaveToBeReceivedFromRS232 - 6));
                                                  break;
                            }
                            break;
   }
  return OK;
}

U8 verify_the_check_sum_for_visible(void)
{
if  (((ReceivingBufferRS232[0] ^ ReceivingBufferRS232[1] ^ ReceivingBufferRS232[2] ^ ReceivingBufferRS232[3] ^
     ReceivingBufferRS232[4] ^ ReceivingBufferRS232[5] ^ ReceivingBufferRS232[6] ^ ReceivingBufferRS232[7] ^
     ReceivingBufferRS232[8] ^ ReceivingBufferRS232[9] ^ ReceivingBufferRS232[10] ^ ReceivingBufferRS232[11] ^
     ReceivingBufferRS232[12] ^ ReceivingBufferRS232[13] ^ ReceivingBufferRS232[14] ^ ReceivingBufferRS232[15] ^
     ReceivingBufferRS232[16] ^ ReceivingBufferRS232[17] ^ ReceivingBufferRS232[18]) == CS2) &&
    ((U8)(ReceivingBufferRS232[10] + ReceivingBufferRS232[11] + ReceivingBufferRS232[12] + ReceivingBufferRS232[13] +
          ReceivingBufferRS232[14] + ReceivingBufferRS232[15] + ReceivingBufferRS232[16]) == CS1))
     {return OK;}
     else {return KO;}
}

U8 verify_the_check_sum_for_thermal(void)
{
if  ((CS3 == 0x7e) &&
    ((U8)(ReceivingBufferRS232[15] + ReceivingBufferRS232[16] + ReceivingBufferRS232[17] + ReceivingBufferRS232[18] +
          ReceivingBufferRS232[19] + ReceivingBufferRS232[20] + ReceivingBufferRS232[21]) == (U8)CS1) &&
    ((U8)(ReceivingBufferRS232[4] ^ ReceivingBufferRS232[5] ^ ReceivingBufferRS232[6] ^ ReceivingBufferRS232[7] ^
          ReceivingBufferRS232[8] ^ ReceivingBufferRS232[9] ^ ReceivingBufferRS232[10] ^ ReceivingBufferRS232[11] ^
          ReceivingBufferRS232[12] ^ ReceivingBufferRS232[13] ^ ReceivingBufferRS232[14] ^ ReceivingBufferRS232[15] ^
          ReceivingBufferRS232[16] ^ ReceivingBufferRS232[17] ^ ReceivingBufferRS232[18] ^ ReceivingBufferRS232[19] ^ 
          ReceivingBufferRS232[20] ^ ReceivingBufferRS232[21] ^ ReceivingBufferRS232[22] ^ ReceivingBufferRS232[23] ^
          ReceivingBufferRS232[24]) == CS2))
     {return OK;}
     else {return KO;}
}

U8 compare_for_overflow_or_syn(void)
{
  if (ReceivingBufferRS232[0] == 0x16) return OK;
  else return KO;
}

/* functia de citire a serialei */
void receive_acknowledge(void)
{
   ReceiveDataFromRS232();
}
