/**
 * @brief CGA tile tool
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE
 * @verbose Tool functions
 */

#include <fujinet-fuji.h>
#include <fujinet-network.h>
#include <stdbool.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <i86.h>
#include <dos.h>
#include "ascii.h"
#include "tool.h"
#include "plot_tile.h"
#include "print.h"
#include "charset.h"

#define TILE_SIZE 8
#define AK_CREATOR_ID 0x1024
#define AK_APP_ID 0x0001
#define AK_KEY_URL 0x00

/**
 * @brief Cursor position
 */
unsigned char cx = 0, cy = 0;

/**
 * @brief Tool running?
 */
bool tool_running = true;

/**
 * @brief current tile index
 */
unsigned int current_tile_index;

/**
 * @brief pointer to current tile
 */
unsigned char *current_tile = NULL;

/**
 * @brief Current color palette (black, cyan, magenta, white)
 */
unsigned char palette[4] = {0,2,4,6};

/**
 * @brief Export URL (64 chars)
 */
unsigned char export_url[256] = "N:TCP://TMA-3:6502/";

/**
 * @brief Set CGA palette to 0-4
 * @param p Palette # (0-4)
 */
void tool_set_cga_palette(unsigned char p)
{
    union REGS r;

    r.h.ah = 0x0B; // Set palette
    r.h.bh = 0x01; // Set main palette
    r.h.bl = p;

    // now update palette array
    if (p)
    {
        palette[1]=3;
        palette[2]=5;
        palette[3]=7;
    }
    else
    {
        palette[1]=2;
        palette[2]=4;
        palette[3]=6;
    }

    int86(0x10,&r,&r); // do it.
}

/**
 * @brief Output banner to bottom of screen
 */
void tool_banner(void)
{
    print(8,23,3,"TILE TOOL VERSION 0.1");
}

/**
 * @brief Initialize tool
 */
void tool_init(void)
{
    tool_set_cga_palette(1);

    current_tile_index = 'A';
    current_tile = &charset['A'];

    charset_cursor(current_tile_index);
    tool_banner();

    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief Get color of tile pixel at x,y
 * @param tile pointer to tile buffer
 * @param x horizontal offset (0-7)
 * @param y vertical offset (0-7)
 * @return color value (0-3)
 */
unsigned char tool_get_pixel_from_tile(unsigned char *tile, unsigned char x, unsigned char y)
{
    unsigned short w = tile[y << 1] << 8 | tile[(y<<1) + 1];
    unsigned char s = (7-x) * 2;

    return (w >> s) & 0x03;
}

/**
 * @brief Set color of tile pixel at x,y to c
 * @param tile pointer to tile buffer
 * @param x horizontal offset (0-7)
 * @param y vertical offset (0-7)
 * @param c Color to set (0-3)
 */
void tool_set_pixel_to_tile(unsigned char *tile, unsigned char x, unsigned char y, unsigned char c)
{
    unsigned char *o = &tile[y<<1];
    unsigned char s = (7 - x & 3) * 2;
    unsigned char mask = (unsigned char)0x03 << s;

    if (x>3)
        o++;

    c &= 0x03;

    *o = (*o & ~mask) | ((unsigned char)c << s);
}

/**
 * @brief Draw a tool pixel at x,y in color c
 * @param x Horizontal position (0-39)
 * @param y Vertical position (0-24)
 * @param c Color of tool pixel (0-3)
 */
void tool_draw_pixel(unsigned char x, unsigned char y, unsigned char c)
{
    if (cx == x && cy == y)
        plot_tile(color_cursor[c],x,y,3);
    else
        plot_tile(color_chips[c],x,y,3);
}

/**
 * @brief Draw tool view
 */
void tool_draw(unsigned char *tile)
{
    unsigned char x=0, y=0;

    // Update drawing area
    for (y=0;y<8;y++)
    {
        char displayed_value_left[3] = {0,0,0};
        char displayed_value_right[3] = {0,0,0};

        for (x=0;x<8;x++)
            tool_draw_pixel(x,y,tool_get_pixel_from_tile(tile,x,y));

        // Update hex values
        sprintf(displayed_value_left,"%02X",tile[(y<<1)]);
        sprintf(displayed_value_right,"%02X",tile[(y<<1)+1]);

        print(9,y,2,displayed_value_left);
        print(12,y,2,displayed_value_right);
    }

    toolState = GET_COMMAND;
}

/**
 * @brief Move cursor up
 */
void tool_cursor_up(void)
{
    if (cy>0)
        cy--;

    toolState = DRAW;
}

/**
 * @brief Move cursor Down
 */
void tool_cursor_down(void)
{
    if (cy<7)
        cy++;

    toolState = DRAW;
}

/**
 * @brief Move cursor Down
 */
void tool_cursor_left(void)
{
    if (cx>0)
        cx--;

    toolState = DRAW;
}

/**
 * @brief Move cursor Down
 */
void tool_cursor_right(void)
{
    if (cx<7)
        cx++;

    toolState = DRAW;
}

/**
 * @brief set color 0 at current cursor position
 */
void tool_set_color_0(void)
{
    tool_set_pixel_to_tile(current_tile,cx,cy,0);
    toolState = DRAW;
}

/**
 * @brief set color 1 at current cursor position
 */
void tool_set_color_1(void)
{
    tool_set_pixel_to_tile(current_tile,cx,cy,1);
    toolState = DRAW;
}

/**
 * @brief set color 2 at current cursor position
 */
void tool_set_color_2(void)
{
    tool_set_pixel_to_tile(current_tile,cx,cy,2);
    toolState = DRAW;
}

/**
 * @brief set color 3 at current cursor position
 */
void tool_set_color_3(void)
{
    tool_set_pixel_to_tile(current_tile,cx,cy,3);
    toolState = DRAW;
}

/**
 * @brief invert tool area
 */
void tool_invert(void)
{
    unsigned char x=0, y=0;

    for (y=0;y<8;y++)
        for (x=0;x<8;x++)
            tool_set_pixel_to_tile(current_tile,x,y,
                                   tool_get_pixel_from_tile(current_tile,x,y) ^ 0xFF & 0x03);

    toolState = DRAW;
}

/**
 * @brief fill area with color 0
 */
void tool_fill_color_0(void)
{
    unsigned char x=0, y=0;

    for (y=0;y<8;y++)
        for (x=0;x<8;x++)
            tool_set_pixel_to_tile(current_tile,x,y,0);

    toolState = DRAW;
}

/**
 * @brief fill area with color 1
 */
void tool_fill_color_1(void)
{
    unsigned char x=0, y=0;

    for (y=0;y<8;y++)
        for (x=0;x<8;x++)
            tool_set_pixel_to_tile(current_tile,x,y,1);

    toolState = DRAW;
}

/**
 * @brief fill area with color 2
 */
void tool_fill_color_2(void)
{
    unsigned char x=0, y=0;

    for (y=0;y<8;y++)
        for (x=0;x<8;x++)
            tool_set_pixel_to_tile(current_tile,x,y,2);

    toolState = DRAW;
}

/**
 * @brief fill area with color 3
 */
void tool_fill_color_3(void)
{
    unsigned char x=0, y=0;

    for (y=0;y<8;y++)
        for (x=0;x<8;x++)
            tool_set_pixel_to_tile(current_tile,x,y,3);

    toolState = DRAW;
}

/**
 * @brief Rotate left
 */
void tool_rol(void)
{
    unsigned short *wp = (unsigned short *)current_tile;
    unsigned char iy=0;

    for (iy=0;iy<8;iy++)
    {
        unsigned short v = *wp;
        *wp = (v << 2) | (v >> 14);
        wp++;
    }

    toolState = DRAW;
}

/**
 * @brief Rotate Right
 */
void tool_ror(void)
{
    unsigned short *wp = (unsigned short *)current_tile;
    unsigned char iy=0;

    for (iy=0;iy<8;iy++)
    {
        unsigned short v = *wp;
        *wp = (v >> 2) | (v << 14);
        wp++;
    }

    toolState = DRAW;
}

/**
 * @brief rotate up
 */
void tool_rou(void)
{
    unsigned char shifted_tile[16];

    shifted_tile[0] = current_tile[2];
    shifted_tile[1] = current_tile[3];
    shifted_tile[2] = current_tile[4];
    shifted_tile[3] = current_tile[5];
    shifted_tile[4] = current_tile[6];
    shifted_tile[5] = current_tile[7];
    shifted_tile[6] = current_tile[8];
    shifted_tile[7] = current_tile[9];
    shifted_tile[8] = current_tile[10];
    shifted_tile[9] = current_tile[11];
    shifted_tile[10] = current_tile[12];
    shifted_tile[11] = current_tile[13];
    shifted_tile[12] = current_tile[14];
    shifted_tile[13] = current_tile[15];
    shifted_tile[14] = current_tile[0];
    shifted_tile[15] = current_tile[1];

    memcpy(current_tile,shifted_tile,sizeof(shifted_tile));

    toolState = DRAW;
}

/**
 * @brief rotate down
 */
void tool_rod(void)
{
    unsigned char shifted_tile[16];

    shifted_tile[0] = current_tile[14];
    shifted_tile[1] = current_tile[15];
    shifted_tile[2] = current_tile[0];
    shifted_tile[3] = current_tile[1];
    shifted_tile[4] = current_tile[2];
    shifted_tile[5] = current_tile[3];
    shifted_tile[6] = current_tile[4];
    shifted_tile[7] = current_tile[5];
    shifted_tile[8] = current_tile[6];
    shifted_tile[9] = current_tile[7];
    shifted_tile[10] = current_tile[8];
    shifted_tile[11] = current_tile[9];
    shifted_tile[12] = current_tile[10];
    shifted_tile[13] = current_tile[11];
    shifted_tile[14] = current_tile[12];
    shifted_tile[15] = current_tile[13];

    memcpy(current_tile,shifted_tile,sizeof(shifted_tile));

    toolState = DRAW;
}

/**
 * @brief Rotate counter-clockwise
 */
void tool_rotate_ccw(void)
{
    unsigned char tile_rotated[16];
    unsigned char r=0, c=0;

    for (r=0;r<8;r++)
    {
        for (c=0;c<8;c++)
        {
            tool_set_pixel_to_tile(tile_rotated, 7-c, r, tool_get_pixel_from_tile(current_tile, r, c));
        }
    }

    memcpy(current_tile,tile_rotated,sizeof(tile_rotated));

    toolState = DRAW;
}

/**
 * @brief Rotate clockwise
 */
void tool_rotate_cw(void)
{
    unsigned char tile_rotated[16];
    unsigned char r=0, c=0;

    for (r=0;r<8;r++)
    {
        for (c=0;c<8;c++)
        {
            tool_set_pixel_to_tile(tile_rotated, c, 7-r, tool_get_pixel_from_tile(current_tile, r, c));
        }
    }

    memcpy(current_tile,tile_rotated,sizeof(tile_rotated));

    toolState = DRAW;
}

/**
 * @brief go to next character in charset
 */
void tool_next_char(void)
{
    if (current_tile_index < 256)
    {
        charset_cursor(current_tile_index);
        current_tile_index++;
        current_tile = &charset[current_tile_index];
        charset_cursor(current_tile_index);
    }

    toolState = DRAW;
}

/**
 * @brief go to previous character in charset
 */
void tool_previous_char(void)
{
    if (current_tile_index > 0)
    {
        charset_cursor(current_tile_index);
        current_tile_index--;
        current_tile = &charset[current_tile_index];
        charset_cursor(current_tile_index);
    }

    toolState = DRAW;
}

/**
 * @brief remap palette value (or set bkg if color 0)
 * @param i Palette index (0-3)
 * @param c new palette color (0-15)
 */
void tool_set_palette(unsigned char i, unsigned char c)
{
    union REGS r;

    if (i==0)
    {
        // Change background color
        r.h.ah = 0x0B; // Set color/palette
        r.h.bh = 0x00; // Set background color
        r.h.bl = c;    // to color c
    }
    else
    {
        // Remap palette to c at index i
        r.h.ah = 0x10; // Remap palette
        r.h.al = 0x00; // subfunction 0
        r.h.bl = i;    // remap index i
        r.h.bh = c;    // to color c
    }

    int86(0x10,&r,&r); // do it.
}

/**
 * @brief increment palette 0 value
 */
void tool_set_palette_0_next(void)
{
    palette[0]++;
    palette[0] &= 0x0f;

    tool_set_palette(0,palette[0]);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief decrement palette 0 value
 */
void tool_set_palette_0_prev(void)
{
    palette[0]--;
    palette[0] &= 0x0f;

    tool_set_palette(0,palette[0]);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief increment palette 1 value
 */
void tool_set_palette_1_next(void)
{
    palette[1]++;
    palette[1] &= 0x0f;

    tool_set_palette(1,palette[1]);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief decrement palette 1 value
 */
void tool_set_palette_1_prev(void)
{
    palette[1]--;
    palette[1] &= 0x0f;

    tool_set_palette(1,palette[1]);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief increment palette 2 value
 */
void tool_set_palette_2_next(void)
{
    palette[2]++;
    palette[2] &= 0x0f;

    tool_set_palette(2,palette[2]);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief decrement palette 2 value
 */
void tool_set_palette_2_prev(void)
{
    palette[2]--;
    palette[2] &= 0x0f;

    tool_set_palette(2,palette[2]);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief increment palette 3 value
 */
void tool_set_palette_3_next(void)
{
    palette[3]++;
    palette[3] &= 0x0f;

    tool_set_palette(3,palette[3]);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief decrement palette 3 value
 */
void tool_set_palette_3_prev(void)
{
    palette[3]--;
    palette[3] &= 0x0f;

    tool_set_palette(3,palette[3]);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief set CGA palette 0
 */
void tool_set_cga_palette_0(void)
{
    tool_set_cga_palette(0);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief set CGA palette 1
 */
void tool_set_cga_palette_1(void)
{
    tool_set_cga_palette(1);
    toolState = UPDATE_PALETTE_LEGENDS;
}

/**
 * @brief Update Palette Legends
 */
void tool_update_palette_legends(void)
{
    unsigned char i=0;

    for (i=0;i<sizeof(palette);i++)
    {
        char val[3] = {0,0,0};

        sprintf(val,"%02X",palette[i]);
        print(i*8+8,20,2,val);
    }

    toolState = DRAW;
}

/**
 * @brief Import/export setup
 */
void tool_import_export_setup(int argc, char *argv[])
{
    unsigned short l=0;

    fuji_set_appkey_details(AK_CREATOR_ID,AK_APP_ID,DEFAULT);

    if (argc<2)
    {
        fuji_read_appkey(AK_KEY_URL,&l,export_url);
    }
    else
    {
        strcpy(export_url,argv[1]);
    }
}

/**
 * @brief Export
 */
void tool_export(void)
{
    if (!strlen(export_url))
    {
        print(0,23,1,"        NO EXPORT URL.  ABORTING        ");
        toolState = DRAW;
        return;
    }

    print(0,23,1,"     CONNECTING TO EXPORT URL...        ");

    if (network_open(export_url,OPEN_MODE_RW,OPEN_TRANS_NONE) != FN_ERR_OK)
    {
        print(0,23,1,"     COULD NOT OPEN EXPORT URL.         ");
        toolState = DRAW;
        return;
    }

    print(0,23,1,"     EXPORTING TO EXPORT URL...         ");

    network_write(export_url,(const unsigned char *)charset,sizeof(charset));

    print(0,23,1,"              EXPORTED                  ");

    network_close(export_url);

    toolState = DRAW;
}

/**
 * @brief Import
 */
void tool_import(void)
{
    if (!strlen(export_url))
    {
        print(0,23,1,"        NO EXPORT URL.  ABORTING        ");
        toolState = DRAW;
        return;
    }
}

/**
 * @brief Process tool keyboard command
 */
void tool_command(void)
{
    switch (getch())
    {
    case 'w':
        toolState = CURSOR_UP;
        break;
    case 'a':
        toolState = CURSOR_LEFT;
        break;
    case 's':
        toolState = CURSOR_DOWN;
        break;
    case 'd':
        toolState = CURSOR_RIGHT;
        break;
    case '0':
        toolState = SET_COLOR_0;
        break;
    case '1':
        toolState = SET_COLOR_1;
        break;
    case '2':
        toolState = SET_COLOR_2;
        break;
    case '3':
        toolState = SET_COLOR_3;
        break;
    case ')':
        toolState = FILL_COLOR_0;
        break;
    case '!':
        toolState = FILL_COLOR_1;
        break;
    case '@':
        toolState = FILL_COLOR_2;
        break;
    case '#':
        toolState = FILL_COLOR_3;
        break;
    case 'I':
    case 'i':
        toolState = INVERT;
        break;
    case 'A':
        toolState = ROL;
        break;
    case 'D':
        toolState = ROR;
        break;
    case 'W':
        toolState = ROU;
        break;
    case 'S':
        toolState = ROD;
        break;
    case 'q':
        toolState = ROTATE_CCW;
        break;
    case 'e':
        toolState = ROTATE_CW;
        break;
    case '-':
        toolState = PREVIOUS_CHAR;
        break;
    case '=':
        toolState = NEXT_CHAR;
        break;
    case 'z':
        toolState = SET_PALETTE_0_PREV;
        break;
    case 'x':
        toolState = SET_PALETTE_0_NEXT;
        break;
    case 'c':
        toolState = SET_PALETTE_1_PREV;
        break;
    case 'v':
        toolState = SET_PALETTE_1_NEXT;
        break;
    case 'b':
        toolState = SET_PALETTE_2_PREV;
        break;
    case 'n':
        toolState = SET_PALETTE_2_NEXT;
        break;
    case 'm':
        toolState = SET_PALETTE_3_PREV;
        break;
    case ',':
        toolState = SET_PALETTE_3_NEXT;
        break;
    case '.':
        toolState = SET_CGA_PALETTE_0;
        break;
    case '/':
        toolState = SET_CGA_PALETTE_1;
        break;
    case 0x1b: // ESC
        toolState = QUIT;
        break;
    case 0x05: // CTRL-E
        toolState = EXPORT;
        break;
    case 0x09: // CTRL-I
        toolState = IMPORT;
        break;
    }
}

/**
 * @brief The main tool() program
 */
void tool(void)
{
    toolState = INIT;

    while(tool_running)
    {
        switch(toolState)
        {
        case INIT:
            tool_init();
            break;
        case DRAW:
            tool_draw(current_tile);
            break;
        case GET_COMMAND:
            tool_command();
            break;
        case CURSOR_UP:
            tool_cursor_up();
            break;
        case CURSOR_DOWN:
            tool_cursor_down();
            break;
        case CURSOR_LEFT:
            tool_cursor_left();
            break;
        case CURSOR_RIGHT:
            tool_cursor_right();
            break;
        case SET_COLOR_0:
            tool_set_color_0();
            break;
        case SET_COLOR_1:
            tool_set_color_1();
            break;
        case SET_COLOR_2:
            tool_set_color_2();
            break;
        case SET_COLOR_3:
            tool_set_color_3();
            break;
        case INVERT:
            tool_invert();
            break;
        case FILL_COLOR_0:
            tool_fill_color_0();
            break;
        case FILL_COLOR_1:
            tool_fill_color_1();
            break;
        case FILL_COLOR_2:
            tool_fill_color_2();
            break;
        case FILL_COLOR_3:
            tool_fill_color_3();
            break;
        case ROL:
            tool_rol();
            break;
        case ROR:
            tool_ror();
            break;
        case ROU:
            tool_rou();
            break;
        case ROD:
            tool_rod();
            break;
        case ROTATE_CCW:
            tool_rotate_ccw();
            break;
        case ROTATE_CW:
            tool_rotate_cw();
            break;
        case PREVIOUS_CHAR:
            tool_previous_char();
            break;
        case NEXT_CHAR:
            tool_next_char();
            break;
        case SET_PALETTE_0_NEXT:
            tool_set_palette_0_next();
            break;
        case SET_PALETTE_0_PREV:
            tool_set_palette_0_prev();
            break;
        case SET_PALETTE_1_NEXT:
            tool_set_palette_1_next();
            break;
        case SET_PALETTE_1_PREV:
            tool_set_palette_1_prev();
            break;
        case SET_PALETTE_2_NEXT:
            tool_set_palette_2_next();
            break;
        case SET_PALETTE_2_PREV:
            tool_set_palette_2_prev();
            break;
        case SET_PALETTE_3_NEXT:
            tool_set_palette_3_next();
            break;
        case SET_PALETTE_3_PREV:
            tool_set_palette_3_prev();
            break;
        case SET_CGA_PALETTE_0:
            tool_set_cga_palette_0();
            break;
        case SET_CGA_PALETTE_1:
            tool_set_cga_palette_1();
            break;
        case UPDATE_PALETTE_LEGENDS:
            tool_update_palette_legends();
            break;
        case EXPORT:
            tool_export();
            break;
        case IMPORT:
            tool_import();
            break;
        case QUIT:
            tool_running = false;
            break;
        }
    }
}
