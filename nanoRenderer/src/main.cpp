#include <cmath>
#include <cstdlib>
#include <ctime>
#include "tgaimage.h"
#include "triangle.h"

int main(int argc, char** argv) {
    constexpr int width  = 128;
    constexpr int height = 128;

    constexpr TGAColor white = {255, 255, 255, 255};
    constexpr TGAColor green   = {  0, 255,   0, 255};
    constexpr TGAColor red     = {  0,   0, 255, 255};

    // Create a blank buffer
    TGAImage framebuffer(width, height, TGAImage::RGB);
  
    triangle(7, 45, 35, 100, 45, 60, framebuffer, red);
    triangle(120, 35, 90, 5, 45, 110, framebuffer, white);
    triangle(115, 83, 80, 90, 85, 120, framebuffer, green);
    
    // Write the buffer to a file
    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}
