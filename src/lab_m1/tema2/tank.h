#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/tema2/projectile.h"

#include <vector>
#include <iostream>

#define TANK_COMPONENT_NUM 4

namespace m1
{
	typedef enum {TURRET, MACHINE_GUN, BODY, WHEELS} TankComponent;

	typedef enum {DARK_GREEN, LIGHT_GREEN, GRAY, DARK_ORANGE, LIGHT_ORANGE} TankColor;

	const glm::vec3 tankColors[] = { glm::vec3(0, 0.4f, 0), glm::vec3(0, 0.6f, 0.2f),
									 glm::vec3(0.61f, 0.61f, 0.61f),  glm::vec3(0.76, 0.42, 0.),
									 glm::vec3(1, 0.55, 0) };

	class Tank
	{
	public:
		Tank(glm::vec3& center);

		/* Position-related vars */
		glm::vec3 position;
		glm::vec3 forward[TANK_COMPONENT_NUM];
		glm::vec3 up[TANK_COMPONENT_NUM];
		glm::vec3 right[TANK_COMPONENT_NUM];
		float angleOY[TANK_COMPONENT_NUM];
		float gunAngleOX;
		float radius;
	
		/* Colors for each component of the tank */
		glm::vec3 color[TANK_COMPONENT_NUM];

		int HP;
		std::vector<Projectile> projectiles;
		float speed;

		void TranslateForward(float distance, TankComponent component);

		void MoveForward(float deltaTime);
		void MoveBackward(float deltaTime);

		void Rotate_OX(float angle, TankComponent component);
		void Rotate_OY(float angle, TankComponent component);
		void Rotate_OZ(float angle, TankComponent component);

		glm::mat4 GetModelMatrix(TankComponent component);
	};
}

