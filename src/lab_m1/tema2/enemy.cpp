#include "lab_m1/tema2/enemy.h"

using namespace m1;

TankEnemy::TankEnemy(glm::vec3& center) : Tank(center)
{
	deltaTimeShooting = 2.f + Engine::GetElapsedTime();
	start_time = Engine::GetElapsedTime();
	color[BODY] = tankColors[DARK_ORANGE];
	color[TURRET] = tankColors[LIGHT_ORANGE];
	currentState = FORWARD;
	expirationTime = -1 + Engine::GetElapsedTime();
	speed = 1.f; // Slower than the player's tank
}

void TankEnemy::updatePosition()
{
	float deltaTime = Engine::GetElapsedTime() - start_time;
	start_time = Engine::GetElapsedTime();

	switch (getState()) {
		case FORWARD:
			MoveForward(deltaTime);
			break;
		case BACKWARD:
			MoveBackward(deltaTime);
			break;
		case ROTATE_LEFT:
			Rotate(deltaTime * speed);
			break;
		case ROTATE_RIGHT:
			Rotate(-deltaTime * speed);
			break;
	}
}