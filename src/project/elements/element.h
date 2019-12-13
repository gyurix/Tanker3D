#pragma once

#include <ppgso.h>
#include <shader.h>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "../utils/textures.h"
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>

class Element {
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    struct Face {
        GLuint v0, v1, v2;
    };
    std::vector<Face> mesh;
    GLuint vao, vbo, tbo, ibo, tid;
    glm::mat4 modelMatrix{1.0f};
    ppgso::Shader program{diffuse_vert_glsl, diffuse_frag_glsl};
    glm::vec3 position{0, 0, 0};
    glm::vec3 rotation{0, 0, 0};
    glm::vec3 scale{1, 1, 1};

    Element();

    ~Element();

    virtual void init();

    virtual void setup();

    virtual void updateTexCoords();

    virtual void update();

    virtual glm::mat4 getViewMatrix(glm::vec3 vPos, float rot);

    void render(int lightCount, glm::vec3 *lights, glm::vec3 *lightColors, glm::vec3 vPos, float rot);

    void render2D(float zoom);
};
