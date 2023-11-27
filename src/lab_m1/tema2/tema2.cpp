#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/tank.h"
#include "lab_m1/tema2/enemy.h"
#include "lab_m1/tema2/projectile.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

void Tema2::LoadMeshes()
{
    {
        Mesh* mesh = new Mesh("tank_body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks"), "tank_body.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks"), "turret.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("gun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks"), "gun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("wheels");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks"), "wheels.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("terrain");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("projectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
}

void Tema2::GenerateBuildings()
{
    int buildingNumber = 0;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate number of buildings
    std::uniform_int_distribution<> genBuildingNo(6, 10);

    buildingNumber = genBuildingNo(gen);

    // Generate position + scale
    for (int i = 0; i < buildingNumber; i++) {
        std::uniform_real_distribution<> genPosition(-23, 23);
        std::uniform_real_distribution<> genScale(2, 5);

        glm::vec3 position = glm::vec3(genPosition(gen), 0, genPosition(gen));
        glm::vec3 scale = glm::vec3(genScale(gen), genScale(gen), genScale(gen));

        Building b(position, scale);
        buildings.push_back(b);
    }
}

void Tema2::GenerateEnemies()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    int enemyNumber = 0;

    // Generate number of enemies
    std::uniform_int_distribution<> genEnemyNo(4, 6);

    enemyNumber = genEnemyNo(gen);

    // Generate position
    for (int i = 0; i < enemyNumber; i++) {
        std::uniform_real_distribution<> genPosition(-23, 23);
        glm::vec3 position = glm::vec3(genPosition(gen), 0, genPosition(gen));

        TankEnemy enemy(position);
        enemies.push_back(enemy);
    }
}

void Tema2::Init()
{
    tank = new Tank(glm::vec3(0, 0, 0));

    GenerateBuildings();
    GenerateEnemies();
 
    perspectiveCamera = new player::Camera();
    perspectiveCamera->FollowTarget(tank->position, 0);

    orthoCamera = new player::Camera(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), true);
    orthoCamera->FollowTarget(tank->position, 0);

    camera = perspectiveCamera;

    totalGameTime = 80 + Engine::GetElapsedTime();
    score = 0;

    top = 4.0f;
    bottom = -top;
    right = top * window->props.aspectRatio;
    left = -right;

    fov = RADIANS(80);
    zNear = 0.01f;
    zFar = 200.0f;

    LoadMeshes();

    {
        Shader* shader = new Shader("TankShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2",
                                            "shaders", "VertexShaderTank.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2",
                                            "shaders", "FragmentShaderTank.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("PlainShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2",
                                            "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders",
                                            "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load font for text
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "game1.ttf"), 18);

    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    // Cursor at gun
    window->SetPointerPosition(resolution.x / 2, resolution.y / 2 - 100);
}

void Tema2::RenderBuilding(Building& building)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, building.position);
    modelMatrix = glm::scale(modelMatrix, building.scale);
    RenderMesh(meshes["box"], shaders["PlainShader"], modelMatrix, building.color);
}

void Tema2::RenderTank(Tank &tank)
{
    RenderTankComponent(tank, meshes["tank_body"], shaders["TankShader"], BODY);
    RenderTankComponent(tank, meshes["turret"], shaders["TankShader"], TURRET);
    RenderTankComponent(tank, meshes["gun"], shaders["TankShader"], MACHINE_GUN);
    RenderTankComponent(tank, meshes["wheels"], shaders["TankShader"], WHEELS);

    /* Print projectiles */
    for (auto& projectile = tank.projectiles.begin(); projectile != tank.projectiles.end(); projectile++) {
        /* Erase projectile if its ttl expired */
        if (projectile->ttl - Engine::GetElapsedTime() <= 0) {
            projectile = tank.projectiles.erase(projectile);
        }

        if (projectile == tank.projectiles.end()) {
            break;
        }

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, projectile->computePosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(projectile->scale));
        RenderMesh(meshes["projectile"], shaders["PlainShader"], modelMatrix, glm::vec3(0, 0, 0));
    }
}

bool Tema2::AreColliding(Tank& t1, Tank& t2, float &distance)
{
    distance = abs(t1.radius + t2.radius - glm::distance(t1.position, t2.position));
    return (glm::distance(t1.position, t2.position) < t1.radius + t2.radius);
}

bool Tema2::AreColliding(Tank& t, Projectile& p)
{
    return (glm::distance(t.position, p.computePosition()) < t.radius + p.radius);
}

bool Tema2::AreColliding(Tank& t, Building& b, float& distance)
{
    float xmin = b.position.x - b.radius.x;
    float xmax = b.position.x + b.radius.x;

    float zmin = b.position.z - b.radius.z;
    float zmax = b.position.z + b.radius.z;

    float edgeX = b.position.x;
    float edgeZ = b.position.z;

    if (t.position.x > xmax)  edgeX = xmax;  // The tank is to the right
    if (t.position.x < xmin)  edgeX = xmin;  // The tank is to the left

    if (t.position.z > zmax)  edgeZ = zmax;  // The tank is above
    if (t.position.z < zmin)  edgeZ = zmin;  // The tank is below

    float distX = t.position.x - edgeX;
    float distZ = t.position.z - edgeZ;

    if (sqrt(distX * distX + distZ * distZ) < t.radius) {
        distance = t.radius - sqrt(distX * distX + distZ * distZ);
        return true;
    }

    return false;
}

bool Tema2::AreColliding(Building& b, Projectile& p)
{
    glm::vec3 projectile_pos = p.computePosition();
    
    // The projectile is over the building
    if (projectile_pos.z > b.position.z + b.radius.z) {
        return false;
    }

    float xmin = b.position.x - b.radius.x;
    float xmax = b.position.x + b.radius.x;

    float zmin = b.position.z - b.radius.z;
    float zmax = b.position.z + b.radius.z;

    float edgeX = b.position.x;
    float edgeZ = b.position.z;

    if (projectile_pos.x > xmax)  edgeX = xmax;
    if (projectile_pos.x < xmin)  edgeX = xmin;

    if (projectile_pos.z > zmax)  edgeZ = zmax;
    if (projectile_pos.z < zmin)  edgeZ = zmin;

    float distX = projectile_pos.x - edgeX;
    float distZ = projectile_pos.z - edgeZ;

    if (sqrt(distX * distX + distZ * distZ) < p.radius) {
        return true;
    }

    return false;
}

void Tema2::RenderAll()
{
    RenderTank(*tank);

    // Render enemies
    for (auto& enemy = enemies.begin(); enemy != enemies.end(); enemy++) {
        if (enemy->HP <= 0) {
            enemy->speed = 0;
        }

        RenderTank(*enemy);
    }

    // Render buildings
    for (auto& building : buildings) {
        RenderBuilding(building);
    }

    // Render terrain
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMesh(meshes["terrain"], shaders["PlainShader"], modelMatrix, glm::vec3(0.27, 0.83, 0.42));
    }
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.8, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
    if (totalGameTime - Engine::GetElapsedTime() <= 0 || tank->numLives <= 0) {
        textRenderer->RenderText(std::string("Game Over"), 450, 320, 1.5f, glm::vec3(0, 1, 0));
        textRenderer->RenderText(std::string("Total Score: ") + to_string(score), 450, 350, 1.5f, glm::vec3(0, 1, 0));
        return;
    }

    camera = perspectiveCamera;
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);

    /* ENEMY ATTACKS */
    for (auto& enemy : enemies) {
        if (!enemy.HP) {
            continue;
        }

        if (glm::distance(tank->position, enemy.position) < 7) {
            glm::vec3 dir = glm::normalize(tank->position - enemy.position);

            if (dir != enemy.forward[TURRET]) {
                enemy.FollowDir(dir, TURRET);
                enemy.FollowDir(dir, MACHINE_GUN);
            }

            if (enemy.deltaTimeShooting - Engine::GetElapsedTime() < 0) {
                glm::vec3 gun_rel_pos = glm::vec3(0, 0.56, 0) + enemy.forward[MACHINE_GUN];
                enemy.projectiles.push_back(Projectile(enemy.position + gun_rel_pos, enemy.forward[MACHINE_GUN]));

                enemy.deltaTimeShooting = 2.f + Engine::GetElapsedTime();
            }
        }
    }

    /* COLLISIONS */

    // Tank - Tank collisions
    for (auto& enemy : enemies) {
        float dist = 0;
        if (AreColliding(*tank, enemy, dist)) {
            // Move tanks away from eachother so they don't intersect anymore
            glm::vec3 P =  dist * glm::normalize(tank->position - enemy.position);
            tank->position += P * 0.5f;
            enemy.position -= P * 0.5f;
        }
    }

    // Projectile - Enemy collisions
    for (auto& projectile = tank->projectiles.begin(); projectile != tank->projectiles.end(); projectile++) {
        for (auto& enemy : enemies) {
            if (AreColliding(enemy, *projectile)) {
                enemy.HP--;

                if (enemy.HP == 0) {
                    score++;
                }

                enemy.HP = enemy.HP < 0 ? 0 : enemy.HP;

                projectile = tank->projectiles.erase(projectile);

                if (projectile == tank->projectiles.end()) {
                    break;
                }
            }
        }
        break;
    }

    // Enemy projectiles - Tank collisions
    for (auto& enemy : enemies) {
        for (auto& projectile = enemy.projectiles.begin(); projectile != enemy.projectiles.end(); projectile++) {
            if (AreColliding(*tank, *projectile)) {
                tank->numLives--;

                if (tank->numLives <= 0) {
                    tank->speed = 0;
                }

                projectile = enemy.projectiles.erase(projectile);

                if (projectile == enemy.projectiles.end()) {
                    break;
                }
            }
        }
    }

    // Tank - Building collisions
    for (auto& building : buildings) {
        float dist = 0;
        if (AreColliding(*tank, building, dist)) {
            // Taking into consideration only coordinates from the XOZ plane
            glm::vec2 P = dist * glm::normalize(glm::vec2(tank->position.x, tank->position.z) 
                                              - glm::vec2(building.position.x, building.position.z));
            tank->position += glm::vec3(P.x, 0, P.y) * 2.f;
        }

        // Enemy collisions
        for (auto& enemy : enemies) {
            if (AreColliding(enemy, building, dist)) {
                // Taking into consideration only coordinates from the XOZ plane
                glm::vec2 P = dist * glm::normalize(glm::vec2(enemy.position.x, enemy.position.z)
                    - glm::vec2(building.position.x, building.position.z));
                enemy.position += glm::vec3(P.x, 0, P.y) * 2.f;
            }
        }
    }

    // Projectile - Building collisions
    for (auto& projectile = tank->projectiles.begin(); projectile != tank->projectiles.end(); projectile++) {
        for (auto& building : buildings) {
            if (AreColliding(building, *projectile)) {
                projectile = tank->projectiles.erase(projectile);

                if (projectile == tank->projectiles.end()) {
                    break;
                }
            }
        }
        break;
    }

    /* RENDERING */
    
    RenderAll();

    // MiniViewport
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    camera = orthoCamera;
    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);

    RenderAll();
}

void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderTankComponent(Tank& tank, Mesh* mesh, Shader* shader, TankComponent component)
{
    if (!mesh || !shader || !shader->program)
        return;

    glm::mat4 modelMatrix = tank.GetModelMatrix(component);

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int location = glGetUniformLocation(shader->program, "Color");
    glUniform3fv(location, 1, glm::value_ptr(tank.color[component]));

    location = glGetUniformLocation(shader->program, "HP");
    glUniform1i(location, tank.HP);

    mesh->Render();
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3 color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
    int location = glGetUniformLocation(shader->program, "Color");
    glUniform3fv(location, 1, glm::value_ptr(color));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) {
        tank->MoveForward(deltaTime);
        perspectiveCamera->FollowTarget(tank->position, 0);
        orthoCamera->FollowTarget(tank->position, 0);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        tank->Rotate(deltaTime * tank->speed);
        perspectiveCamera->FollowTarget(tank->position, -deltaTime * tank->speed);
        orthoCamera->FollowTarget(tank->position, -deltaTime * tank->speed);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        tank->MoveBackward(deltaTime);
        perspectiveCamera->FollowTarget(tank->position, 0);
        orthoCamera->FollowTarget(tank->position, 0);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
         tank->Rotate(-deltaTime * tank->speed);
         perspectiveCamera->FollowTarget(tank->position, deltaTime * tank->speed);
         orthoCamera->FollowTarget(tank->position, deltaTime * tank->speed);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.002f;
    float sensivityOY = 0.001f;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        perspectiveCamera->RotateThirdPerson_OX(deltaY * sensivityOX);
        perspectiveCamera->RotateThirdPerson_OY(deltaX * sensivityOY);
    }
    else 
    {
        //tank->Rotate_OX(-deltaY * sensivityOY, MACHINE_GUN);
        tank->Rotate_OY(-deltaX * sensivityOX, TURRET);
        tank->Rotate_OY(-deltaX * sensivityOX, MACHINE_GUN);
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && tank->deltaTimeShooting - Engine::GetElapsedTime() <= 0) {
        /* Relative front of the gun position */
        glm::vec3 gun_rel_pos = glm::vec3(0, 0.56, 0) + tank->forward[MACHINE_GUN];
        tank->projectiles.push_back(Projectile(tank->position + gun_rel_pos, tank->forward[MACHINE_GUN]));
        tank->deltaTimeShooting = 1.5f + Engine::GetElapsedTime();
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
