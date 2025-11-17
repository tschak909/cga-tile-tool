/**
 * @brief plot a 8x8 2bpp tile to screen at column x, row y
 * @param tile ptr to 2bpp tile data * 8
 * @param x Column 0-39
 * @param y Row 0-24
 */

/**
 * @brief Number of bytes in a single line of CGA graphics
 */
#define VIDEO_LINE_BYTES 80

/**
 * @brief 0xB800 segment offset for odd lines
 */
#define VIDEO_ODD_OFFSET 0x2000

/**
 * @brief Address of CGA video RAM
 */
#define VIDEO_RAM_ADDR ((unsigned char far *)0xB8000000UL)

/**
 * @brief Pointer to video RAM
 */
unsigned char far *video = VIDEO_RAM_ADDR;

/**
 * @brief Plot a single 8x8 tile at requested coordinates
 * @param tile Pointer to tile data, expected to be 16 bytes in length
 * @param x horizontal position (0-39)
 * @param y vertical position (0-199)
 * @param c color (0-3)
 */
void plot_tile(const unsigned char *tile, unsigned char x, unsigned char y, unsigned char c)
{
    unsigned char i=0;
    unsigned char m=0;

    if (y<25)
        y <<= 3; // Convert row to line

    x <<= 1; // Convert column to video ram offset

    for (i=0;i<8;i++)
    {
        unsigned char r = y + i;
        unsigned char rh = r >> 1; // Because CGA is interleaved odd/even.
        unsigned short ro = rh * VIDEO_LINE_BYTES + x;

        // If row is odd, go into second bank.
        if (r & 1)
            ro += VIDEO_ODD_OFFSET;

        // Set mask for given color
        switch(c)
        {
        case 0:
            m = 0x00;
            break;
        case 1:
            m = 0x55;
            break;
        case 2:
            m = 0xAA;
            break;
        case 3:
            m = 0xFF;
            break;
        }

        // Put tile data into video RAM.
        video[ro] = tile[i*2];
        video[ro+1] = tile[i*2+1];
        video[ro] &= m;
        video[ro+1] &= m;
    }
}

/**
 * @brief Plot a single 8x8 tile at requested coordinates XOR
 * @param tile Pointer to tile data, expected to be 16 bytes in length
 * @param x horizontal position (0-39)
 * @param y vertical position (0-199)
 */
void plot_tile_xor(const unsigned char *tile, unsigned char x, unsigned char y)
{
    unsigned char i=0;
    unsigned char m=0;

    if (y<25)
        y <<= 3; // Convert row to line

    x <<= 1; // Convert column to video ram offset

    for (i=0;i<8;i++)
    {
        unsigned char r = y + i;
        unsigned char rh = r >> 1; // Because CGA is interleaved odd/even.
        unsigned short ro = rh * VIDEO_LINE_BYTES + x;

        // If row is odd, go into second bank.
        if (r & 1)
            ro += VIDEO_ODD_OFFSET;

        // Put tile data into video RAM.
        video[ro] ^= tile[i*2];
        video[ro+1] ^= tile[i*2+1];
    }
}
