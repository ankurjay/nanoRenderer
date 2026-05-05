#include "line.h"

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color)
{
    // First, check if the line is steep
    bool steep = std::abs(ax - bx) < std::abs(ay - by);

    // If the line is steep, we transpose the coordinates
    if (steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    if (ax > bx) {
       // make it go left-to-right
       std::swap(ax, bx);
       std::swap(ay, by); 
    }

    auto dtor = bx - ax;
    auto ntor = by - ay;
    auto abs_ntor = std::abs(ntor);

    int y = ay;
    int ierror = 0;
    int y_step = (ntor > 0) ? 1 : -1; // If positive slope, increment 1 else decrement 1
    
    if (steep) {
        for (int x = ax; x <= bx; x++) {
            // Since we transposed the coordinates earlier, we need to de-transpose them
            framebuffer.set(y, x, color);
            ierror += 2 * abs_ntor;
            y += y_step                 * (ierror > dtor); 
            ierror -= 2 * dtor          * (ierror > dtor);
        }
    } else {
        for (int x = ax; x <= bx; x++) {
            framebuffer.set(x, y, color);
            ierror += 2 * abs_ntor;
            y += y_step                 * (ierror > dtor); 
            ierror -= 2 * dtor          * (ierror > dtor);
        }
    }
}
