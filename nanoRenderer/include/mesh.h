#pragma once

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

class Mesh {
    Model model;
    std::tuple<int, int> project_orthogonal(vec3 v, int width, int height);

    public:
        Mesh(const std::string filename);
        void render(TGAImage &framebuffer);
};