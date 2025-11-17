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
    plot_tile(color_chips[3],36,20,3);

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
    print(x,7,1,"          ESC");
    print(x+14,7,3,"QUIT");

    x=23;

    print(8,20,1,"ZX");
    print(16,20,1,"CV");
    print(24,20,1,"BN");
    print(32,20,1,"M,");

    /* print(x,18,3,"[MJ]      COLOR 0"); */
    /* print(x,19,3,"[VF]      COLOR 1"); */
    /* print(x,20,3,"[BG]      COLOR 2"); */
    /* print(x,21,3,"[NH]      COLOR 3"); */


}

/**
 * @brief Draw all legends
 */
void legends(void)
{
    legend_keys();
    legend_colors();
}
