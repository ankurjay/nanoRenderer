#include "mesh.h"
#include "tgaimage.h"
#include "triangle.h"

constexpr TGAColor EDGE_COLOR = {0, 0, 255, 255}; // RED
constexpr TGAColor VERTEX_COLOR = {255, 255, 255, 255}; // WHITE

Mesh::Mesh(const std::string filename) : model(filename) {}

/**
* Input models are assumed to have been scaled to fit the [-1,1]^3 cube world coordinates
* We want to project (x,y,z) orthogonally to (x,y), shift the (x,y) to be in [0, 2]^2 square
* and finally scale it to span the screen defined by width and height 
*/
std::tuple<int, int, int> Mesh::project_orthogonal(vec3 v, int width, int height) {
    return {
        (v.x + 1.f) * width / 2,
        (v.y + 1.f) * height / 2,
        (v.z + 1.f) * 255 / 2
    };
}


void Mesh::render(TGAImage &framebuffer, TGAImage &zbuffer) {
    int width = framebuffer.width();
    int height = framebuffer.height();

    for (int i = 0; i < model.nfaces(); i++) {
        // Iterate through all triangles
        auto [ax, ay, az] = project_orthogonal(model.vert(i, 0), width, height);
        auto [bx, by, bz] = project_orthogonal(model.vert(i, 1), width, height);
        auto [cx, cy, cz] = project_orthogonal(model.vert(i, 2), width, height);
        TGAColor rnd;
        for (int i = 0; i < 3; i++) rnd[i] = std::rand() % 255;
        triangle(ax, ay, az, bx, by, bz, cx, cy, cz, framebuffer, zbuffer, rnd);
    }    
}

