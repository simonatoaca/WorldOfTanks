#include "lab_m1/tema2/enemy.h"

using namespace m1;

TankEnemy::TankEnemy(glm::vec3& center) : Tank(center)
{
	color[BODY] = tankColors[DARK_ORANGE];
	color[TURRET] = tankColors[LIGHT_ORANGE];
}
