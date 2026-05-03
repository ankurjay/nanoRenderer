#include "tgaimage.h"

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;

    TGAImage framebuffer(width, height, TGAImage::RGB);
    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}