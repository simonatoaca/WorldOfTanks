#include "lab_m1/tema2/tank.h"

using namespace m1;
using namespace std;

Tank::Tank(glm::vec3& center)
{
	for (auto& component : { TURRET, MACHINE_GUN, BODY, WHEELS }) {
		forward[component] = glm::vec3(0, 0, -1);
		up[component] = glm::vec3(0, 1, 0);
		right[component] = glm::vec3(1, 0, 0);
		angleOY[component] = 0;
	}

	position = center;
	gunAngleOX = 0;

	color[BODY] = tankColors[DARK_GREEN];
	color[TURRET] = tankColors[LIGHT_GREEN];
	color[MACHINE_GUN] = tankColors[GRAY];
	color[WHEELS] = tankColors[GRAY];

	HP = 3;
	numLives = 5;
	deltaTimeShooting = Engine::GetElapsedTime();
	radius = 1; // One unit in the grid
	speed = 2.f;
}

void Tank::TranslateForward(float distance, TankComponent component)
{
	position += forward[component] * distance;
}

void Tank::MoveForward(float deltaTime)
{
	float distance = deltaTime * speed;
	glm::vec3 dir = glm::normalize(glm::vec3(forward[BODY].x, 0, forward[BODY].z));
	position += dir * distance;
}

void Tank::MoveBackward(float deltaTime)
{
	float distance = -deltaTime * speed;
	glm::vec3 dir = glm::normalize(glm::vec3(forward[BODY].x, 0, forward[BODY].z));
	position += dir * distance;
}

void Tank::Rotate_OX(float angle, TankComponent component)
{
	gunAngleOX += angle;
	gunAngleOX = gunAngleOX < 0 ? 0 : gunAngleOX > 60 ? 60 : gunAngleOX;
	/*forward[component] = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, right[component]) *
						 glm::vec4(forward[component], 1)));*/
						 //up[component] = glm::cross(right[component], forward[component]);
}

void Tank::Rotate_OY(float angle, TankComponent component)
{
	forward[component] = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, up[component]) *
		glm::vec4(forward[component], 1)));
	right[component] = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, up[component]) *
		glm::vec4(right[component], 1)));
	up[component] = glm::cross(right[component], forward[component]);
}

void Tank::Rotate_OZ(float angle, TankComponent component)
{
	right[component] = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, forward[component]) * glm::vec4(right[component], 1)));
	up[component] = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, forward[component]) * glm::vec4(up[component], 1)));
}

void Tank::Rotate(float angle)
{
	Rotate_OY(angle, BODY);
	Rotate_OY(angle, TURRET);
	Rotate_OY(angle, WHEELS);
	Rotate_OY(angle, MACHINE_GUN);
}

void Tank::FollowDir(glm::vec3& dir, TankComponent component)
{
	forward[component] = dir;
	right[component] = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), RADIANS(-90), up[component]) *
		glm::vec4(forward[component], 1)));
	up[component] = glm::cross(right[component], forward[component]);
}

glm::mat4 Tank::GetModelMatrix(TankComponent component)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);

	angleOY[component] = -atan2(right[component].z, right[component].x);

	if (component == MACHINE_GUN) {
		//gunAngleOX = atan2(forward[component].y, glm::length(glm::vec2(forward[component].x, forward[component].z)));
		//gunAngleOX = gunAngleOX > RADIANS(45) ? RADIANS(45) : (gunAngleOX < RADIANS(0) ? RADIANS(0) : gunAngleOX);

		//std::cout << "Angle OY: " << DEGREES(angleOY[component]) << " Angle OX: " << DEGREES(gunAngleOX) << std::endl;

		/* Relative position to the tank's center */
		glm::vec3 gun_rel_pos = glm::vec3(0.25 * cos(angleOY[component] + RADIANS(90)), 0.56,
			0.25 * sin(angleOY[component] - RADIANS(90)));

		modelMatrix = glm::translate(modelMatrix, gun_rel_pos);
		modelMatrix = glm::rotate(modelMatrix, gunAngleOX, right[component]);
		modelMatrix = glm::translate(modelMatrix, -gun_rel_pos);
	}

	modelMatrix = glm::rotate(modelMatrix, angleOY[component], up[component]);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7));

	return modelMatrix;
}
