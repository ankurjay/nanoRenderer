#include "triangle.h"

void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) {
    // Sort the vertices a, b, c in ascending Y order via bubble sort
    if (ay > by) { std::swap(ax, bx); std::swap(ay, by); }
    if (ay > cy) { std::swap(ax, cx); std::swap(ay, cy); }
    if (by > cy) { std::swap(bx, cx); std::swap(by, cy); }

    int total_height = cy - ay; 

    // Scan-line rasterization for lower half
    if (ay != by) {
        int segment_height = by - ay;
        for (int y = ay; y <= by; y++) {
            int x1 = ax + ((cx - ax) * (y - ay)) / total_height;
            int x2 = ax + ((bx - ax) * (y - ay)) / segment_height;
            
            // Draw a horizontal line without overheads from line.h
            for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++)
                framebuffer.set(x, y, color); 
        }
    }

    // Scan-line rasterization for upper half
    if (by != cy) {
        int segment_height = cy - by;
        for (int y = by; y <= cy; y++) {
            int x1 = ax + ((cx - ax) * (y - ay)) / total_height;
            int x2 = bx + ((cx - bx) * (y - by)) / segment_height;
            
            // Draw a horizontal line without overheads from line.h
            for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++)
                framebuffer.set(x, y, color); 
        }
    }
}