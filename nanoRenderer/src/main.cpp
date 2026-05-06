#include <cmath>
#include <cstdlib>
#include <ctime>
#include "tgaimage.h"
#include "wireframe.h"

int main(int argc, char** argv) {
    constexpr int width  = 800;
    constexpr int height = 800;

    // Create a blank buffer
    TGAImage framebuffer(width, height, TGAImage::RGB);
  
    // Create a Wireframe object by reading from an obj file
    Wireframe wireframe(argv[1]);

    // Render the object onto the buffer
    wireframe.render(framebuffer);
    
    // Write the buffer to a file
    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}
