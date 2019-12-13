#pragma once

#include "background.h"
#include "cube.h"
#include "bullet.h"
#include <set>

#define TANK_ROTATION_SPEED 2.0f
#define TANK_MOVEMENT_SPEED 5.0f
#define FIRE_DELAY 150
#define REGEN_DELAY 400

typedef struct TankKeys {
    char up, down, left, right, fire;
} TankKeys;

class Tank {
public:
    bool isBlue;

    clock_t nextFire = 0;

    clock_t nextRegen = 0;

    float rotation = 0;

    int hp = 100;

    Cube head, body;

    std::vector<Bullet> bullets{};

    Tank(bool isBlue);

    void handleKeys(Background *bg, TankKeys *keys, float dTime);

    void fire();

    void damage();

    void render(int lightCount, glm::vec3 *lights, glm::vec3 *lightColors, glm::vec3 vPos, float rot);
};