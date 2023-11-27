#pragma once

namespace m1
{
	class Building
	{
	public:
		Building(glm::vec3& position, glm::vec3& scale) : scale(scale),
				position(position + glm::vec3(0, scale.y / 2, 0)),
				radius(glm::vec3(0.5, 0.5, 0.5) * scale),
				color(glm::vec3(0.56, 0.56, 0.56)) {};

		glm::vec3 color;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 radius;
	};
}
