#pragma once
#include <cstdlib>
#include "tgaimage.h"

/**
 * @brief An implementation of Bresenham's Line Drawing Algorithm
 */
void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color);