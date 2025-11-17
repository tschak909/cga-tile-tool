/**
 * @brief CGA tile tool
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE
 * @verbose main program
 */

#include "init.h"
#include "legend.h"
#include "tool.h"
#include "done.h"
#include "charset.h"

/**
 * @brief Previous graphics mode, set by init, used by done.
 */
int oldMode=0;

/**
 * @brief Program entry point
 */
int main(void)
{
    oldMode = init();
    charset_init();
    legends();
    charset_draw_all();
    tool();
    done(oldMode);

    return 0;
}
