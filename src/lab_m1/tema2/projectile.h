#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "core/engine.h"

#include <vector>

namespace m1
{
	class Projectile
	{
	public:
		Projectile(glm::vec3 start_pos, glm::vec3 direction)
		{
			initial_position = start_pos;
			this->direction = direction;
			speed = glm::vec3(6 * direction.x, 6 * direction.y, 6 * direction.z);
			scale = 0.07f;
			radius = 0.2f;
			start_time = Engine::GetElapsedTime();
			ttl = 5 + Engine::GetElapsedTime();
		}

		glm::vec3 computePosition()
		{
			float deltaTime = Engine::GetElapsedTime() - start_time;
			glm::vec3 position = initial_position;
			
			speed.y -= 0.01 * deltaTime;
			position += glm::vec3(speed.x * deltaTime, speed.y * deltaTime, speed.z * deltaTime);

			/* The projectile disappears */
			if (position.y < 0) {
				ttl = 0;
			}

			return position;
		}

		glm::vec3 initial_position;
		glm::vec3 direction;
		float start_time;
		glm::vec3 speed;
		float scale;
		float radius;
		int ttl;
	};
}
