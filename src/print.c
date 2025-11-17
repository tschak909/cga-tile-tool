/**
 * @brief CGA tile tool
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE
 * @verbose Print string
 */

#include "print.h"
#include "plot_tile.h"
#include "ascii.h"

/**
 * @brief Print string at location x,y using ASCII charset
 * @param x Horizontal position (0-39)
 * @param y Vertical position (0-23)
 * @param c Color (0-3)
 * @param s Pointer to string
 */
void print(unsigned char x, unsigned char y, unsigned char c, const char *s)
{
    signed char b=0;

    while (b = *s++)
    {
        b -= 32;
        if (b<0 || b>96)
            continue;

        if (x>39)
        {
            x=0;
            y++;
        }

        plot_tile(&ascii[b],x++,y,c);
    }
}
