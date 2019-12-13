//
// Created by Gyurix on 11/10/2019.
//
#include "element.h"

Element::Element() {
}

void Element::setup() {
    init();

    // Copy data to OpenGL
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Copy positions to gpu
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto position_attrib = program.getAttribLocation("Position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Set vertex program inputs
    // Copy texture positions to gpu
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

    auto texCoord_attrib = program.getAttribLocation("TexCoord");
    glEnableVertexAttribArray(texCoord_attrib);
    glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy indices to GPU
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(Face), mesh.data(), GL_STATIC_DRAW);
}

void Element::init() {
}

Element::~Element() {
    glDeleteTextures(1, &tid);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Element::updateTexCoords() {
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
}

void Element::update() {
    modelMatrix = glm::mat4{1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, {scale.x, scale.y, scale.z});
    modelMatrix = glm::rotate(modelMatrix, rotation.x, {1.0f, 0.0f, 0.0f});
    modelMatrix = glm::rotate(modelMatrix, -rotation.y, {0.0f, 1.0f, 0.0f});
    modelMatrix = glm::rotate(modelMatrix, rotation.z, {0.0f, 0.0f, 1.0f});
}

glm::mat4 Element::getViewMatrix(glm::vec3 vPos, float rot) {
    float y=vPos.y>2.5f?vPos.y+1:vPos.y;
    glm::vec3 eye(vPos.x - glm::sin(rot), y, vPos.z + glm::cos(rot));
    glm::vec3 center(vPos.x, vPos.y, vPos.z);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    return glm::lookAt(eye, center, up);
}

// Draw polygons
void Element::render(int lightCount, glm::vec3 *lights, glm::vec3 *lightColors, glm::vec3 vPos, float rot) {
    glm::mat4 view = getViewMatrix(vPos, rot);
    glm::mat4 projection = glm::perspective((ppgso::PI / 180.f) * 120.0f, 1.0f, 0.6f, 20.0f);
    program.use();
    program.setUniform("ProjectionMatrix", projection);
    program.setUniform("ViewMatrix", view);
    program.setUniform("ModelMatrix", modelMatrix);
    glUniform3fv(program.getUniformLocation("Light"), lightCount, &(lights[0].x));
    glUniform3fv(program.getUniformLocation("LightColor"), lightCount, &(lightColors[0].x));
    glUniform1iv(program.getUniformLocation("LightCount"), 1, &lightCount);
    glUniform1i(program.getUniformLocation("Texture"), 0);
    glActiveTexture((GLenum) (GL_TEXTURE0));
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tid);
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh.size() * 3, GL_UNSIGNED_INT, 0);
}

void Element::render2D(float zoom) {
    this->scale = {zoom, zoom, zoom};
    update();
    glm::mat4 view = getViewMatrix({0.0f, 0.0f, 0.0f}, 0.0f);
    glm::mat4 projection = glm::perspective((ppgso::PI / 180.f) * 90.0f, 1.0f, 0.01f, 20.0f);
    program.use();
    program.setUniform("ProjectionMatrix", projection);
    program.setUniform("ViewMatrix", view);
    program.setUniform("ModelMatrix", modelMatrix);
    glUniform1i(program.getUniformLocation("Texture"), 0);
    glActiveTexture((GLenum) (GL_TEXTURE0));
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tid);
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh.size() * 3, GL_UNSIGNED_INT, 0);
}