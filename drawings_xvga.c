
#include "caractere_xvga.h"
#include "drawings_xvga.h"
#include "string.h"
#include "configuration_xvga.h"
#include "acces_porturi_xvga.h"
#include "usart_xvga.h"

U8 CrossStatus = CROSS_STEP1;
U8 RAEStatus = CROSS_STEP1;
U8 FrameScheduler =  FRAME_STEP1;
U8 StandardScheduler = STANDARD_STEP1;
U8 GridScheduler = GRID_STEP1;
U8 DeleteScheduler = DELETE_STEP1;
U8 CursorScheduler = CURSOR_STEP1;
U8 PointScheduler = CURSOR_STEP1;
U8 FillScheduler = FILL_STEP1;
U16 IndexCaracter;


void copy_string_in_memory(U32 address, const char *str)
{
CPoint P0;
CDot Pixel;
U16 x,y;

    for (y = 0; y < CHARACTER_HEIGHT; y++)
    {
        for (x = 0; x <((CHARACTER_WIDTH*strlen(str))>>1)+1 ; x++)
        {
            P0.x = x<<1;
            P0.y = y;
            Pixel = DrawText(P0,(const CPoint){0,0}, str);
            if (Pixel.Color != 0) write_to_S1D13506(Pixel.Adresa + address,Pixel.Color);
        }
    }
}


CDot DrawFrame(CPoint p, const CPoint p1, const CPoint p2, const CPoint p3, const CPoint p4)
{
    CDot Pixel = {0,0};

    Pixel.Adresa = (DISPLAY_RESOLUTION_X * p.y) + p.x;
    Pixel.Color = DISPLAY_BACKGROUND_COLOR;

    if ((((p.y == p1.y) || (p.y == p4.y)) && ((p.x >= p1.x) && (p.x <= p2.x)))||\
       ((p.y == p1.y + (p4.y - p1.y)/4) && ((p.x >= p1.x) && (p.x <= p2.x)))||\
       ((p.y == p1.y + (p4.y - p1.y)/2) && ((p.x >= p1.x) && (p.x <= p2.x)))||\
       (((p.y >= p1.y) && (p.y <= p4.y)) && ((p.x == p1.x + (p2.x - p1.x)/2)))||\
       (((p.y >= p1.y) && (p.y <= p4.y)) && ((p.x == p1.x) || (p.x == p2.x))))
       Pixel.Color |= DISPLAY_FOREGROUND_COLOR;

    p.x = p.x+1;

    if ((((p.y == p1.y) || (p.y == p4.y)) && ((p.x >= p1.x) && (p.x <= p2.x)))||\
       ((p.y == p1.y + (p4.y - p1.y)/4) && ((p.x >= p1.x) && (p.x <= p2.x)))||\
       ((p.y == p1.y + (p4.y - p1.y)/2) && ((p.x >= p1.x) && (p.x <= p2.x)))||\
       (((p.y >= p1.y) && (p.y <= p4.y)) && ((p.x == p1.x + (p2.x - p1.x)/2)))||\
       (((p.y >= p1.y) && (p.y <= p4.y)) && ((p.x == p1.x) || (p.x == p2.x))))
       Pixel.Color |= (DISPLAY_FOREGROUND_COLOR << 8);

    return Pixel;
}


//**************************************************************************
// p = punctul curent
// BitmapOrigin = punctul de unde se va afisa textul
// str = tringul afisat
//**************************************************************************
CDot DrawText(volatile CPoint p, const CPoint BitmapOrigin, const char *str)
{
    char c;
    CDot Pixel;
    U8 IndexCaracter, IndexPixel;
    U16 Index;

    //usart_putchar (XVGA_USART, strlen(str));
    //usart_putchar (XVGA_USART, str[1]);

    Pixel.Adresa = p.y * DISPLAY_RESOLUTION_X + p.x;
    Pixel.Color = DISPLAY_BACKGROUND_COLOR;

    if ((p.y >= BitmapOrigin.y) && (p.x >= BitmapOrigin.x))
    {
        //usart_putchar (XVGA_USART, str[0]);
        IndexCaracter = ((p.x - BitmapOrigin.x)/CHARACTER_WIDTH);
        //usart_putchar (XVGA_USART, IndexCaracter);
        IndexPixel = (p.x - BitmapOrigin.x) - IndexCaracter*CHARACTER_WIDTH;
        if ((IndexCaracter <= (strlen(str)-1)) && (p.y <= (BitmapOrigin.y + CHARACTER_HEIGHT - 1)))
        {
            //Index = floor((p.x - BitmapOrigin.x)/CHARACTER_WIDTH);
            c = str[IndexCaracter];
            if ((c >= 0x61) && (c <= 0x7a))
            {
                Index = c - 0x61;
                if (Letters[(p.y - BitmapOrigin.y)*MATRIX_LETTERS_WIDTH + Index*CHARACTER_WIDTH+IndexPixel] == '*')
                    Pixel.Color = DISPLAY_FOREGROUND_COLOR;
            }
            else if ((c >= 0x41) && (c <= 0x5a))
            {
                Index = c - 0x41;
                if (LettersCaps[(p.y - BitmapOrigin.y)*MATRIX_LETTERS_CAPS_WIDTH + Index*CHARACTER_WIDTH+IndexPixel] == '*')
                    Pixel.Color = DISPLAY_FOREGROUND_COLOR;
            }
            else if ((c >= 0x20) && (c <= 0x3f))
            {
                Index = c - 0x20;
                if (FiguresAndSymbols[(p.y - BitmapOrigin.y)*MATRIX_FIGURURES_AND_SYMBOLS_WIDTH + Index*CHARACTER_WIDTH+IndexPixel] == '*')
                    Pixel.Color = DISPLAY_FOREGROUND_COLOR;
            }
        }
    }

    p.x = p.x + 1;

    if ((p.y >= BitmapOrigin.y) && (p.x >= BitmapOrigin.x))
       {
            //usart_putchar (XVGA_USART, str[0]);
            IndexCaracter = ((p.x - BitmapOrigin.x)/CHARACTER_WIDTH);
            //usart_putchar (XVGA_USART, IndexCaracter);
            IndexPixel = (p.x - BitmapOrigin.x) - IndexCaracter*CHARACTER_WIDTH;
            if ((IndexCaracter <= (strlen(str)-1)) && (p.y <= (BitmapOrigin.y + CHARACTER_HEIGHT - 1)))
            {
                //Index = floor((p.x - BitmapOrigin.x)/CHARACTER_WIDTH);
                c = str[IndexCaracter];
                if ((c >= 0x61) && (c <= 0x7a))
                {
                    Index = c - 0x61;
                    if (Letters[(p.y - BitmapOrigin.y)*MATRIX_LETTERS_WIDTH + Index*CHARACTER_WIDTH+IndexPixel] == '*')
                       Pixel.Color |= (DISPLAY_FOREGROUND_COLOR << 8);
                }
                else if ((c >= 0x41) && (c <= 0x5a))
                {
                    Index = c - 0x41;
                    if (LettersCaps[(p.y - BitmapOrigin.y)*MATRIX_LETTERS_CAPS_WIDTH + Index*CHARACTER_WIDTH+IndexPixel] == '*')
                        Pixel.Color |= (DISPLAY_FOREGROUND_COLOR << 8);
                }
                else if ((c >= 0x20) && (c <= 0x3f))
                {
                    Index = c - 0x20;
                    if (FiguresAndSymbols[(p.y - BitmapOrigin.y)*MATRIX_FIGURURES_AND_SYMBOLS_WIDTH + Index*CHARACTER_WIDTH+IndexPixel] == '*')
                        Pixel.Color |= (DISPLAY_FOREGROUND_COLOR << 8);
                }
            }
        }
    return Pixel;
}

CDot DrawGrid(CPoint p, const CPoint PO)
{
    CDot Pixel = {0,0};

    Pixel.Adresa = (DISPLAY_RESOLUTION_X * p.y) + p.x;
    Pixel.Color = DISPLAY_BACKGROUND_COLOR;

    if (((p.x == PO.x) || (p.x == PO.x + WIDTH_GRID) ||\
       (p.x == PO.x + 2*WIDTH_GRID) || (p.x == PO.x + 3*WIDTH_GRID)) &&\
       (p.y >= PO.y) && (p.y <= PO.y + HEIGHT_GRID))
        Pixel.Color |= DISPLAY_FOREGROUND_COLOR;

    p.x = p.x+1;

    if (((p.x == PO.x) || (p.x == PO.x + WIDTH_GRID) ||\
       (p.x == PO.x + 2*WIDTH_GRID) || (p.x == PO.x + 3*WIDTH_GRID)) &&\
       (p.y >= PO.y) && (p.y <= PO.y + HEIGHT_GRID))
       Pixel.Color |= (DISPLAY_FOREGROUND_COLOR << 8);

    return Pixel;
}


//***************************************************************************************************
// p = punctul curent
// p1 = punctul stanga sus
// p2 = punctul dreapta sus
// p3 = punctul dreapta jos
// p4 = punctul stanga jos
//***************************************************************************************************
CDot DrawCrossCursor(CPoint p, const CPoint CrossOrigin)
{
    CDot Pixel;

    Pixel.Adresa =16 * p.y + p.x;
    Pixel.Color = 0x0000AAAA;

    p.x = p.x * 4;
    if ((p.y >= (CrossOrigin.y - CROSS_ARM)) && (p.y <= (CrossOrigin.y + CROSS_ARM)) && (p.x >= (CrossOrigin.x - CROSS_ARM)) && (p.x <= (CrossOrigin.x + CROSS_ARM)))
    {if ((p.x == CrossOrigin.x) || (p.y == CrossOrigin.y)){Pixel.Color &= ~0x00000080; Pixel.Color |= 0x00000040;}}
    p.x = p.x + 1;
    if ((p.y >= (CrossOrigin.y - CROSS_ARM)) && (p.y <= (CrossOrigin.y + CROSS_ARM)) && (p.x >= (CrossOrigin.x - CROSS_ARM)) && (p.x <= (CrossOrigin.x + CROSS_ARM)))
    {if ((p.x == CrossOrigin.x) || (p.y == CrossOrigin.y)){Pixel.Color &= ~0x00000020; Pixel.Color |= 0x00000010;}}
    p.x = p.x + 1;
    if ((p.y >= (CrossOrigin.y - CROSS_ARM)) && (p.y <= (CrossOrigin.y + CROSS_ARM)) && (p.x >= (CrossOrigin.x - CROSS_ARM)) && (p.x <= (CrossOrigin.x + CROSS_ARM)))
    {if ((p.x == CrossOrigin.x) || (p.y == CrossOrigin.y)){Pixel.Color &= ~0x00000008; Pixel.Color |= 0x00000004;}}
    p.x = p.x + 1;
    if ((p.y >= (CrossOrigin.y - CROSS_ARM)) && (p.y <= (CrossOrigin.y + CROSS_ARM)) && (p.x >= (CrossOrigin.x - CROSS_ARM)) && (p.x <= (CrossOrigin.x + CROSS_ARM)))
    {if ((p.x == CrossOrigin.x) || (p.y == CrossOrigin.y)){Pixel.Color &= ~0x00000002; Pixel.Color |= 0x00000001;}}
    p.x = p.x + 1;
    if ((p.y >= (CrossOrigin.y - CROSS_ARM)) && (p.y <= (CrossOrigin.y + CROSS_ARM)) && (p.x >= (CrossOrigin.x - CROSS_ARM)) && (p.x <= (CrossOrigin.x + CROSS_ARM)))
    {if ((p.x == CrossOrigin.x) || (p.y == CrossOrigin.y)){Pixel.Color &= ~0x00008000; Pixel.Color |= 0x00004000;}}
    p.x = p.x + 1;
    if ((p.y >= (CrossOrigin.y - CROSS_ARM)) && (p.y <= (CrossOrigin.y + CROSS_ARM)) && (p.x >= (CrossOrigin.x - CROSS_ARM)) && (p.x <= (CrossOrigin.x + CROSS_ARM)))
    {if ((p.x == CrossOrigin.x) || (p.y == CrossOrigin.y)){Pixel.Color &= ~0x00002000; Pixel.Color |= 0x00001000;}}
    p.x = p.x + 1;
    if ((p.y >= (CrossOrigin.y - CROSS_ARM)) && (p.y <= (CrossOrigin.y + CROSS_ARM)) && (p.x >= (CrossOrigin.x - CROSS_ARM)) && (p.x <= (CrossOrigin.x + CROSS_ARM)))
    {if ((p.x == CrossOrigin.x) || (p.y == CrossOrigin.y)){Pixel.Color &= ~0x00000800; Pixel.Color |= 0x00000400;}}
    p.x = p.x + 1;
    if ((p.y >= (CrossOrigin.y - CROSS_ARM)) && (p.y <= (CrossOrigin.y + CROSS_ARM)) && (p.x >= (CrossOrigin.x - CROSS_ARM)) && (p.x <= (CrossOrigin.x + CROSS_ARM)))
    {if ((p.x == CrossOrigin.x) || (p.y == CrossOrigin.y)){Pixel.Color &= ~0x00000200; Pixel.Color |= 0x00000100;}}

    return Pixel;
}


CDot DrawFrameCursor(CPoint p, const CPoint CursorOrigin)
{
    CDot Pixel;

    Pixel.Adresa = 16 * p.y + p.x;
    Pixel.Color = 0x0000AAAA;

    p.x = p.x * 4;
    if (((((p.y == (CursorOrigin.y - HALF_BIG_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_BIG_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_BIG_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_BIG_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_BIG_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_BIG_FRAME_X))))) ||\
        ((((p.y == (CursorOrigin.y - HALF_SMALL_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_SMALL_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_SMALL_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_SMALL_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_SMALL_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_SMALL_FRAME_X))))))
           {Pixel.Color &= ~0x00000080; Pixel.Color |= 0x00000040;}
    p.x = p.x + 1;
    if (((((p.y == (CursorOrigin.y - HALF_BIG_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_BIG_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_BIG_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_BIG_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_BIG_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_BIG_FRAME_X))))) ||\
        ((((p.y == (CursorOrigin.y - HALF_SMALL_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_SMALL_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_SMALL_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_SMALL_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_SMALL_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_SMALL_FRAME_X))))))
           {Pixel.Color &= ~0x00000020; Pixel.Color |= 0x00000010;}
    p.x = p.x + 1;
    if (((((p.y == (CursorOrigin.y - HALF_BIG_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_BIG_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_BIG_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_BIG_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_BIG_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_BIG_FRAME_X))))) ||\
        ((((p.y == (CursorOrigin.y - HALF_SMALL_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_SMALL_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_SMALL_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_SMALL_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_SMALL_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_SMALL_FRAME_X))))))
           {Pixel.Color &= ~0x00000008; Pixel.Color |= 0x00000004;}
    p.x = p.x + 1;
    if (((((p.y == (CursorOrigin.y - HALF_BIG_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_BIG_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_BIG_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_BIG_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_BIG_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_BIG_FRAME_X))))) ||\
        ((((p.y == (CursorOrigin.y - HALF_SMALL_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_SMALL_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_SMALL_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_SMALL_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_SMALL_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_SMALL_FRAME_X))))))
           {Pixel.Color &= ~0x00000002; Pixel.Color |= 0x00000001;}
    p.x = p.x + 1;
    if (((((p.y == (CursorOrigin.y - HALF_BIG_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_BIG_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_BIG_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_BIG_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_BIG_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_BIG_FRAME_X))))) ||\
        ((((p.y == (CursorOrigin.y - HALF_SMALL_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_SMALL_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_SMALL_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_SMALL_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_SMALL_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_SMALL_FRAME_X))))))
           {Pixel.Color &= ~0x00008000; Pixel.Color |= 0x00004000;}
    p.x = p.x + 1;
    if (((((p.y == (CursorOrigin.y - HALF_BIG_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_BIG_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_BIG_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_BIG_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_BIG_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_BIG_FRAME_X))))) ||\
        ((((p.y == (CursorOrigin.y - HALF_SMALL_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_SMALL_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_SMALL_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_SMALL_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_SMALL_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_SMALL_FRAME_X))))))
           {Pixel.Color &= ~0x00002000; Pixel.Color |= 0x00001000;}
    p.x = p.x + 1;
    if (((((p.y == (CursorOrigin.y - HALF_BIG_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_BIG_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_BIG_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_BIG_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_BIG_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_BIG_FRAME_X))))) ||\
        ((((p.y == (CursorOrigin.y - HALF_SMALL_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_SMALL_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_SMALL_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_SMALL_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_SMALL_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_SMALL_FRAME_X))))))
           {Pixel.Color &= ~0x00000800; Pixel.Color |= 0x00000400;}
    p.x = p.x + 1;
    if (((((p.y == (CursorOrigin.y - HALF_BIG_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_BIG_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_BIG_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_BIG_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_BIG_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_BIG_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_BIG_FRAME_X))))) ||\
        ((((p.y == (CursorOrigin.y - HALF_SMALL_FRAME_Y)) || (p.y == (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x >= (CursorOrigin.x - HALF_SMALL_FRAME_X)) && (p.x <= (CursorOrigin.x + HALF_SMALL_FRAME_X)))) ||
         (((p.y >= (CursorOrigin.y - HALF_SMALL_FRAME_Y)) && (p.y <= (CursorOrigin.y + HALF_SMALL_FRAME_Y))) &&\
          ((p.x == (CursorOrigin.x - HALF_SMALL_FRAME_X)) || (p.x == (CursorOrigin.x + HALF_SMALL_FRAME_X))))))
           {Pixel.Color &= ~0x00000200; Pixel.Color |= 0x00000100;}

    //Pixel.Color = 0x0000AAAA;
    //Pixel.Color = 0x00000055;
    return Pixel;
}


void fill_all_memory_with_color(U16 Color)
{
    wait_to_complete_BitBLT_engine();
    set_BitBLT_Destination_Start_Address(0);
    set_BitBLT_Width(1024-1);
    set_BitBLT_Height(1024-1);
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation_with_liniar_destination();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Destination_Start_Address(0x100000);
    set_BitBLT_Width(1024-1);
    set_BitBLT_Height(1024-1);
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation_with_liniar_destination();
}


void draw_the_cross(U32 start_address, U16 Color)
{
    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * wPoz_C_Y + wPoz_C_X - bLg_Brat_c - 1);
    set_BitBLT_Memory_Address_Offset(0);
    set_BitBLT_Width(2 * bLg_Brat_c - 1);
    set_BitBLT_Height(1-1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation_with_liniar_destination();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * (wPoz_C_Y - bLg_Brat_c) + wPoz_C_X - 1);
    set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_X/2);
    set_BitBLT_Width(1-1);
    set_BitBLT_Height(2 * bLg_Brat_c - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation();
}

void draw_the_frame(U32 start_address, U16 Color)
{
    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * bPoz_D_X + bPoz_D_X - 1);
    set_BitBLT_Memory_Address_Offset(0);
    set_BitBLT_Width(DISPLAY_RESOLUTION_X - 2*bPoz_D_X - 1);
    set_BitBLT_Height(1-1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation_with_liniar_destination();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * (DISPLAY_RESOLUTION_Y/2 - bPoz_D_X) + bPoz_D_X - 1);
    set_BitBLT_Memory_Address_Offset(0);
    set_BitBLT_Width(DISPLAY_RESOLUTION_X - 2*bPoz_D_X - 1);
    set_BitBLT_Height(1-1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation_with_liniar_destination();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * DISPLAY_RESOLUTION_Y/2 + bPoz_D_X - 1);
    set_BitBLT_Memory_Address_Offset(0);
    set_BitBLT_Width(DISPLAY_RESOLUTION_X - 2*bPoz_D_X - 1);
    set_BitBLT_Height(1-1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation_with_liniar_destination();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * ( DISPLAY_RESOLUTION_Y - bPoz_D_X) + bPoz_D_X - 1);
    set_BitBLT_Memory_Address_Offset(0);
    set_BitBLT_Width(DISPLAY_RESOLUTION_X - 2*bPoz_D_X - 1);
    set_BitBLT_Height(1-1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation_with_liniar_destination();


    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * bPoz_D_X + bPoz_D_X - 1);
    set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_X/2);
    set_BitBLT_Width(1-1);
    set_BitBLT_Height(DISPLAY_RESOLUTION_Y - 2*bPoz_D_X - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * bPoz_D_X + DISPLAY_RESOLUTION_X/2 - 1);
    set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_X/2);
    set_BitBLT_Width(1-1);
    set_BitBLT_Height(DISPLAY_RESOLUTION_Y - 2*bPoz_D_X - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * (bPoz_D_X + 1) - bPoz_D_X - 1);
    set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_X/2);
    set_BitBLT_Width(1-1);
    set_BitBLT_Height(DISPLAY_RESOLUTION_Y - 2*bPoz_D_X - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation();
}


void draw_the_grid(U32 start_address, U16 Color)
{
    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * wPoz_E_Y + wPoz_E_X - 1);
    set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_X/2);
    set_BitBLT_Width(1-1);
    set_BitBLT_Height(GRID_LONG_Y - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * wPoz_E_Y + wPoz_E_X + GRID_CLEARANCE - 1);
    set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_X/2);
    set_BitBLT_Width(1-1);
    set_BitBLT_Height(GRID_LONG_Y - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * wPoz_E_Y + wPoz_E_X + 2 * GRID_CLEARANCE - 1);
    set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_X/2);
    set_BitBLT_Width(1-1);
    set_BitBLT_Height(GRID_LONG_Y - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation();

    wait_to_complete_BitBLT_engine();
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Destination_Start_Address(start_address + DISPLAY_RESOLUTION_X * wPoz_E_Y + wPoz_E_X + 3 * GRID_CLEARANCE - 1);
    set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_X/2);
    set_BitBLT_Width(1-1);
    set_BitBLT_Height(GRID_LONG_Y - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation();
}



//************************************************************************************************
//************************************************************************************************
//
//     functii pentru ecran rotit la 90 grade
//
//************************************************************************************************
//************************************************************************************************
void copy_LettersCaps_in_memory_90(U32 address)
{
    CDot Pixel;
    U16 i, j;

    for (j = 0; j < CHARACTER_HEIGHT_90 ; j++)
    {
        for (i = 0; i < MATRIX_LETTERS_CAPS_WIDTH_90 ; i++)
        {
            Pixel.Color = 0;
            Pixel.Adresa = address + j*DISPLAY_RESOLUTION_Y_90 + i;
            if (LettersCaps_90MX[MATRIX_LETTERS_CAPS_WIDTH_90*j+i] == '*') Pixel.Color = DISPLAY_FOREGROUND_COLOR;
            else Pixel.Color = DISPLAY_BACKGROUND_COLOR;
            i++;
            if (LettersCaps_90MX[MATRIX_LETTERS_CAPS_WIDTH_90*j+i] == '*') Pixel.Color |= (DISPLAY_FOREGROUND_COLOR << 8);
            else Pixel.Color |= (DISPLAY_BACKGROUND_COLOR << 8);
            write_to_S1D13506(Pixel.Adresa,Pixel.Color);
        }
    }
}

void copy_Letters_in_memory_90(U32 address)
{
    CDot Pixel;
    U16 i, j;

    for (j = 0; j < CHARACTER_HEIGHT_90 ; j++)
    {
        for (i = 0; i < MATRIX_LETTERS_WIDTH_90 ; i++)
        {
            Pixel.Color = 0;
            Pixel.Adresa = address + j*DISPLAY_RESOLUTION_Y_90 + i;
            if (Letters_90MX[MATRIX_LETTERS_WIDTH_90*j+i] == '*') Pixel.Color = DISPLAY_FOREGROUND_COLOR;
            else Pixel.Color = DISPLAY_BACKGROUND_COLOR;
            i++;
            if (Letters_90MX[MATRIX_LETTERS_WIDTH_90*j+i] == '*') Pixel.Color |= (DISPLAY_FOREGROUND_COLOR << 8);
            else Pixel.Color |= (DISPLAY_BACKGROUND_COLOR << 8);
            write_to_S1D13506(Pixel.Adresa,Pixel.Color);
        }
    }
}

void copy_FiguresAndSymbols_in_memory_90(U32 address)
{
    CDot Pixel;
    U16 i, j;

    for (j = 0; j < CHARACTER_HEIGHT_90 ; j++)
    {
        for (i = 0; i < MATRIX_FIGURURES_AND_SYMBOLS_WIDTH_90 ; i++)
        {
            Pixel.Color = 0;
            Pixel.Adresa = address + j*DISPLAY_RESOLUTION_Y_90 + i;
            if (FiguresAndSymbols_90MX[MATRIX_FIGURURES_AND_SYMBOLS_WIDTH_90*j+i] == '*') Pixel.Color = DISPLAY_FOREGROUND_COLOR;
            else Pixel.Color = DISPLAY_BACKGROUND_COLOR;
            i++;
            if (FiguresAndSymbols_90MX[MATRIX_FIGURURES_AND_SYMBOLS_WIDTH_90*j+i] == '*') Pixel.Color |= (DISPLAY_FOREGROUND_COLOR << 8);
            else Pixel.Color |= (DISPLAY_BACKGROUND_COLOR << 8);
            write_to_S1D13506(Pixel.Adresa,Pixel.Color);
        }
    }
}


void DrawText_90(U32 start_panel_address, CPoint p, const char *str)
{
    char c;
    U16 IndexCaracter, Index;
    U32 AdresaSursa, AdresaDestinatie;

    for (IndexCaracter = 0; IndexCaracter < strlen(str); IndexCaracter++)
    {
        c = str[IndexCaracter];
        if ((c >= 0x41) && (c <= 0x5a))
        {
            Index = c - 0x41;
            AdresaSursa = ADDRESS_LETTER_CAPS_STORAGE_90 + Index * CHARACTER_WIDTH_90;
            //AdresaDestinatie = DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;
        }
        else if ((c >= 0x61) && (c <= 0x7a))
        {
            Index = c - 0x61;
            AdresaSursa = ADDRESS_LETTERS_STORAGE_90 + Index * CHARACTER_WIDTH_90;
            //AdresaDestinatie = DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;
        }
        else if ((c >= 0x20) && (c <= 0x3f))
        {
        	Index = c - 0x20;
            AdresaSursa = ADDRESS_FIGURES_AND_SYMBOLS_90 + Index * CHARACTER_WIDTH_90;
            //AdresaDestinatie = DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;
        }

        //AdresaSursa = ADDRESS_LETTER_CAPS_STORAGE_90 + Index * CHARACTER_WIDTH_90;
        AdresaDestinatie = start_panel_address + DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;

        wait_to_complete_BitBLT_engine();
        set_BitBLT_Destination_Start_Address(AdresaDestinatie);
        set_BitBLT_Source_Start_Address(AdresaSursa);
        set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
        set_BitBLT_Width(CHARACTER_WIDTH_90);
        set_BitBLT_Height(CHARACTER_HEIGHT_90);
        set_BitBLT_Background_Color(0);
        set_BitBLT_Foreground_Color(0);
        set_BitBLT_Operation(2, 12);
        start_BitBLT_Operation();
    }
}

void DrawTextVariable_90(U32 panel_address, CPoint p, char *str)
{
    char c;
    U16 IndexCaracter, Index;
    U32 AdresaSursa, AdresaDestinatie;

    for (IndexCaracter = 0; IndexCaracter < strlen(str); IndexCaracter++)
    {
        c = str[IndexCaracter];
        if ((c >= 0x41) && (c <= 0x5a))
        {
            Index = c - 0x41;
            AdresaSursa = ADDRESS_LETTER_CAPS_STORAGE_90 + Index * CHARACTER_WIDTH_90;
            //AdresaDestinatie = DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;
        }
        else if ((c >= 0x61) && (c <= 0x7a))
        {
            Index = c - 0x61;
            AdresaSursa = ADDRESS_LETTERS_STORAGE_90 + Index * CHARACTER_WIDTH_90;
            //AdresaDestinatie = DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;
        }
        else if ((c >= 0x20) && (c <= 0x3f))
        {
            Index = c - 0x20;
            AdresaSursa = ADDRESS_FIGURES_AND_SYMBOLS_90 + Index * CHARACTER_WIDTH_90;
            //AdresaDestinatie = DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;
        }

        //AdresaSursa = ADDRESS_LETTER_CAPS_STORAGE_90 + Index * CHARACTER_WIDTH_90;
        AdresaDestinatie = panel_address + DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;

        wait_to_complete_BitBLT_engine();
        set_BitBLT_Destination_Start_Address(AdresaDestinatie);
        set_BitBLT_Source_Start_Address(AdresaSursa);
        set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
        set_BitBLT_Width(CHARACTER_WIDTH_90);
        set_BitBLT_Height(CHARACTER_HEIGHT_90);
        set_BitBLT_Background_Color(DISPLAY_BACKGROUND_COLOR);
        set_BitBLT_Foreground_Color(DISPLAY_FOREGROUND_COLOR);
        set_BitBLT_Operation(2, 12);
        start_BitBLT_Operation();
    }
}


U8 draw_text_90(U32 panel_address, CPoint p, char * str, int size)
{
    char c;
    U16 Index;
    U32 AdresaSursa, AdresaDestinatie;

    if (IndexCaracter < size)
    {
        if (check_if_complete_BitBLT_engine())
        {
        c = str[IndexCaracter];
        if ((c >= 0x41) && (c <= 0x5a))
        {
            Index = c - 0x41;
            AdresaSursa = ADDRESS_LETTER_CAPS_STORAGE_90 + Index * CHARACTER_WIDTH_90;
            //AdresaDestinatie = DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;
        }
        else if ((c >= 0x61) && (c <= 0x7a))
        {
            Index = c - 0x61;
            AdresaSursa = ADDRESS_LETTERS_STORAGE_90 + Index * CHARACTER_WIDTH_90;
            //AdresaDestinatie = DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;
        }
        else if ((c >= 0x20) && (c <= 0x3f))
        {
            Index = c - 0x20;
            AdresaSursa = ADDRESS_FIGURES_AND_SYMBOLS_90 + Index * CHARACTER_WIDTH_90;
            //AdresaDestinatie = DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;
        }

        //AdresaSursa = ADDRESS_LETTER_CAPS_STORAGE_90 + Index * CHARACTER_WIDTH_90;
        AdresaDestinatie = panel_address + DISPLAY_RESOLUTION_Y_90 * (p.x + 1) - p.y + IndexCaracter * CHARACTER_HEIGHT_90 * DISPLAY_RESOLUTION_Y_90 - CHARACTER_WIDTH_90;

        //wait_to_complete_BitBLT_engine();
        set_BitBLT_Destination_Start_Address(AdresaDestinatie);
        set_BitBLT_Source_Start_Address(AdresaSursa);
        set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
        set_BitBLT_Width(CHARACTER_WIDTH_90);
        set_BitBLT_Height(CHARACTER_HEIGHT_90);
        set_BitBLT_Background_Color(DISPLAY_BACKGROUND_COLOR);
        set_BitBLT_Foreground_Color(DISPLAY_FOREGROUND_COLOR);
        set_BitBLT_Operation(2, 12);
        start_BitBLT_Operation();

        IndexCaracter++;
        }
    } else {IndexCaracter = 0; return TRUE;}
    return FALSE;
}

U8 draw_the_standard_90(U32 panel_address, volatile DRStandard * rstandard, U16 Color, U8 Refresh)
{
    CPoint P0;

        switch (StandardScheduler)
        {
            case STANDARD_STEP1:
                 IndexCaracter = 0;
                 StandardScheduler = STANDARD_STEP2;
                 break;
            // afiseaza DataX
            case STANDARD_STEP2:
                 if ((Refresh & (1 << BIT_REFRESH_OF_FRAME_POSITION)) || (Refresh & (1 << BIT_REFRESH_OF_X)))
                 {
                     P0.x = rstandard->wPozX + rstandard->swOffsetX;
                     P0.y = rstandard->wPozY + rstandard->swOffsetY;
                     if (draw_text_90(panel_address,P0,&sNewRStandard.cDataX[0],WIDTH_X))StandardScheduler = STANDARD_STEP3;
                 }else StandardScheduler = STANDARD_STEP3;
                 break;
            // afiseza DataY
            case STANDARD_STEP3:
                 if ((Refresh & (1 << BIT_REFRESH_OF_FRAME_POSITION)) || (Refresh & (1 << BIT_REFRESH_OF_Y)))
                 {
                     P0.x = DISPLAY_RESOLUTION_X_90 - rstandard->wPozX - CHARACTER_HEIGHT_90 * WIDTH_Y + rstandard->swOffsetX;
                     P0.y = rstandard->wPozY + rstandard->swOffsetY;
                     if (draw_text_90(panel_address,P0,&sNewRStandard.cDataY[0],WIDTH_Y))StandardScheduler = STANDARD_STEP4;
                 }else StandardScheduler = STANDARD_STEP4;
                 break;
            // afiseaza TM
            case STANDARD_STEP4:
                 if ((Refresh & (1 << BIT_REFRESH_OF_FRAME_POSITION)) || (Refresh & (1 << BIT_REFRESH_OF_TM)))
                 {
                    P0.x = rstandard->wPozX + rstandard->swOffsetX;
                    P0.y = DISPLAY_RESOLUTION_Y_90 - rstandard->wPozY - CHARACTER_WIDTH_90 + rstandard->swOffsetY;
                    if (draw_text_90(panel_address,P0,&sNewRStandard.cTm[0],WIDTH_TM))StandardScheduler = STANDARD_STEP5;
                 }else StandardScheduler = STANDARD_STEP5;
                 break;
            // afiseaza ERR
            case STANDARD_STEP5:
                 if(sNewRStandard.bFlags & (1 << BIT_DISPLAY_ERR))//((Refresh & (1 << BIT_REFRESH_OF_FLAG_ERR)) &&
                 {
                     P0.x = DISPLAY_RESOLUTION_X_90 - rstandard->wPozX - CHARACTER_HEIGHT_90 * WIDTH_ERR + rstandard->swOffsetX;
                     P0.y = DISPLAY_RESOLUTION_Y_90 - rstandard->wPozY - CHARACTER_WIDTH_90 + rstandard->swOffsetY;
                     if (draw_text_90(panel_address,P0,&sNewRStandard.cErr[0],WIDTH_ERR))StandardScheduler = STANDARD_STEP6;
                 }
                 // ascunde sectiunea ERR
                 else {if (hide_err_section(panel_address, &sNewRStandard, DISPLAY_BACKGROUND_COLOR))StandardScheduler = STANDARD_STEP6;}
                 break;
            // afiseaza caracter READY
            case STANDARD_STEP6:
                 if(sNewRStandard.bFlags & (1 << BIT_DISPLAY_READY))
                 {if (draw_ready_character(panel_address, &sNewRStandard, DISPLAY_FOREGROUND_COLOR)) return TRUE;}
                 else {if (draw_ready_character(panel_address, &sNewRStandard, DISPLAY_BACKGROUND_COLOR)) return TRUE;}
                 break;
        }
        return FALSE;
}

U8 hide_err_section(U32 panel_address, volatile DRStandard * rstandard, U16 Color)
{
    if (check_if_complete_BitBLT_engine())
    {
         set_BitBLT_Foreground_Color(Color);
         set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90*(DISPLAY_RESOLUTION_X_90 - rstandard->wPozX - WIDTH_ERR * CHARACTER_HEIGHT_90 + rstandard->swOffsetX) + rstandard->wPozY - rstandard->swOffsetY);
         set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
         set_BitBLT_Width(CHARACTER_WIDTH_90 - 1);
         set_BitBLT_Height(WIDTH_ERR*CHARACTER_HEIGHT_90 - 1);
         set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
         start_BitBLT_Operation();
         return TRUE;
    }
    return FALSE;
}

U8 draw_ready_character(U32 pannel_address, volatile DRStandard * rstandard, U16 Color)
{
    if (check_if_complete_BitBLT_engine())
    {
        set_BitBLT_Foreground_Color(Color);
        set_BitBLT_Destination_Start_Address(pannel_address + DISPLAY_RESOLUTION_Y_90*(DISPLAY_RESOLUTION_X_90/2 + rstandard->swOffsetX) - rstandard->wPozY - rstandard->swOffsetY - CHARACTER_WIDTH_90 );
        set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
        set_BitBLT_Width(CHARACTER_WIDTH_90 - 1);
        set_BitBLT_Height(CHARACTER_HEIGHT_90 - 1);
        set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
        start_BitBLT_Operation();
        return TRUE;
    }
    return FALSE;
}

U8 delete_character_of_standard_view(U32 pannel_address, volatile DRStandard * rstandard, U16 Color)
{
    switch(DeleteScheduler)
    {
        case DELETE_STEP1:
             if (check_if_complete_BitBLT_engine())
             {
                 set_BitBLT_Foreground_Color(Color);
                 set_BitBLT_Destination_Start_Address(pannel_address + DISPLAY_RESOLUTION_Y_90 * rstandard->wPozX + rstandard->wPozY);
                 set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                 set_BitBLT_Width(CHARACTER_WIDTH_90 - 1);
                 set_BitBLT_Height(DISPLAY_RESOLUTION_X_90 - 2*rstandard->wPozX - 1);
                 set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                 start_BitBLT_Operation();
                 DeleteScheduler = DELETE_STEP2;
                 //return TRUE;
             }
        case DELETE_STEP2:
            if (check_if_complete_BitBLT_engine())
            {
                 set_BitBLT_Foreground_Color(Color);
                 set_BitBLT_Destination_Start_Address(pannel_address + DISPLAY_RESOLUTION_Y_90*(rstandard->wPozX + 1)- rstandard->wPozY - CHARACTER_WIDTH_90 );
                 set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                 set_BitBLT_Width(CHARACTER_WIDTH_90 - 1);
                 set_BitBLT_Height(DISPLAY_RESOLUTION_X_90 - 2*rstandard->wPozX - 1);
                 set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                 start_BitBLT_Operation();
                 //DeleteScheduler = DELETE_STEP3;
                 return TRUE;
            }
            break;
            /*
        case DELETE_STEP3:
            if (check_if_complete_BitBLT_engine())
            {
                 set_BitBLT_Foreground_Color(Color);
                 set_BitBLT_Destination_Start_Address(pannel_address + DISPLAY_RESOLUTION_Y_90*(DISPLAY_RESOLUTION_X_90 - standard->bPoz_x - WIDTH_Y*CHARACTER_HEIGHT_90 )- standard->bPoz_y - CHARACTER_WIDTH_90 );
                 set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                 set_BitBLT_Width(CHARACTER_WIDTH_90 - 1);
                 set_BitBLT_Height(WIDTH_Y*CHARACTER_HEIGHT_90 - 1);
                 set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                 start_BitBLT_Operation();
                 return TRUE;
            }
            break;
            */
    }
    return FALSE;
}

U8 display_ready(U32 panel_address, volatile DStandard * standard, U16 Color)
{
    if (check_if_complete_BitBLT_engine())
    {
        set_BitBLT_Foreground_Color(Color);
        set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90*(DISPLAY_RESOLUTION_X_90/2) - standard->bPoz_y - CHARACTER_WIDTH_90 );
        set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
        set_BitBLT_Width(CHARACTER_WIDTH_90 - 1);
        set_BitBLT_Height(CHARACTER_HEIGHT_90 - 1);
        set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
        start_BitBLT_Operation();
        return TRUE;
    }
    return TRUE;
}

U8 fill_memory_view_with_color(U32 address, U16 Color)
{
    set_BitBLT_Destination_Start_Address(address);
    set_BitBLT_Width(800-1);
    set_BitBLT_Height(800-1);
    set_BitBLT_Foreground_Color(Color);
    set_BitBLT_Background_Color(Color);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    start_BitBLT_Operation_with_liniar_destination();
    return TRUE;
}

U8 draw_the_RAL_90(U32 panel_address, volatile DRAL * ral, U16 Color)
{
    switch(CrossStatus)
    {
        case CROSS_STEP1:
            if (check_if_complete_BitBLT_engine())
            {
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90 * (ral->wPozX) + ral->wPozY - ral->wBrat - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(2 * ral->wBrat - 1);
                set_BitBLT_Height(CROSS_WIDTH - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                //start_BitBLT_Operation_with_liniar_destination();
                CrossStatus = CROSS_STEP2;
            }
            break;
        case CROSS_STEP2:
            if (check_if_complete_BitBLT_engine())
            {
               //set_BitBLT_Foreground_Color(Color);
               set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90 * (ral->wPozX - ral->wBrat) + ral->wPozY - 1);
               set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
               set_BitBLT_Width(CROSS_WIDTH - 1);
               set_BitBLT_Height(2 * ral->wBrat - 1);
               set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
               start_BitBLT_Operation();
               CrossStatus = CROSS_STEP3;
            }
            break;
        case CROSS_STEP3:
            return TRUE;
        break;
    }
    return FALSE;
}


U8 draw_the_RAE_90(U32 panel_address, volatile DRAE * rae, U16 Color)
{
    switch(RAEStatus)
    {
        case CROSS_STEP1:
            if (check_if_complete_BitBLT_engine())
            {
                // se considera display de monitor de calculator
                // linia orizontala
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90 * (rae->wPozX) + rae->wPozY - (2 * rae->wBratY) / 3 - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(2 * rae->wBratY - 1);
                set_BitBLT_Height(CROSS_WIDTH - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                //start_BitBLT_Operation_with_liniar_destination();
                RAEStatus = CROSS_STEP2;
            }
            break;
        case CROSS_STEP2:
            if (check_if_complete_BitBLT_engine())
            {
               //linia verticala
               //set_BitBLT_Foreground_Color(Color);
               set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90 * (rae->wPozX - rae->wBratX) + rae->wPozY - 1);
               set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
               set_BitBLT_Width(CROSS_WIDTH - 1);
               set_BitBLT_Height(2 * rae->wBratX - 1);
               set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
               start_BitBLT_Operation();
               RAEStatus = CROSS_STEP3;
            }
            break;
        case CROSS_STEP3:
            return TRUE;
        break;
    }
    return FALSE;
}

U8 draw_the_frame_90(U32 panel_address, DRAM * ram, U16 Color)
{
    switch(FrameScheduler)
    {
        case FRAME_STEP1:
            if (check_if_complete_BitBLT_engine())
            {
                // se va considera display-ul de monitor de calculator neintors
                // primul segment vertical de la stanga
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + (DISPLAY_RESOLUTION_Y_90 * (DISPLAY_RESOLUTION_X_90 - ram->wLgX))/2 + (DISPLAY_RESOLUTION_Y_90 - 2 * ram->wLgX)/2 + (DISPLAY_RESOLUTION_Y_90 * ram->swOffsetX) - ram->swOffsetY - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(FRAME_WIDTH - 1);
                set_BitBLT_Height(ram->wLgX - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                FrameScheduler = FRAME_STEP2;
            }
            break;
        case FRAME_STEP2:
            if (check_if_complete_BitBLT_engine())
            {
                // al doilea segment vertical de la stanga
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + (DISPLAY_RESOLUTION_Y_90 * (DISPLAY_RESOLUTION_X_90 - ram->wLgX))/2 + (DISPLAY_RESOLUTION_Y_90 - ram->wLgX)/2 + (DISPLAY_RESOLUTION_Y_90 * ram->swOffsetX) - ram->swOffsetY - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(FRAME_WIDTH - 1);
                set_BitBLT_Height(ram->wLgX - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                FrameScheduler = FRAME_STEP3;
            }
            break;
        case FRAME_STEP3:
            if (check_if_complete_BitBLT_engine())
            {
                //al treilea segment vertical de la stanga
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + (DISPLAY_RESOLUTION_Y_90 * (DISPLAY_RESOLUTION_X_90 - ram->wLgX))/2 + DISPLAY_RESOLUTION_Y_90/2 + (DISPLAY_RESOLUTION_Y_90 * ram->swOffsetX) - ram->swOffsetY - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(FRAME_WIDTH - 1);
                set_BitBLT_Height(ram->wLgX - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                FrameScheduler = FRAME_STEP4;
            }
            break;
        case FRAME_STEP4:
            if (check_if_complete_BitBLT_engine())
            {
                // al patrulea segment vertical de la stanga
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + (DISPLAY_RESOLUTION_Y_90 * (DISPLAY_RESOLUTION_X_90 - ram->wLgX))/2 + (DISPLAY_RESOLUTION_Y_90 + 2 * ram->wLgX)/2 + (DISPLAY_RESOLUTION_Y_90 * ram->swOffsetX) - ram->swOffsetY - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(FRAME_WIDTH - 1);
                set_BitBLT_Height(ram->wLgX - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                FrameScheduler = FRAME_STEP5;
            }
            break;
        case FRAME_STEP5:
            if (check_if_complete_BitBLT_engine())
            {
                // primul segment orizontal incepand de sus
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + (DISPLAY_RESOLUTION_Y_90 * (DISPLAY_RESOLUTION_X_90 - ram->wLgX))/2 + (DISPLAY_RESOLUTION_Y_90 - 2 * ram->wLgX)/2 + (DISPLAY_RESOLUTION_Y_90 * ram->swOffsetX) - ram->swOffsetY - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(2 * ram->wLgX - 1);
                set_BitBLT_Height(FRAME_WIDTH - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                //start_BitBLT_Operation_with_liniar_destination();
                start_BitBLT_Operation();
                FrameScheduler = FRAME_STEP6;
            }
            break;
        case FRAME_STEP6:
            if (check_if_complete_BitBLT_engine())
            {
                // al doilea segment orizontal incepand de sus
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + (DISPLAY_RESOLUTION_Y_90 * DISPLAY_RESOLUTION_X_90)/2 + (DISPLAY_RESOLUTION_Y_90 - 2 * ram->wLgX)/2 + (DISPLAY_RESOLUTION_Y_90 * ram->swOffsetX) - ram->swOffsetY - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(2 * ram->wLgX - 1);
                set_BitBLT_Height(FRAME_WIDTH - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                //start_BitBLT_Operation_with_liniar_destination();
                start_BitBLT_Operation();
                FrameScheduler = FRAME_STEP7;
            }
            break;
        case FRAME_STEP7:
            if (check_if_complete_BitBLT_engine())
            {
               // al treilea segment orizontal incepand de sus
               set_BitBLT_Foreground_Color(Color);
               set_BitBLT_Destination_Start_Address(panel_address + (DISPLAY_RESOLUTION_Y_90 * (DISPLAY_RESOLUTION_X_90 + ram->wLgX))/2 + (DISPLAY_RESOLUTION_Y_90 - 2 * ram->wLgX)/2 + (DISPLAY_RESOLUTION_Y_90 * ram->swOffsetX) - ram->swOffsetY - 1);
               set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
               set_BitBLT_Width(2 * ram->wLgX + FRAME_WIDTH - 1);
               set_BitBLT_Height(FRAME_WIDTH - 1);
               set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
               //start_BitBLT_Operation_with_liniar_destination();
               start_BitBLT_Operation();
               FrameScheduler = FRAME_STEP8;
            }
            break;
        case FRAME_STEP8:
        	//show_data_by_LED((((DISPLAY_RESOLUTION_Y_90 * (DISPLAY_RESOLUTION_X_90 - ram->wLgX))/2)>>8));
            return TRUE;
            break;
    }
    return FALSE;
}


U8 draw_the_grid_90(U32 panel_address, DRGrid * grid, U16 Color)
{
    switch(GridScheduler)
    {
        case GRID_STEP1:
            if (check_if_complete_BitBLT_engine())
            {
                //wait_to_complete_BitBLT_engine();
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90 * (grid->wPozX + 1)- GRID_LONG_Y - GRID_POSITION_Y - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(GRID_LONG_Y -1);
                set_BitBLT_Height(2 - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                GridScheduler = GRID_STEP2;
            }
            break;
        case GRID_STEP2:
            if (check_if_complete_BitBLT_engine())
            {
                //wait_to_complete_BitBLT_engine();
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90 * (grid->wPozX + grid->wClearance + 1) - GRID_LONG_Y - GRID_POSITION_Y - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(GRID_LONG_Y -1);
                set_BitBLT_Height(2 - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                GridScheduler = GRID_STEP3;
            }
            break;
        case GRID_STEP3:
            if (check_if_complete_BitBLT_engine())
            {
                //wait_to_complete_BitBLT_engine();
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90 * (grid->wPozX + 2 * grid->wClearance + 1) - GRID_LONG_Y - GRID_POSITION_Y - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(GRID_LONG_Y -1);
                set_BitBLT_Height(2 - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                GridScheduler = GRID_STEP4;
            }
            break;
        case GRID_STEP4:
            if (check_if_complete_BitBLT_engine())
            {
                //wait_to_complete_BitBLT_engine();
                set_BitBLT_Foreground_Color(Color);
                set_BitBLT_Destination_Start_Address(panel_address + DISPLAY_RESOLUTION_Y_90 * (grid->wPozX + 3 * grid->wClearance + 1) - GRID_LONG_Y - GRID_POSITION_Y - 1);
                set_BitBLT_Memory_Address_Offset(DISPLAY_RESOLUTION_Y_90/2);
                set_BitBLT_Width(GRID_LONG_Y -1);
                set_BitBLT_Height(2 - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                start_BitBLT_Operation();
                GridScheduler = GRID_STEP5;
            }
            break;
        case GRID_STEP5:
            return TRUE;
            break;
    }
    return FALSE;
}

void draw_cursor_point(void)
{
    wait_to_complete_BitBLT_engine();
    //segment stanga - ecran intors spre stanga cu 90
    set_BitBLT_Foreground_Color(0xa5);
    set_BitBLT_Destination_Start_Address(MEMORY_ADDRESS_OF_CURSOR_1 + 16*29+7);
    set_BitBLT_Memory_Address_Offset(8);
    set_BitBLT_Width(1 - 1);
    set_BitBLT_Height(4 - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    //start_BitBLT_Operation_with_liniar_destination();
    start_BitBLT_Operation();

    wait_to_complete_BitBLT_engine();
    //segment stanga - ecran intors spre stanga cu 90
    set_BitBLT_Foreground_Color(0x5a);
    set_BitBLT_Destination_Start_Address(MEMORY_ADDRESS_OF_CURSOR_1 + 16*29+8);
    set_BitBLT_Memory_Address_Offset(8);
    set_BitBLT_Width(1 - 1);
    set_BitBLT_Height(4 - 1);
    set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
    //start_BitBLT_Operation_with_liniar_destination();
    start_BitBLT_Operation();
}


U8 draw_frame_cursor(U32 cursor_address, volatile DRStandard * rstandard)
{
    U8 Cat, Rest;
    U8 Color1, Color2, Color3, Color4;
    U8 Lg_Seg;
    U32 Adresa1, Adresa2, Adresa3, Adresa4, Adresa5;

    Rest = (32 - rstandard->bLgRoY/2)%4;
    Cat = ((32 - rstandard->bLgRoY/2)/4);

    Adresa1 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + Cat;
    Adresa2 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + 15 - Cat;

    if (Rest == 0)
    {
        Color1 = 0x6a; Color2 = 0xa9;
        Color3 = 0x55; Color4 = 0x55;
        Adresa3 = cursor_address + 16*(32 - rstandard->bLgRoX /2 - 1) + Cat;
        Adresa4 = cursor_address + 16*(32 - rstandard->bLgRoX /2 - 1) + Cat + 1;
        Adresa5 = cursor_address + 16*(32 - rstandard->bLgRoX /2 - 1) + rstandard->bLgRoY / 4 + Cat;
        Lg_Seg = (rstandard->bLgRoY)/4;
       //show_data_by_LED(1);
    }
    else if (Rest == 1)
    {
        Color1 = 0x9a; Color2 = 0xa6;
        Color3 = 0x95; Color4 = 0x56;
        Adresa3 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + Cat + 1;
        Adresa4 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + Cat;
        Adresa5 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + rstandard->bLgRoY / 4 + Cat;
        Lg_Seg = (rstandard->bLgRoY)/4 - 1;
        //show_data_by_LED(2);
    }
    else if (Rest == 2)
    {
        Color1 = 0xa6; Color2 = 0x9a;
        Color3 = 0xa5; Color4 = 0x5a;
        Adresa3 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + Cat + 1;
        Adresa4 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + Cat;
        Adresa5 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + rstandard->bLgRoY / 4 + Cat;
        Lg_Seg = (rstandard->bLgRoY)/4 - 1;
        //show_data_by_LED(3);
    }
    else if (Rest == 3)
    {
        Color1 = 0xa9; Color2 = 0x6a;
        Color3 = 0xa9; Color4 = 0x6a;
        Adresa3 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + Cat + 1;
        Adresa4 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + Cat + 1;
        Adresa5 = cursor_address + 16*(32 - rstandard->bLgRoX / 2 - 1) + rstandard->bLgRoY / 4 + Cat;
        Lg_Seg = (rstandard->bLgRoY)/4;
        //show_data_by_LED(4);
    }


    //Adresa3 = cursor_address + 16*(32 - standard->bLg_ro_x/2 - 1) + Cat + 1;
    //Adresa4 = cursor_address + 16*(32 - standard->bLg_ro_x/2 - 1) + Cat + 1;
    //Adresa5 = cursor_address + 16*(32 - standard->bLg_ro_x/2 - 1) + standard->bLg_ro_y/4 + Cat;

    //show_data_by_LED(temp1);

    switch(CursorScheduler)
    {
        case CURSOR_STEP1:
            if (check_if_complete_BitBLT_engine())
            {
                //wait_to_complete_BitBLT_engine();
                //segment stanga - ecran intors spre stanga cu 90
                set_BitBLT_Foreground_Color(0xa9);
                set_BitBLT_Destination_Start_Address(cursor_address + 16*30+7);
                set_BitBLT_Memory_Address_Offset(8);
                set_BitBLT_Width(1 - 1);
                set_BitBLT_Height(2 - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                //start_BitBLT_Operation_with_liniar_destination();
                start_BitBLT_Operation();
                CursorScheduler = CURSOR_STEP2;
            }
            break;
        case CURSOR_STEP2:
            if (check_if_complete_BitBLT_engine())
            {
                //wait_to_complete_BitBLT_engine();
                //segment stanga - ecran intors spre stanga cu 90
                set_BitBLT_Foreground_Color(0x6a);
                set_BitBLT_Destination_Start_Address(cursor_address + 16*30+8);
                set_BitBLT_Memory_Address_Offset(8);
                set_BitBLT_Width(1 - 1);
                set_BitBLT_Height(2 - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                //start_BitBLT_Operation_with_liniar_destination();
                start_BitBLT_Operation();
                CursorScheduler = CURSOR_STEP3;
            }
            break;
        case CURSOR_STEP3:
            if (check_if_complete_BitBLT_engine())
            {
                //segment stanga - ecran intors spre stanga cu 90
                set_BitBLT_Foreground_Color(Color1);
                //set_BitBLT_Background_Color(0x22);
                set_BitBLT_Destination_Start_Address(Adresa1);
                set_BitBLT_Memory_Address_Offset(8);
                set_BitBLT_Width(1 - 1);
                set_BitBLT_Height(rstandard->bLgRoX - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                //start_BitBLT_Operation_with_liniar_destination();
                start_BitBLT_Operation();
                CursorScheduler = CURSOR_STEP4;
            }
            break;
        case CURSOR_STEP4:
            if (check_if_complete_BitBLT_engine())
            {
                //segment stanga - ecran intors spre stanga cu 90
                set_BitBLT_Foreground_Color(Color2);
                set_BitBLT_Destination_Start_Address(Adresa2);
                set_BitBLT_Memory_Address_Offset(8);
                set_BitBLT_Width(1 - 1);
                set_BitBLT_Height(rstandard->bLgRoX - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                //start_BitBLT_Operation_with_liniar_destination();
                start_BitBLT_Operation();
                CursorScheduler = CURSOR_STEP5;
                //return TRUE;
            }
            break;
        case CURSOR_STEP5:
            if (check_if_complete_BitBLT_engine())
            {
                //segment stanga - ecran intors spre stanga cu 90
                set_BitBLT_Foreground_Color(0x5555);
                set_BitBLT_Destination_Start_Address(Adresa3);
                set_BitBLT_Memory_Address_Offset(8*(rstandard->bLgRoX - 1));
                set_BitBLT_Width(Lg_Seg - 1);
                set_BitBLT_Height(2 - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                //start_BitBLT_Operation_with_liniar_destination();
                start_BitBLT_Operation();
                if ((Rest == 0) || (Rest == 3))return TRUE;
                CursorScheduler = CURSOR_STEP6;
                //return TRUE;
            }
            break;
        case CURSOR_STEP6:
            if (check_if_complete_BitBLT_engine())
            {
                //segment stanga - ecran intors spre stanga cu 90
                set_BitBLT_Foreground_Color(Color3);
                set_BitBLT_Destination_Start_Address(Adresa4);
                set_BitBLT_Memory_Address_Offset(8*(rstandard->bLgRoX - 1));
                set_BitBLT_Width(1 - 1);
                set_BitBLT_Height(2 - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                //start_BitBLT_Operation_with_liniar_destination();
                start_BitBLT_Operation();
                CursorScheduler = CURSOR_STEP7;
                //return TRUE;
            }
            break;
        case CURSOR_STEP7:
            if (check_if_complete_BitBLT_engine())
            {
                //segment stanga - ecran intors spre stanga cu 90
                set_BitBLT_Foreground_Color(Color4);
                set_BitBLT_Destination_Start_Address(Adresa5);
                set_BitBLT_Memory_Address_Offset(8*(rstandard->bLgRoX - 1));
                set_BitBLT_Width(1 - 1);
                set_BitBLT_Height(2 - 1);
                set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
                //start_BitBLT_Operation_with_liniar_destination();
                start_BitBLT_Operation();
                return TRUE;
            }
            break;
    }
    return FALSE;
}

U8 clear_cursor_memory(U32 memory_cursor)
{
    if (check_if_complete_BitBLT_engine())
    {
        //segment stanga - ecran intors spre stanga cu 90
        set_BitBLT_Foreground_Color(0xaaaa);
        set_BitBLT_Destination_Start_Address(memory_cursor);
        set_BitBLT_Memory_Address_Offset(8);
        set_BitBLT_Width(1024 - 1);
        set_BitBLT_Height(1 - 1);
        set_BitBLT_Operation(BITBLT_OPERATION_SOLID_FILL, 12);
        start_BitBLT_Operation_with_liniar_destination();
        //start_BitBLT_Operation();
        return TRUE;
    }
    return FALSE;
}
