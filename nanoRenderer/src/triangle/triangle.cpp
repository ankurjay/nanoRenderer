#include "triangle.h"

std::tuple<int, int> minmax3(int a, int b, int c) {
    return { std::min(std::min(a, b), c), std::max(std::max(a, b), c)};
}

double signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return 0.5 * ((by - ay)*(bx + ax) + (cy - by)*(cx + bx) + (ay - cy)*(ax + cx));
}

void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) {
    auto [bbminx, bbmaxx] = minmax3(ax, bx, cx);
    auto [bbminy, bbmaxy] = minmax3(ay, by, cy);
    double total_area = signed_triangle_area(ax, ay, bx, by, cx, cy);
    if (total_area < 1) return; // Backface culling + discarding triangles that cover less than a pixel

#pragma omp parallel for
    for (int x = bbminx; x <= bbmaxx; x++) {
        for (int y = bbminy; y <= bbmaxy; y++) {
            // Compute the barycentric coordinates
            double alpha = signed_triangle_area(x, y, bx, by, cx, cy) / total_area;
            double beta  = signed_triangle_area(x, y, cx, cy, ax, ay) / total_area;
            double gamma = signed_triangle_area(x, y, ax, ay, bx, by) / total_area;

            // Negative barycentric coordinate means that the pixel is outside the triangle
            if (alpha < 0 || beta < 0 || gamma < 0) continue; // Skip rendering

            framebuffer.set(x, y, color); // Otherwise, render the pixel
        }
    }
}

void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &framebuffer, TGAImage &zbuffer, TGAColor color) {
    auto [bbminx, bbmaxx] = minmax3(ax, bx, cx);
    auto [bbminy, bbmaxy] = minmax3(ay, by, cy);
    double total_area = signed_triangle_area(ax, ay, bx, by, cx, cy);
    if (total_area < 1) return; // Backface culling + discarding triangles that cover less than a pixel

#pragma omp parallel for
    for (int x = bbminx; x <= bbmaxx; x++) {
        for (int y = bbminy; y <= bbmaxy; y++) {
            // Compute the barycentric coordinates
            double alpha = signed_triangle_area(x, y, bx, by, cx, cy) / total_area;
            double beta  = signed_triangle_area(x, y, cx, cy, ax, ay) / total_area;
            double gamma = signed_triangle_area(x, y, ax, ay, bx, by) / total_area;

            // Negative barycentric coordinate means that the pixel is outside the triangle
            if (alpha < 0 || beta < 0 || gamma < 0) continue; // Skip rendering
            unsigned char z = static_cast<unsigned char>(alpha * az + beta * bz + gamma * cz);
            if (z <= zbuffer.get(x, y)[0]) continue;
            zbuffer.set(x, y, {z});
            framebuffer.set(x, y, color); // Otherwise, render the pixel
        }
    }
}