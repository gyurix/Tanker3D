#include "rectangle.h"

Rectangle::Rectangle(glm::vec2 tp1, glm::vec2 tp2) : Element{} {
    tpos1 = tp1;
    tpos2 = tp2;
}

void Rectangle::setup_vertices() {
    glm::vec3 hsize{1.0f, 1.0f, 0.0f};
    vertices.push_back({-hsize.x, -hsize.y, -hsize.z});
    vertices.push_back({hsize.x, -hsize.y, hsize.z});
    vertices.push_back({hsize.x, hsize.y, hsize.z});
    vertices.push_back({-hsize.x, hsize.y, -hsize.z});
}

void Rectangle::setup_mesh() {
    mesh.push_back({0, 1, 2});
    mesh.push_back({0, 2, 3});
}

void Rectangle::setup_tex_coords() {
    texCoords.push_back({tpos1.x, tpos1.y});
    texCoords.push_back({tpos2.x, tpos1.y});
    texCoords.push_back({tpos2.x, tpos2.y});
    texCoords.push_back({tpos1.x, tpos2.y});
}

void Rectangle::init() {
    setup_vertices();
    setup_mesh();
    setup_tex_coords();
    position = {0.0f, 0.0f, 0.0f};
}