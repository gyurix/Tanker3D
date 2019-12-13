#pragma once

#include "element.h"
#include <vector>

#define DIRT 3
#define STONE 2
#define TUNNEL 1

class Background : public Element {
public:
    glm::i32vec2 size;
    char *materials;
    Background(glm::i32vec2 size);
    char get_material(int x, int y);
    void init();
    void set_material(int x, int z, char material);

    bool destroyMap(float xIn, float zIn);

    bool canBreak(float xIn, float zIn);

    void gen_background();

    char print_material(char mat);

private:
    void setup_mesh();
    void setup_tex_coords(char material);
    void setup_vertices();
    void update_tex_coords(int x, int z, char material);
};