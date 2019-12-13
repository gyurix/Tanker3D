#include "bullet.h"
#include "tank.h"

BulletDrawer::BulletDrawer() {
    tid = load_texture("assets/bullet.bmp");
}

BulletDrawer::~BulletDrawer() {
    glDeleteTextures(1, &tid);
}

glm::mat4 BulletDrawer::getViewMatrix(glm::vec3 vPos, float rot) {
    float y=vPos.y>2.5f?vPos.y+1:vPos.y;
    glm::vec3 eye(vPos.x - glm::sin(rot), y, vPos.z + glm::cos(rot));
    glm::vec3 center(vPos.x, vPos.y, vPos.z);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    return glm::lookAt(eye, center, up);
}

void BulletDrawer::update() {
    modelMatrix = glm::mat4{1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, {scale.x, scale.y, scale.z});
    modelMatrix = glm::rotate(modelMatrix, rotation.z, {0.0f, 0.0f, 1.0f});
}

void BulletDrawer::draw(glm::vec3 pos, glm::vec3 vPos, float rot) {
    position = pos;
    update();
    glm::mat4 projection = glm::perspective((ppgso::PI / 180.f) * 120.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 view = getViewMatrix(vPos, rot);
    program.use();
    program.setUniform("ProjectionMatrix", projection);
    program.setUniform("ViewMatrix", view);
    program.setUniform("ModelMatrix", modelMatrix);
    auto unifLoc = program.getUniformLocation("Texture");
    glUniform1i(unifLoc, 0);
    glActiveTexture((GLenum) (GL_TEXTURE0));
    glBindTexture(GL_TEXTURE_2D, tid);
    mesh.render();
}

Bullet::Bullet(char *shooter, glm::vec3 pos, glm::vec3 speed, glm::vec3 acc) {
    this->shooter = shooter;
    this->pos = this->pos0 = pos;
    this->speed = speed;
    this->acc = acc;
}

double Bullet::dist(glm::vec3 p) {
    return sqrt((p.x - pos.x) * (p.x - pos.x) + (p.y - pos.y) * (p.y - pos.y) + (p.z - pos.z) * (p.z - pos.z));
}

char Bullet::tick(char *blueTank, char *greenTank, Background *bg, float dTime) {
    t += dTime * 5;
    pos = pos0 + speed * t + acc * 0.5f * t * t;
    if (shooter == greenTank && dist(((Tank*)blueTank)->body.position) < 0.7f) {
        ((Tank*)blueTank)->damage();
        return 0;
    }
    if (shooter == blueTank && dist(((Tank*)greenTank)->body.position) < 0.7f) {
        ((Tank*)greenTank)->damage();
        return 0;
    }
    if (pos.y < -1)
        return 0;
    if (!bg->canBreak(pos.x, pos.z) || bg->destroyMap(pos.x, pos.z))
        return 0;
    return 1;
}