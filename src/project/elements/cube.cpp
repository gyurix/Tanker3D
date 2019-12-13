#include "cube.h"

Cube::Cube() : Element{} {

}

void Cube::setup_vertices() {

    // LEFT
    vertices.push_back({pos1.x, pos1.y, pos1.z});
    vertices.push_back({pos1.x, pos1.y, pos2.z});
    vertices.push_back({pos1.x, pos2.y, pos2.z});
    vertices.push_back({pos1.x, pos2.y, pos1.z});

    // DOWN
    vertices.push_back({pos1.x, pos1.y, pos1.z});
    vertices.push_back({pos2.x, pos1.y, pos1.z});
    vertices.push_back({pos2.x, pos1.y, pos2.z});
    vertices.push_back({pos1.x, pos1.y, pos2.z});

    // BACK
    vertices.push_back({pos2.x, pos1.y, pos1.z});
    vertices.push_back({pos1.x, pos1.y, pos1.z});
    vertices.push_back({pos1.x, pos2.y, pos1.z});
    vertices.push_back({pos2.x, pos2.y, pos1.z});

    // RIGHT
    vertices.push_back({pos2.x, pos1.y, pos2.z});
    vertices.push_back({pos2.x, pos1.y, pos1.z});
    vertices.push_back({pos2.x, pos2.y, pos1.z});
    vertices.push_back({pos2.x, pos2.y, pos2.z});

    // UP
    vertices.push_back({pos1.x, pos2.y, pos2.z});
    vertices.push_back({pos2.x, pos2.y, pos2.z});
    vertices.push_back({pos2.x, pos2.y, pos1.z});
    vertices.push_back({pos1.x, pos2.y, pos1.z});

    // FRONT
    vertices.push_back({pos1.x, pos1.y, pos2.z});
    vertices.push_back({pos2.x, pos1.y, pos2.z});
    vertices.push_back({pos2.x, pos2.y, pos2.z});
    vertices.push_back({pos1.x, pos2.y, pos2.z});
}

void Cube::setup_mesh() {
    for (GLuint st = 0; st < 24; st += 4) {
        mesh.push_back({st, st + 1, st + 2});
        mesh.push_back({st, st + 2, st + 3});
    }
}

void Cube::setTextureY(float y) {
    frontTexture.y = backTexture.y = leftTexture.y = rightTexture.y = upTexture.y = downTexture.y = y;
}

void Cube::setup_tex_coords() {
    for (glm::vec2 cmin : {leftTexture, downTexture, backTexture, rightTexture, upTexture, frontTexture}) {
        glm::vec2 cmax = cmin + textureSize;
        texCoords.push_back({cmin.x, cmin.y});
        texCoords.push_back({cmax.x, cmin.y});
        texCoords.push_back({cmax.x, cmax.y});
        texCoords.push_back({cmin.x, cmax.y});
    }
}

void Cube::init() {
    setup_vertices();
    setup_mesh();
    setup_tex_coords();
}