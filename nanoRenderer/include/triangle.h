#pragma once
#include <cstdlib>
#include "tgaimage.h"

/**
 * @brief An implementation of triangle rasterization
 */
void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color);

/**
 * @brief An implementation of triangle rasterization with Z-buffers
 */
void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &framebuffer, TGAImage &zbuffer, TGAColor color);