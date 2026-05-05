#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include "tgaimage.h"

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
    for (int x = ax; x <= bx; x++) {
        // If transposed, de-transpose
        if (steep) {
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
        ierror += 2 * abs_ntor;
        if (ierror > dtor) {
            y += (ntor > 0) ? 1 : -1; // If positive slope, increment 1 else decrement 1
            ierror -= 2 * dtor;
        }
    }
}


int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;

    // Create a blank buffer
    TGAImage framebuffer(width, height, TGAImage::RGB);
   
    std::srand(std::time({}));

    for (int i = 0; i < (1<<24); i++) {
        int ax = rand() % width, ay = rand() % height;
        int bx = rand() % width, by = rand() % height;
        TGAColor color = {
            static_cast<uint8_t>(rand() % 255),
            static_cast<uint8_t>(rand() % 255),
            static_cast<uint8_t>(rand() % 255),
            static_cast<uint8_t>(rand() % 255)
        };
        line(ax, ay, bx, by, framebuffer, color);
    }

    // Write the buffer to a file
    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}
