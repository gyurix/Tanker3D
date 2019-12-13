#pragma once

#include "element.h"
#include <vector>

class Rectangle : public Element {
public:
    glm::vec2 tpos1;
    glm::vec2 tpos2;

    Rectangle(glm::vec2 tpos1, glm::vec2 tpos2);

    void init();

private:
    void setup_vertices();

    void setup_mesh();

    void setup_tex_coords();

    void render(glm::vec3 vPos, float rot);
};