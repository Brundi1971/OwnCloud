
#ifndef FLASH_XVGA_H_
#define FLASH_XVGA_H_

typedef union
{
    U64 U64_data;;
    U8 U8_data[8];
    struct
    {
        char flags[2];
        U16 wLgX;
        U16 swOffsetX;
        U16 swOffsetY;
    } tData;
}stored_RAM_u;

typedef union
{
    U64 U64_data;;
    U8 U8_data[8];
    struct
    {
        char flags[2];
        U16 wOriginX;
        U16 wOriginY;
        U16 dummy;
    } tData;
}stored_RAL_u;

typedef union
{
    U64 U64_data;;
    U8 U8_data[8];
    struct
    {
        char flags[2];
        U16 wOriginX;
        U16 wOriginY;
        U16 dummy;
    } tData;
}stored_RAE_u;

U64 read_RAM_from_flash(void);
U64 read_RAE_from_flash(void);
U64 read_RAL_from_flash(void);
void write_RAM_to_flash(U64 data);
void write_RAE_to_flash(U64 data);
void write_RAL_to_flash(U64 data);
void read_stored_values_of_RAM(void);
void read_stored_values_of_RAE(void);
void read_stored_values_of_RAL(void);
void store_values_position_of_refernce(void);

#endif
