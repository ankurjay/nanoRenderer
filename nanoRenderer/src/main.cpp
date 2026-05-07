#include <cmath>
#include <cstdlib>
#include <ctime>
#include "tgaimage.h"
#include "mesh.h"

int main(int argc, char** argv) {
    constexpr int width  = 800;
    constexpr int height = 800;

    // Create a blank buffer
    TGAImage framebuffer(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);
    
    // Create a mesh
    Mesh mesh(argv[1]);

    // Render the mesh
    mesh.render(framebuffer, zbuffer);
    
    // Write the buffer to a file
    framebuffer.write_tga_file("framebuffer.tga");
    zbuffer.write_tga_file("zbuffer.tga");

    return 0;
}
