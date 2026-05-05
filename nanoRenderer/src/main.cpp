#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include "line.h"
#include "tgaimage.h"

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;

    // Create a blank buffer
    TGAImage framebuffer(width, height, TGAImage::RGB);
   
    // Write the buffer to a file
    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}
