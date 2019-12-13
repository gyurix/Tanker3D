#include "tank.h"


static GLuint tankTexture = 99999999;

Tank::Tank(const bool isBlue) {
    head.tid = tankTexture == 99999999 ? tankTexture = load_texture("assets/textures.bmp") : tankTexture;
    body.tid = head.tid;
    head.setTextureY(0.625f - (isBlue ? 0.25f : 0.0f));
    head.pos1 = {-0.1f, -0.1f, -0.2f};
    head.pos2 = {0.1f, 0.1f, 0.2f};
    body.setTextureY(0.75f - (isBlue ? 0.25f : 0.0f));
    body.pos1 = {-0.15f, -0.5f, -0.3f};
    body.pos2 = {0.15f, -0.1f, 0.3f};
    head.setup();
    body.setup();
    this->isBlue = isBlue;
}

void Tank::handleKeys(Background *bg, TankKeys *keys, float dTime) {
    clock_t t = clock();
    if (t > nextRegen && hp < 100) {
        ++hp;
        nextRegen = t + REGEN_DELAY;
    }
    int up = keys->up - keys->down;
    int side = keys->right - keys->left;
    float rot = side * TANK_ROTATION_SPEED * dTime;
    rotation = ppgso::PI * 2 + rotation + rot;
    while (rotation > ppgso::PI)
        rotation -= ppgso::PI * 2;
    float move = up * TANK_MOVEMENT_SPEED * dTime;
    glm::vec2 dest{body.position.x + move * glm::sin(rotation), body.position.z - move * glm::cos(rotation)};
    if (bg->canBreak(dest.x, body.position.z))
        body.position.x = dest.x;
    if (bg->canBreak(body.position.x, dest.y))
        body.position.z = dest.y;
    head.rotation.y = rotation;
    head.position = body.position;
    head.update();
    if (up) {
        body.rotation.y = rotation;
        body.update();
    }
    bg->destroyMap(body.position.x, body.position.z);
    if (keys->fire)
        fire();
}

void Tank::render(int lightCount, glm::vec3 *lights, glm::vec3 *lightColors, glm::vec3 vPos, float rot) {
    head.render(0, lights, lightColors, vPos, rot);
    body.render(0, lights, lightColors, vPos, rot);
}

void Tank::fire() {
    clock_t cl = clock();
    if (nextFire > cl)
        return;
    nextFire = cl + FIRE_DELAY;
    glm::vec3 speed = {-glm::sin(rotation), 0, glm::cos(rotation)};
    glm::vec3 pos = body.position - 0.5f * speed;
    pos.y += 0.1f;
    glm::vec3 acc = {0.00015f * (rand() % 2000 - 1000), -0.05f, 0.00015f * (rand() % 2000 - 1000)};
    Bullet b{(char*)this, pos, -speed, acc};
    bullets.push_back(b);
}

void Tank::damage() {
    this->hp -= 25;
    if (this->hp <= 0) {
        this->hp = 0;
    }
}
