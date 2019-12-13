#pragma once

#include "element.h"
#include <vector>

class Cube : public Element {
public:
    glm::vec3 pos1{-0.5f, -0.5f, -0.5f}, pos2{0.5f, 0.0f, 0.5f};
    glm::vec2 frontTexture{0.0f, 0.75f}, backTexture{0.125f, 0.75f};
    glm::vec2 leftTexture{0.25f, 0.75f}, rightTexture{0.375f, 0.75f};
    glm::vec2 upTexture{0.5f, 0.75f}, downTexture{0.625f, 0.75f};
    glm::vec2 textureSize{0.125f, 0.125f};

    Cube();

    void init();

    void setTextureY(float y);

private:
    void setup_vertices();

    void setup_mesh();

    void setup_tex_coords();
};