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
        void LoadMeshes();

        void GenerateBuildings();
        void GenerateEnemies();

        bool AreColliding(Tank& t1, Tank& t2, float& distance);
        bool AreColliding(Tank& t, Projectile& p);
        bool AreColliding(Tank& t, Building& b, float& distance);
        bool AreColliding(Building& b, Projectile& p);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        player::Camera* camera;
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

        // The renderer object
        gfxc::TextRenderer* textRenderer;
    };
}   // namespace m1
