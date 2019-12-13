#pragma once

#include "../utils/textures.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "element.h"
#include "background.h"

class BulletDrawer {
public:
    ppgso::Mesh mesh{"assets/bullet.obj"};
    GLuint tid;
    glm::mat4 modelMatrix{1.0f};
    ppgso::Shader program{diffuse_vert_glsl, diffuse_frag_glsl};
    glm::vec3 position{0, 0, 0};
    glm::vec3 rotation{0, 0, 0};
    glm::vec3 scale{1.0 / 80.0f, 1 / 80.0f, 1 / 80.0f};

    BulletDrawer();

    ~BulletDrawer();

    void draw(glm::vec3 pos, glm::vec3 view, float rot);

private:
    void update();

    glm::mat4 getViewMatrix(glm::vec3 vPos, float rot);
};

class Bullet {
public:
    glm::vec3 pos0;
    glm::vec3 pos;
    glm::vec3 speed;
    glm::vec3 acc;
    float t = 0;
    char *shooter;

    Bullet(char *shooter, glm::vec3 pos, glm::vec3 speed, glm::vec3 acc);

    double dist(glm::vec3 pos);
    char tick(char *blueTank, char *greenTank, Background *bg, float dTime);
};