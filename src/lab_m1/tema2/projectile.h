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
			speed = 5;
			scale = 0.07;
			radius = 0.1; // ??????
			start_time = Engine::GetElapsedTime();
			ttl = 2 + Engine::GetElapsedTime();
		}

		glm::vec3 computePosition(float current_time)
		{
			float deltaTime = current_time - start_time;
			glm::vec3 position = initial_position;
			position += direction * (speed * deltaTime);

			/* The projectile disappears */
			if (position.y < 0) {
				ttl = 0;
			}

			return position;
		}

		glm::vec3 initial_position;
		glm::vec3 direction;
		float start_time;
		float speed;
		float scale;
		float radius;
		int ttl;
	};
}
