#include "glm/glm.hpp"

#include <array>

class Triangle {
public:
   Triangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& _color){
		positions[0] = p1;
		positions[1] = p2;
		positions[2] = p3;
		color = _color;

		//  normal = glm::normalize(glm::vec3(1,1,1));
		normal = glm::normalize(glm::cross(p2-p1, p3-p2));
	};
    
	//referens eller kopia
   glm::vec3 &getNormal() {return normal;}
private:
	std::array<glm::vec3, 3> positions;
	glm::vec3 normal;
	glm::vec3 color;
};