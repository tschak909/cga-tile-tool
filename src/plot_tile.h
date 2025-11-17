#ifndef PLOT_TILE_H
#define PLOT_TILE_H

/**
 * @brief Plot a single 8x8 tile at requested coordinates
 * @param tile Pointer to tile data, expected to be 16 bytes in length
 * @param x horizontal position (0-39)
 * @param y vertical position (0-199)
 * @param c Color (0-3)
 */
void plot_tile(const unsigned char *tile, unsigned char x, unsigned char y, unsigned char c);

/**
 * @brief Plot a single 8x8 tile at requested coordinates XOR
 * @param tile Pointer to tile data, expected to be 16 bytes in length
 * @param x horizontal position (0-39)
 * @param y vertical position (0-199)
 */
void plot_tile_xor(const unsigned char *tile, unsigned char x, unsigned char y);


#endif /* PLOT_TILE_H */
