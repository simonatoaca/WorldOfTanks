#pragma once

#include "lab_m1/tema2/tank.h"
#include <random>

namespace m1
{
	typedef enum { FORWARD, BACKWARD, ROTATE_LEFT, ROTATE_RIGHT } State;

	class TankEnemy : public Tank
	{
	public:
		TankEnemy(glm::vec3& center);

		float deltaTimeShooting;
		float start_time;
		void updatePosition();

	private:
		State currentState;
		float expirationTime;

		State getState()
		{
			std::random_device rd;

			// If the time didn't expire
			if (expirationTime - Engine::GetElapsedTime() > 0) {
				return currentState;
			}

			static std::mt19937 gen(rd());

			// Generate expiration time
			std::uniform_int_distribution<> genExpTime(1, 4);

			expirationTime = genExpTime(gen) + Engine::GetElapsedTime();

			// Generate next state
			std::uniform_int_distribution<> genState(0, 3);

			currentState = (State)genState(gen);
		}
	};
}
