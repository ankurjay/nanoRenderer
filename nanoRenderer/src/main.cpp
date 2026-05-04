#include <cmath>
#include "tgaimage.h"

constexpr TGAColor white = {255, 255, 255, 255};
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color)
{
    auto denom = static_cast<float>(bx - ax);
    auto ydiff = by - ay;
    if (ax > bx) {
       // make it go left-to-right
       std::swap(ax, bx);
       std::swap(ay, by); 
    }

    for (int x = ax; x <= bx; x++) {
        float t = (x - ax) / denom;
        int y = std::round(ay + ydiff * t);
        framebuffer.set(x, y, color);
    }
}


int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;

    // Create a blank buffer
    TGAImage framebuffer(width, height, TGAImage::RGB);
    
    // Define points
    int ax = 7, ay = 3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;
    
    // Draw Lines
    line(ax, ay, bx, by, framebuffer, blue);
    line(cx, cy, bx, by, framebuffer, green);
    line(cx, cy, ax, ay, framebuffer, yellow);
    // same line as above, but coordinates reversed
    line(ax, ay, cx, cy, framebuffer, red);

    // Draw corner points
    framebuffer.set(ax, ay, white);
    framebuffer.set(bx, by, white);
    framebuffer.set(cx, cy, white);

    // Write the buffer to a file
    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}