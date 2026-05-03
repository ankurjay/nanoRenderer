#include "tgaimage.h"


/**
* @brief Basic function that takes in a TGAImage and sets 3 pixels to white at specified coordinates
*/
void example(TGAImage &framebuffer) {
    constexpr TGAColor white = {255, 255, 255, 255 };
    
    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    framebuffer.set(ax, ay, white);
    framebuffer.set(bx, by, white);
    framebuffer.set(cx, cy, white);
}

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;

    // Create a blank buffer
    TGAImage framebuffer(width, height, TGAImage::RGB);
    
    // Modify pixels
    example(framebuffer);

    // Write the buffer to a file
    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}