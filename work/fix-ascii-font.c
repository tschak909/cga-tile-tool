/**
 * @brief shift ascii font to color 3 on color 0
 */

#include <stdio.h>
#include "../src/ascii.h"

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

void main(void)
{
    int i=0;

    for (i=0;i<96;i++)
    {
        int v = i+32;
        int j = 0;

        for (j=0;j<8;j++)
        {
            int k=0;

            for (k=0;k<8;k++)
            {
                unsigned char c = tool_get_pixel_from_tile(ascii[i], k, j);

                if (c==1)
                    c=0;
                else if (c==0)
                    c=3;

                tool_set_pixel_to_tile(ascii[i], k, j, c);
            }
        }

        printf("        {\n");
        printf("            // \"%c\" (0x%02X)\n",v,v);
        printf("            0x%02X, 0x%02X,\n",ascii[i][0],ascii[i][1]);
        printf("            0x%02X, 0x%02X,\n",ascii[i][2],ascii[i][3]);
        printf("            0x%02X, 0x%02X,\n",ascii[i][4],ascii[i][5]);
        printf("            0x%02X, 0x%02X,\n",ascii[i][6],ascii[i][7]);
        printf("            0x%02X, 0x%02X,\n",ascii[i][8],ascii[i][9]);
        printf("            0x%02X, 0x%02X,\n",ascii[i][10],ascii[i][11]);
        printf("            0x%02X, 0x%02X,\n",ascii[i][12],ascii[i][13]);
        printf("            0x%02X, 0x%02X,\n",ascii[i][14],ascii[i][15]);
        printf("        },\n");
    }
}
