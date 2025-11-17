/**
 * @brief CGA tile tool
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE
 * @verbose Done Funcs
 */

#include <i86.h>
#include <dos.h>
#include "done.h"
#include "defines.h"

void done(int oldMode)
{
    union REGS r;

    // Set graphics mode
    r.h.ah = FUNC_SET_MODE;
    r.h.al = oldMode;
    int86(INT_VIDEO,&r,&r);
}
