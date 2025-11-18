/**
 * @brief CGA tile tool
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE
 * @verbose Tool functions
 */

#ifndef TOOL_H
#define TOOL_H

enum _toolState
{
    INIT,
    DRAW,
    QUIT,
    GET_COMMAND,
    CURSOR_UP,
    CURSOR_DOWN,
    CURSOR_LEFT,
    CURSOR_RIGHT,
    SET_COLOR_0,
    SET_COLOR_1,
    SET_COLOR_2,
    SET_COLOR_3,
    INVERT,
    FILL_COLOR_0,
    FILL_COLOR_1,
    FILL_COLOR_2,
    FILL_COLOR_3,
    ROL,
    ROR,
    ROU,
    ROD,
    ROTATE_CCW,
    ROTATE_CW,
    PREVIOUS_CHAR,
    NEXT_CHAR,
    SET_PALETTE_0_NEXT,
    SET_PALETTE_0_PREV,
    SET_PALETTE_1_NEXT,
    SET_PALETTE_1_PREV,
    SET_PALETTE_2_NEXT,
    SET_PALETTE_2_PREV,
    SET_PALETTE_3_NEXT,
    SET_PALETTE_3_PREV,
    SET_CGA_PALETTE_0,
    SET_CGA_PALETTE_1,
    UPDATE_PALETTE_LEGENDS,
    EXPORT,
    IMPORT
} toolState = DRAW;

/**
 * @brief the charset cursor
 */
const unsigned char charset_cursor_tile[16] =
{
    0xAA, 0xAA,
    0x80, 0x02,
    0x80, 0x02,
    0x80, 0x02,
    0x80, 0x02,
    0x80, 0x02,
    0x80, 0x02,
    0xAA, 0xAA
};

/**
 * @brief colored tiles, used for each tile pixel in tool view.
 */
const unsigned char color_cursor[4][16] =
    {
        // Color 0
        {
            0xFF, 0xFF,
            0xC0, 0x03,
            0xC0, 0x03,
            0xC0, 0x03,
            0xC0, 0x03,
            0xC0, 0x03,
            0xC0, 0x03,
            0xFF, 0xFF
        },
        // Color 1
        {
            0xFF, 0xFF,
            0xD5, 0x57,
            0xD5, 0x57,
            0xD5, 0x57,
            0xD5, 0x57,
            0xD5, 0x57,
            0xD5, 0x57,
            0xFF, 0xFF
        },
        // Color 2
        {
            0xFF, 0xFF,
            0xEA, 0xAB,
            0xEA, 0xAB,
            0xEA, 0xAB,
            0xEA, 0xAB,
            0xEA, 0xAB,
            0xEA, 0xAB,
            0xFF, 0xFF
        },
        // Color 3
        {
            0x00, 0x00,
            0x7F, 0xFE,
            0x7F, 0xFE,
            0x7F, 0xFE,
            0x7F, 0xFE,
            0x7F, 0xFE,
            0x7F, 0xFE,
            0x00, 0x00
        }
    };

/**
 * @brief colored tiles, used for each tile pixel in tool view.
 */
const unsigned char color_chips[4][16] =
{
    // Color 0
    {
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00
    },
    // Color 1
    {
        0x55, 0x55,
        0x55, 0x55,
        0x55, 0x55,
        0x55, 0x55,
        0x55, 0x55,
        0x55, 0x55,
        0x55, 0x55,
        0x55, 0x55
    },
    // Color 2
    {
        0xAA, 0xAA,
        0xAA, 0xAA,
        0xAA, 0xAA,
        0xAA, 0xAA,
        0xAA, 0xAA,
        0xAA, 0xAA,
        0xAA, 0xAA,
        0xAA, 0xAA
    },
    // Color 3
    {
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF,
        0xFF, 0xFF
    }
};


/**
 * @brief The main tool() program
 */
void tool(void);

/**
 * @brief Import/export setup
 */
void tool_import_export_setup(int argc, char *argv[]);

#endif /* TOOL_H */
