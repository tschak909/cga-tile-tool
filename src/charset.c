/**
 * @brief CGA tile tool
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE
 * @verbose Character set functions
 */

#include <string.h>
#include "ascii.h"
#include "plot_tile.h"
#include "print.h"
#include "tool.h"

extern unsigned int current_tile_index;

/**
 * @brief Character set buffer 256 chars.
 */
unsigned char charset[256][16];

/**
 * @brief Initialize character set from ASCII set
 */
void charset_init(void)
{
    memcpy(&charset[32],&ascii[0],sizeof(ascii));
}

/**
 * @brief Display charset cursor
 * @param c Character # to display (0-255)
 */
void charset_cursor(unsigned char c)
{
    unsigned char y = c / 40;
    unsigned char x = c % 40;

    y += 10;

    plot_tile_xor(charset_cursor_tile,x,y);
}

/**
 * @brief Display entire character set
 */
void charset_draw_all(void)
{
    int i=0;
    unsigned char x=0, y=10;

    print(0,9,3,"----------------------------------------");

    for (i=0;i<256;i++)
    {
        plot_tile(charset[i],x,y,3);

        if (x>38)
        {
            x=0;
            y++;
        }
        else
        {
            x++;
        }

        if (i==current_tile_index)
            charset_cursor(i);
    }

    print(0,16,3,"----------------------------------------");
}
