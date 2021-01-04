#ifndef MATHS_H
#define MATHS_H

#include <glm/glm.hpp>

class Maths
{
public:
	static glm::mat4 CreateTransformMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);
};

#endif // MATHS_H
