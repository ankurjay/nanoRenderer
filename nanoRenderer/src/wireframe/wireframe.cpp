#include "wireframe.h"
#include "line.h"
#include "tgaimage.h"

constexpr TGAColor EDGE_COLOR = {0, 0, 255, 255}; // RED
constexpr TGAColor VERTEX_COLOR = {255, 255, 255, 255}; // WHITE

Wireframe::Wireframe(const std::string filename) : model(filename) {}

/**
* Input models are assumed to have been scaled to fit the [-1,1]^3 cube world coordinates
* We want to project (x,y,z) orthogonally to (x,y), shift the (x,y) to be in [0, 2]^2 square
* and finally scale it to span the screen defined by width and height 
*/
std::tuple<int, int> Wireframe::project_orthogonal(vec3 v, int width, int height) {
    return {
        (v.x + 1.f) * width / 2,
        (v.y + 1.f) * height / 2
    };
}


void Wireframe::render(TGAImage &framebuffer) {
    int width = framebuffer.width();
    int height = framebuffer.height();

    for (int i = 0; i < model.nfaces(); i++) {
        // Iterate through all triangles
        auto [ax, ay] = project_orthogonal(model.vert(i, 0), width, height);
        auto [bx, by] = project_orthogonal(model.vert(i, 1), width, height);
        auto [cx, cy] = project_orthogonal(model.vert(i, 2), width, height);

        line(ax, ay, bx, by, framebuffer, EDGE_COLOR);
        line(bx, by, cx, cy, framebuffer, EDGE_COLOR);
        line(ax, ay, cx, cy, framebuffer, EDGE_COLOR);
    }
    
    // Draw the model's vertices as points on the screen
    for (int i = 0; i < model.nverts(); i++) {
        vec3 v = model.vert(i);
        auto [x, y] = project_orthogonal(v, width, height);
        framebuffer.set(x, y, VERTEX_COLOR);
    }
}

