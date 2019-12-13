#include "background.h"

Background::Background(glm::i32vec2 size) : Element{} {
    this->size = size;
    this->tid = load_texture("assets/bg.bmp");
    materials = (char *) malloc(sizeof(char) * (GLuint) size.x * (GLuint) size.y);
}

bool Background::canBreak(float xIn, float zIn) {
    xIn += xIn > 0 ? 0.5f : -0.5f;
    zIn += zIn > 0 ? 0.5f : -0.5f;
    return get_material((int) xIn, (int) zIn) != STONE;
}

bool Background::destroyMap(float xIn, float zIn) {
    xIn += xIn > 0 ? 0.5f : -0.5f;
    zIn += zIn > 0 ? 0.5f : -0.5f;
    bool result = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int x = (int) (xIn + dx * 0.5);
            int z = (int) (zIn + dy * 0.5);
            if (get_material(x, z) == DIRT) {
                set_material(x, z, TUNNEL);
                result = 1;
            }
        }
    }
    updateTexCoords();
    return result;
}

void Background::setup_vertices() {
    vertices.clear();
    glm::vec3 hsize{0.5f, 0.5f, 0.5f};
    int minx = -(size.x - 1) / 2;
    int maxx = size.x / 2;
    int minz = -(size.y - 1) / 2;
    int maxz = size.y / 2;
    for (int x = minx; x <= maxx; ++x) {
        for (int z = minz; z <= maxz; ++z) {
            for (float mult = -1; mult < 2; mult += 2) {
                vertices.push_back({x + mult * hsize.x, -hsize.y, z - hsize.z});
                vertices.push_back({x + mult * hsize.x, -hsize.y, z + hsize.z});
                vertices.push_back({x + mult * hsize.x, hsize.y, z + hsize.z});
                vertices.push_back({x + mult * hsize.x, hsize.y, z + -hsize.z});

                vertices.push_back({x - hsize.x, mult * hsize.y, z - hsize.z});
                vertices.push_back({x + hsize.x, mult * hsize.y, z - hsize.z});
                vertices.push_back({x + hsize.x, mult * hsize.y, z + hsize.z});
                vertices.push_back({x - hsize.x, mult * hsize.y, z + hsize.z});

                vertices.push_back({x - hsize.x, -hsize.y, z + mult * hsize.z});
                vertices.push_back({x + hsize.x, -hsize.y, z + mult * hsize.z});
                vertices.push_back({x + hsize.x, hsize.y, z + mult * hsize.z});
                vertices.push_back({x - hsize.x, hsize.y, z + mult * hsize.z});
            }
        }
    }
}

void Background::set_material(int x, int z, char material) {
    update_tex_coords(x, z, material);
}

void Background::setup_mesh() {
    GLuint maxst = (GLuint) (24 * size.x * size.y);
    for (GLuint st = 0; st < maxst; st += 4) {
        mesh.push_back({st, st + 1, st + 2});
        mesh.push_back({st, st + 2, st + 3});
    }
}

void Background::setup_tex_coords(char material) {
    glm::vec2 coordMin = {(material % 2) / 2.0f + 0.01f, (material / 2) / 2.0f + 0.01f};
    glm::vec2 coordMax = {coordMin.x + 0.48f, coordMin.y + 0.48f};

    for (int i = 0; i < 2; ++i) {
        texCoords.push_back({coordMax.x, coordMin.y});
        texCoords.push_back({coordMax.x, coordMax.y});
        texCoords.push_back({coordMin.x, coordMax.y});
        texCoords.push_back({coordMin.x, coordMin.y});
    }

    texCoords.push_back({coordMin.x, coordMin.y});
    texCoords.push_back({coordMin.x, coordMax.y});
    texCoords.push_back({coordMax.x, coordMax.y});
    texCoords.push_back({coordMax.x, coordMin.y});

    texCoords.push_back({coordMax.x, coordMax.y});
    texCoords.push_back({coordMax.x, coordMin.y});
    texCoords.push_back({coordMin.x, coordMin.y});
    texCoords.push_back({coordMin.x, coordMax.y});

    texCoords.push_back({coordMin.x, coordMin.y});
    texCoords.push_back({coordMin.x, coordMax.y});
    texCoords.push_back({coordMax.x, coordMax.y});
    texCoords.push_back({coordMax.x, coordMin.y});

    texCoords.push_back({coordMax.x, coordMin.y});
    texCoords.push_back({coordMax.x, coordMax.y});
    texCoords.push_back({coordMin.x, coordMax.y});
    texCoords.push_back({coordMin.x, coordMin.y});
}

void Background::init() {
    setup_vertices();
    setup_mesh();
    gen_background();
}

char Background::print_material(char mat) {
    return mat == STONE ? 'S' : mat == DIRT ? 'D' : mat == TUNNEL ? 'T' : '?';
}

void Background::gen_background() {
    for (int y = 1; y < size.y - 1; ++y) {
        for (int x = 1; x < size.x - 1; ++x) {
            materials[x * size.y + y] = rand() % 10 == 0 ? STONE : DIRT;
        }
    }
    for (int x = 0; x < size.x; ++x) {
        materials[x * size.y] = STONE;
        materials[x * size.y + size.y - 1] = STONE;
    }

    for (int y = 0; y < size.y; ++y) {
        materials[y] = STONE;
        materials[(size.x - 1) * size.y + y] = STONE;
    }
    /*int offsetX = (size.x - 1) / 2;
    int offsetZ = (size.y - 1) / 2;
    materials[offsetX * size.y + offsetZ] = STONE;*/

    /*printf("Background:\n    ");
    for (int x = 0; x < size.x; ++x)
        printf("%4d", x - offsetX);
    printf("\n");
    for (int y = 0; y < size.y; ++y) {
        printf("%4d", y - offsetZ);
        for (int x = 0; x < size.x; ++x) {
            printf("   %c", print_material(get_material(x - offsetX, y - offsetZ)));
        }
        printf("\n");
    }*/

    texCoords.clear();
    for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x) {
            setup_tex_coords(materials[y * size.x + x]);
        }
    }
}

void Background::update_tex_coords(int x, int z, char material) {
    x += (size.x - 1) / 2;
    z += (size.y - 1) / 2;
    materials[x * size.y + z] = material;
    glm::vec2 coordMin = {(material % 2) / 2.0f + 0.01f, (material / 2) / 2.0f + 0.01f};
    glm::vec2 coordMax = {coordMin.x + 0.48f, coordMin.y + 0.48f};
    GLuint st = (GLuint) (x * size.y + z) * 24;
    texCoords[st] = {coordMax.x, coordMin.y};
    texCoords[st + 1] = {coordMax.x, coordMax.y};
    texCoords[st + 2] = {coordMin.x, coordMax.y};
    texCoords[st + 3] = {coordMin.x, coordMin.y};

    texCoords[st + 4] = {coordMax.x, coordMin.y};
    texCoords[st + 5] = {coordMax.x, coordMax.y};
    texCoords[st + 6] = {coordMin.x, coordMax.y};
    texCoords[st + 7] = {coordMin.x, coordMin.y};

    texCoords[st + 8] = {coordMin.x, coordMin.y};
    texCoords[st + 9] = {coordMin.x, coordMax.y};
    texCoords[st + 10] = {coordMax.x, coordMax.y};
    texCoords[st + 11] = {coordMax.x, coordMin.y};

    texCoords[st + 12] = {coordMax.x, coordMax.y};
    texCoords[st + 13] = {coordMax.x, coordMin.y};
    texCoords[st + 14] = {coordMin.x, coordMin.y};
    texCoords[st + 15] = {coordMin.x, coordMax.y};

    texCoords[st + 16] = {coordMin.x, coordMin.y};
    texCoords[st + 17] = {coordMin.x, coordMax.y};
    texCoords[st + 18] = {coordMax.x, coordMax.y};
    texCoords[st + 19] = {coordMax.x, coordMin.y};

    texCoords[st + 20] = {coordMax.x, coordMin.y};
    texCoords[st + 21] = {coordMax.x, coordMax.y};
    texCoords[st + 22] = {coordMin.x, coordMax.y};
    texCoords[st + 23] = {coordMin.x, coordMin.y};
}

char Background::get_material(int x, int z) {
    if (x < -(size.x - 1) / 2 || z < -(size.y - 1) / 2)
        printf("Index out of bounds\n");
    x += (size.x - 1) / 2;
    z += (size.y - 1) / 2;
    return materials[x * size.y + z];
}
