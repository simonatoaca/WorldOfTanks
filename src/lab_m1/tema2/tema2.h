#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "lab_m1/tema2/player_camera.h"
#include "lab_m1/tema2/tank.h"
#include "lab_m1/tema2/enemy.h"
#include "lab_m1/tema2/building.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderTank(Tank &tank);
        void RenderBuilding(Building& building);
        void RenderTankComponent(Tank& tank, Mesh* mesh, Shader* shader, TankComponent component);
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3 color);
        void RenderAll();
        void LoadMeshes();

        void GenerateBuildings();
        void GenerateEnemies();

        bool AreColliding(Tank& t1, Tank& t2, float& distance);
        bool AreColliding(Tank& t, Projectile& p);
        bool AreColliding(Tank& t, Building& b, float& distance);
        bool AreColliding(Building& b, Projectile& p);

        void EnemyAttacks();
        void ProjectileCollisions();
        void TankCollisions();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        bool gameOver;

        player::Camera* camera;
        player::Camera* perspectiveCamera;
        player::Camera* orthoCamera;
        glm::mat4 projectionMatrix;

        Tank* tank;
        unsigned int score;
        std::vector<TankEnemy> enemies;
        std::vector<Building> buildings;

        // Time-related variables in seconds
        float totalGameTime;
        float projectileShootTime;

        float fov;
        float zNear, zFar;
        float right, left, bottom, top;

        float mapBoundXmax;
        float mapBoundXmin;
        float mapBoundZmax;
        float mapBoundZmin;

        // The renderer object
        gfxc::TextRenderer* textRenderer;

        ViewportArea miniViewportArea;
    };
}   // namespace m1
