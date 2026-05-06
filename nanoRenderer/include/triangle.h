#pragma once
#include <cstdlib>
#include "tgaimage.h"

/**
 * @brief An implementation of triangle rasterization
 */
void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color);