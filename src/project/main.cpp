#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include "utils/textures.h"
#include "elements/cube.h"
#include "elements/tank.h"
#include <algorithm>
#include "elements/rectangle.h"

struct PressedKeys {
    TankKeys blue, green;
} pressedKeys;

#define MAIN_SCREEN 0
#define GAME_SCREEN 1
#define BLUE_WIN_SCREEN 2
#define GREEN_WIN_SCREEN 3

class TunelerGameWindow : public ppgso::Window {
private:
    Tank blueTank{true};
    Tank greenTank{false};
    BulletDrawer bulletDrawer;
    Background bg{{45, 45}};
    float lastTime = 0.0f;
    int lightCount;
    clock_t zoomStart;
    char viewMode = 0;
    glm::vec3 lights[100];
    glm::vec3 lightColors[100];
    Rectangle blueWinScreen{{0.125f, 0.25f},
                            {0.25f,  0.375f}};
    Rectangle greenWinScreen{{0.25f,  0.25f},
                             {0.375f, 0.375f}};
    Rectangle mainScreen{{0.375f, 0.25f},
                         {0.5f,   0.375f}};
    Rectangle hpBar{{0.0f,   0.25f},
                    {0.125f, 0.375f}};
    char screen = MAIN_SCREEN;
public:
    TunelerGameWindow() : Window{"Tuneler3D by Juraj Barath", 1100, 700} {
        hideCursor();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_ALPHA);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthFunc(GL_LESS);
        //rect.setup();
        bg.setup();
        lightColors[0] = {1.0f, 1.0f, 1.0f};
        lightColors[1] = {1.0f, 1.0f, 1.0f};
        for (int i = 2; i < 100; ++i)
            lightColors[i] = {2.0f, 2.0f, 1.0f};
        mainScreen.tid = blueTank.body.tid;
        blueWinScreen.tid = blueTank.body.tid;
        greenWinScreen.tid = blueTank.body.tid;
        hpBar.tid = blueTank.body.tid;
        mainScreen.setup();
        blueWinScreen.setup();
        greenWinScreen.setup();
        hpBar.setup();
        resetKeys();
    }

    void resetKeys() {
        pressedKeys.blue = {false, false, false, false, false};
        pressedKeys.green = {false, false, false, false, false};
    }

    void startGame() {
        resetKeys();
        bg.gen_background();
        bg.updateTexCoords();
        greenTank.hp = 100;
        blueTank.hp = 100;
        blueTank.body.position = {rand() % (bg.size.x - 3) - bg.size.x / 2.0f + 1.5f, 0.0f,
                                  rand() % (bg.size.y - 3) - bg.size.y / 2.0f + 1.5f};
        blueTank.head.position = blueTank.body.position;
        blueTank.body.update();

        greenTank.body.position = {rand() % (bg.size.x - 2) - bg.size.x / 2.0f + 1.5f, 0.0f,
                                   rand() % (bg.size.y - 2) - bg.size.y / 2.0f + 1.5f};
        greenTank.head.position = greenTank.body.position;
        greenTank.body.update();
        for (int x = -1; x <= 1; ++x) {
            for (int z = -1; z <= 1; ++z) {
                bg.set_material(blueTank.body.position.x + x, blueTank.body.position.z + z, DIRT);
                bg.set_material(greenTank.body.position.x + z, greenTank.body.position.z + z, DIRT);
            }
        }
        screen = GAME_SCREEN;
    }

    void renderTankScreen(Tank *tank) {
        glm::vec3 offset = {0.0f, viewMode == 0 ? 0.0f : viewMode == 1 ? 1.0f : 3.0f, 0.0f};
        float rot = tank->rotation;
        if (screen != GAME_SCREEN) {
            float o_y = (clock() - zoomStart) / 1000.0f;
            if (o_y > 5.0f)
                rot += (o_y - 5) / 10.0f;
            if (o_y > 2.0f) {
                offset.x = o_y - 2;
                offset.z = o_y - 1;
                o_y = 2.0f;
            }
            offset.y = o_y;
        }
        glm::vec3 pos = tank->body.position + offset;
        bg.render(lightCount, lights, lightColors, pos, rot);
        blueTank.render(lightCount, lights, lightColors, pos, rot);
        greenTank.render(lightCount, lights, lightColors, pos, rot);
        for (auto b : blueTank.bullets) {
            bulletDrawer.draw(b.pos, pos, rot);
        }
        for (auto b : greenTank.bullets)
            bulletDrawer.draw(b.pos, pos, rot);
    }

    void onIdle() final {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (screen != MAIN_SCREEN) {
            // Move and Render shape
            float time = (float) glfwGetTime();
            float dTime = time - lastTime;

            lastTime = time;
            if (screen == GAME_SCREEN) {
                blueTank.handleKeys(&bg, &pressedKeys.blue, dTime);
                greenTank.handleKeys(&bg, &pressedKeys.green, dTime);
            }
            std::vector<Bullet> left{};
            for (std::vector<Bullet>::iterator it = blueTank.bullets.begin(); it != blueTank.bullets.end(); ++it) {
                if (it->tick((char *) &blueTank, (char *) &greenTank, &bg, dTime))
                    left.push_back(*it);
            }
            blueTank.bullets.clear();
            blueTank.bullets.insert(blueTank.bullets.end(), left.begin(), left.end());
            left.clear();
            for (std::vector<Bullet>::iterator it = greenTank.bullets.begin(); it != greenTank.bullets.end(); ++it)
                if (it->tick((char *) &blueTank, (char *) &greenTank, &bg, dTime))
                    left.push_back(*it);
            greenTank.bullets.clear();
            greenTank.bullets.insert(greenTank.bullets.end(), left.begin(), left.end());

            lightCount = 2 + (int) greenTank.bullets.size() + (int) blueTank.bullets.size();
            lights[0] = blueTank.body.position;
            lights[1] = greenTank.body.position;
            unsigned long long offset = 2;
            unsigned long long len = blueTank.bullets.size();
            for (unsigned long long i = 0; i < len; ++i)
                lights[i + offset] = blueTank.bullets[i].pos;
            offset += len;
            len = greenTank.bullets.size();
            for (unsigned long long i = 0; i < len; ++i)
                lights[i + offset] = greenTank.bullets[i].pos;
            glViewport(20, 180, 500, 500);
            renderTankScreen(&blueTank);
            glViewport(20, 50, 500, 80);
            hpBar.position.x = -(100 - blueTank.hp) * 0.02f;
            hpBar.render2D(1.0f);

            glViewport(540, 180, 500, 500);
            renderTankScreen(&greenTank);
            glViewport(540, 50, 500, 80);
            hpBar.position.x = -(100 - greenTank.hp) * 0.02f;
            hpBar.render2D(1.0f);

            if (screen == GAME_SCREEN) {
                if (greenTank.hp == 0) {
                    screen = BLUE_WIN_SCREEN;
                    zoomStart = clock();
                } else if (blueTank.hp == 0) {
                    screen = GREEN_WIN_SCREEN;
                    zoomStart = clock();
                }
            }
        }
        if (screen == MAIN_SCREEN) {
            glViewport(200, 0, 700, 700);
            mainScreen.render2D(1.0f);
        } else if (screen == BLUE_WIN_SCREEN) {
            glClear(GL_DEPTH_BUFFER_BIT);
            glViewport(200, 0, 700, 700);
            blueWinScreen.render2D(getZoom());
        } else if (screen == GREEN_WIN_SCREEN) {
            glClear(GL_DEPTH_BUFFER_BIT);
            glViewport(200, 0, 700, 700);
            greenWinScreen.render2D(getZoom());
        }
    }

    float getZoom() {
        float zoom = (clock() - zoomStart) / 1000.0f;
        return zoom > 1 ? 1.0f : zoom;
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        if (action != GLFW_PRESS && action != GLFW_RELEASE)
            return;
        if (action == GLFW_PRESS && key == GLFW_KEY_SPACE && screen != GAME_SCREEN) {
            if (screen == MAIN_SCREEN)
                startGame();
            else
                screen = MAIN_SCREEN;
        }
        if (screen != GAME_SCREEN)
            return;
        switch (key) {
            case GLFW_KEY_W:
                pressedKeys.blue.up = action == GLFW_PRESS;
                break;
            case GLFW_KEY_S:
                pressedKeys.blue.down = action == GLFW_PRESS;
                break;
            case GLFW_KEY_A:
                pressedKeys.blue.left = action == GLFW_PRESS;
                break;
            case GLFW_KEY_D:
                pressedKeys.blue.right = action == GLFW_PRESS;
                break;
            case GLFW_KEY_X:
                pressedKeys.blue.fire = action == GLFW_PRESS;
                break;
            case GLFW_KEY_I:
                if (action == GLFW_PRESS) {
                    printf("INFO:\n");
                    printf("Blue Tank: {pos = %g %g, hp = %d, rot = %g, mat = %c}\n", blueTank.body.position.x,
                           blueTank.body.position.z,
                           blueTank.hp, blueTank.rotation,
                           bg.print_material(bg.get_material(blueTank.body.position.x, blueTank.body.position.z)));
                    printf("Green Tank: {pos = %g %g, hp = %d, rot = %g, mat = %c}\n", greenTank.body.position.x,
                           greenTank.body.position.z,
                           greenTank.hp, greenTank.rotation,
                           bg.print_material(bg.get_material(greenTank.body.position.x, greenTank.body.position.z)));
                }
                break;
            case GLFW_KEY_V:
                if (action == GLFW_PRESS) {
                    viewMode = (viewMode + 1) % 3;
                }
                break;
            case GLFW_KEY_UP:
                pressedKeys.green.up = action == GLFW_PRESS;
                break;
            case GLFW_KEY_DOWN:
                pressedKeys.green.down = action == GLFW_PRESS;
                break;
            case GLFW_KEY_LEFT:
                pressedKeys.green.left = action == GLFW_PRESS;
                break;
            case GLFW_KEY_RIGHT:
                pressedKeys.green.right = action == GLFW_PRESS;
                break;
            case GLFW_KEY_KP_0:
                pressedKeys.green.fire = action == GLFW_PRESS;
                break;
        }
    }
};

int main() {
    puts("Starting Tuneler3D, please wait...");
    auto window = TunelerGameWindow{};
    puts("Startup complete, enjoy the game.");
    while (window.pollEvents()) {}
    return EXIT_SUCCESS;
}
