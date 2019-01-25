
#ifndef DRAWINGS_XVGA_H_
#define DRAWINGS_XVGA_H_

#include "compiler.h"

#define CROSS_STEP1                                1
#define CROSS_STEP2                                2
#define CROSS_STEP3                                3
#define CROSS_STEP4                                4
#define CROSS_STEP5                                5
#define CROSS_STEP6                                6
#define CROSS_STEP7                                7
#define CROSS_STEP8                                8
#define CROSS_STEP9                                9

#define FRAME_STEP1                                1
#define FRAME_STEP2                                2
#define FRAME_STEP3                                3
#define FRAME_STEP4                                4
#define FRAME_STEP5                                5
#define FRAME_STEP6                                6
#define FRAME_STEP7                                7
#define FRAME_STEP8                                8
#define FRAME_STEP9                                9

#define STANDARD_STEP1                             1
#define STANDARD_STEP2                             2
#define STANDARD_STEP3                             3
#define STANDARD_STEP4                             4
#define STANDARD_STEP5                             5
#define STANDARD_STEP6                             6
#define STANDARD_STEP7                             7
#define STANDARD_STEP8                             8
#define STANDARD_STEP9                             9
#define STANDARD_STEP10                            10
#define STANDARD_STEP11                            11
#define STANDARD_STEP12                            12

#define GRID_STEP1                                 1
#define GRID_STEP2                                 2
#define GRID_STEP3                                 3
#define GRID_STEP4                                 4
#define GRID_STEP5                                 5
#define GRID_STEP6                                 6
#define GRID_STEP7                                 7
#define GRID_STEP8                                 8
#define GRID_STEP9                                 9

#define FILL_STEP1                                 1
#define FILL_STEP2                                 2
#define FILL_STEP3                                 3
#define FILL_STEP4                                 4
#define FILL_STEP5                                 5
#define FILL_STEP6                                 6
#define FILL_STEP7                                 7
#define FILL_STEP8                                 8
#define FILL_STEP9                                 9

#define DELETE_STEP1                               1
#define DELETE_STEP2                               2
#define DELETE_STEP3                               3
#define DELETE_STEP4                               4
#define DELETE_STEP5                               5
#define DELETE_STEP6                               6
#define DELETE_STEP7                               7
#define DELETE_STEP8                               8
#define DELETE_STEP9                               9

#define CURSOR_STEP1                               1
#define CURSOR_STEP2                               2
#define CURSOR_STEP3                               3
#define CURSOR_STEP4                               4
#define CURSOR_STEP5                               5
#define CURSOR_STEP6                               6
#define CURSOR_STEP7                               7
#define CURSOR_STEP8                               8
#define CURSOR_STEP9                               9

#define TEST_STEP1                                 1
#define TEST_STEP2                                 2
#define TEST_STEP3                                 3
#define TEST_STEP4                                 4
#define TEST_STEP5                                 5


extern U8 bLg_Brat_c;
extern U16 wPoz_C_X;
extern U16 wPoz_C_Y;

extern U8 bPoz_D_X;
extern U8 bPoz_D_Y;

extern U16 wPoz_E_X;
extern U16 wPoz_E_Y;


extern U8 CrossStatus;
extern U8 RAEStatus;
extern U8 FrameScheduler;
extern U8 StandardScheduler;
extern U8 DeleteScheduler;
extern U8 GridScheduler;
extern U8 CursorScheduler;
extern U8 FillScheduler;


typedef struct DRAM
{
    U16 wLgX;
    S16 swOffsetX;
    S16 swOffsetY;
    U16 wRPM;
    U16 wPozAX;
    U16 wPozAY;
    U16 wPozBX;
    U16 wPozBY;
}DRAM;

typedef struct DRAL
{
    U16 wPozX;
    U16 wPozY;
    U16 wOriginX;
    U16 wOriginY;
    U16 wRPM;
    U16 wBrat;
}DRAL;

typedef struct DRAE
{
    U16 wPozX;
    U16 wPozY;
    U16 wOriginX;
    U16 wOriginY;
    U16 wRPM;
    U16 wBratX;
    U16 wBratY;
}DRAE;

typedef struct DRStandard
{
    char cTm[3];
    char cErr[6];
    char cDataX[4];
    char cDataY[4];
    U16 wPozRoX;
    U16 wPozRoY;
    U16 wPozX;
    U16 wPozY;
    S16 swOffsetX;
    S16 swOffsetY;
    U8 bFlags;
    U16 wRPM;
    U8 bLgRoX;
    U8 bLgRoY;
}DRStandard;

typedef struct DRGrid
{
    U8 bViteza_gm;
    U8 bSign;
    U32 AdresaGrid;
    U32 ScalarSpeed_100us;
    U8 StartMoving;
    U16 wRPM;
    U16 wClearance;
    U16 wPozX;
    U16 wPozY;
}DRGrid;

typedef struct CPoint
{
    U16 x;
    U16 y;
}CPoint;

typedef struct CDot
{
    U32 Adresa;
    U32 Color;
}CDot;

typedef struct DCross
{
    U8 bLg_brat;
    U16 wPoz_x;
    U16 wPoz_y;
}DCross;

typedef struct DFrame
{
    U8 bPoz_x;
    U8 bPoz_y;
}DFrame;

typedef struct DStandard
{
    char cTm[3];
    char cErr[6];
    char cDataX[4];
    char cDataY[4];
    U16 wPoz_ro_x;
    U16 wPoz_ro_y;
    U8 bLg_ro_x;
    U8 bLg_ro_y;
    U8 bPoz_x;
    U8 bPoz_y;
    U8 bFlags;
}DStandard;

typedef struct DGrid
{
    U8 bViteza_gm;
    U8 bSign;
    U32 AdresaGrid;
    U32 ScalarSpeed_100us;
    U8 StartMoving;
}DGrid;


CDot DrawFrame(CPoint p, const CPoint p1, const CPoint p2, const CPoint p3, const CPoint p4);
CDot DrawText(CPoint p, const CPoint BitmapOrigin, const char *str);
CDot DrawGrid(CPoint p, const CPoint CrossOrigin);
CDot DrawCrossCursor(volatile CPoint p, const CPoint CrossOrigin);
CDot DrawFrameCursor(CPoint p, const CPoint CursorOrigin);

void copy_string_in_memory(U32 address, const char *str);
void draw_the_cross(U32 start_address, U16 Color);
void draw_the_frame(U32 start_address, U16 Color);
void draw_the_grid(U32 start_address, U16 Color);
void fill_all_memory_with_color(U16 Color);
void DrawText_90(U32 start_panel_address, CPoint p, const char *str);
void DrawTextVariable_90(U32 start_panel_address, CPoint p, char *str);
U8 DisplayTextInVirtualFrame(U32 start_panel_address);

void copy_LettersCaps_in_memory_90(U32 address);
void copy_Letters_in_memory_90(U32 address);
void copy_FiguresAndSymbols_in_memory_90(U32 address);

U8 fill_memory_view_with_color(U32 address, U16 Color);
U8 draw_the_RAL_90(U32 panel_address, volatile DRAL * ral, U16 Color);
U8 draw_the_RAE_90(U32 panel_address, volatile DRAE * rae, U16 Color);
U8 draw_the_frame_90(U32 panel_address, DRAM * ram, U16 Color);
U8 draw_the_standard_90(U32 panel_address, volatile DRStandard * rstandard, U16 Color, U8 Refresh);
U8 draw_text_90(U32 panel_address, CPoint p, char *str, int size);
U8 delete_character_of_standard_view(U32 address, volatile DRStandard * rstandard, U16 Color);
U8 draw_ready_character(U32 panel_address, volatile DRStandard * rstandard, U16 Color);
U8 display_ready(U32 pannel_address, volatile DStandard * standard, U16 Color);
U8 hide_err_section(U32 panel_address, volatile DRStandard * rstandard, U16 Color);
U8 draw_the_grid_90(U32 panel_address, DRGrid * grid, U16 Color);
void draw_cursor_point(void);
U8 draw_frame_cursor(U32 cursor_address, volatile DRStandard * rstandard);
U8 clear_cursor_memory(U32 memory_cursor);

#endif
