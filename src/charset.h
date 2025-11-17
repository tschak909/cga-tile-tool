/**
 * @brief CGA tile tool
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3, see LICENSE
 * @verbose Character set functions
 */

#ifndef CHARSET_H
#define CHARSET_H

/**
 * @brief Character set buffer 256 chars.
 */
extern unsigned char charset[256][16];

/**
 * @brief Initialize character set from ASCII set
 */
void charset_init(void);

/**
 * @brief Display charset cursor
 * @param c Character # to display (0-255)
 */
void charset_cursor(unsigned char c);

/**
 * @brief Display entire character set
 */
void charset_draw_all(void);

#endif /* CHARSET_H */
