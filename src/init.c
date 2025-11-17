/**
 * @brief CGA tile tool
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE
 * @verbose Initialization functions
 */

#include <dos.h>
#include <i86.h>
#include "defines.h"

int init(void)
{
    union REGS r;
    int oldMode=0;

    // Get old mode
    r.h.ah = FUNC_GET_MODE;
    int86(INT_VIDEO,&r,&r);

    oldMode=r.h.al;

    // Set graphics mode
    r.h.ah = FUNC_SET_MODE;
    r.h.al = APA_320_200_4;
    int86(INT_VIDEO,&r,&r);

    return oldMode;
}
