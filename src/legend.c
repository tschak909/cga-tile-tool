/**
 * @brief CGA tile tool
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE
 * @verbose Draw legends
 */

#include "plot_tile.h"
#include "tool.h"
#include "ascii.h"
#include "print.h"

/**
 * @brief draw legends for colors
 */
void legend_colors(void)
{
    plot_tile(color_cursor[0],11,20,3);
    plot_tile(color_chips[1],19,20,3);
    plot_tile(color_chips[2],27,20,3);
    plot_tile(color_chips[3],35,20,3);

    /* print(28,18,3,"0"); */
    /* print(28,19,3,"1"); */
    /* print(28,20,3,"2"); */
    /* print(28,21,3,"3"); */
}

/**
 * @brief key legends
 */
void legend_keys(void)
{
    unsigned char x=15;

    print(x,0,1,"         WASD");
    print(x+14,0,3,"MOVE CURSOR");
    print(x,1,1,"   SHIFT WASD");
    print(x+14,1,3,"SHIFT  BITS");
    print(x,2,1,"         0123");
    print(x+14,2,3,"PLOT  COLOR");
    print(x,3,1,"   SHIFT 0123");
    print(x+14,3,3,"CLEAR COLOR");
    print(x,4,1,"           QE");
    print(x+14,4,3,"ROTATE");
    print(x,5,1,"            I");
    print(x+14,5,3,"INVERT");
    print(x,6,1,"           ^E");
    print(x+14,6,3,"EXPORT");
    print(x,7,1,"           ^I");
    print(x+14,7,3,"IMPORT");
    print(x,8,1,"          ESC");
    print(x+14,8,3,"QUIT");

    x=23;

    print(5,20,1,"ZX");
    print(13,20,1,"CV");
    print(21,20,1,"BN");
    print(29,20,1,"M,");

}

/**
 * @brief Draw all legends
 */
void legends(void)
{
    legend_keys();
    legend_colors();
}
