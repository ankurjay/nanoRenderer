#pragma once

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

class Wireframe {
    Model model;
    std::tuple<int, int> project_orthogonal(vec3 v, int width, int height);

    public:
        Wireframe(const std::string filename);
        void render(TGAImage &framebuffer);
};