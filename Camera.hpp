#include "glm/vec3.hpp"
#include "glm/glm.hpp"

// Image size
const int WIDTH = 800;
const int HEIGHT = 800;

// camera positions
const glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

const glm::vec3 CameraDirection = glm::normalize(CameraPos - CameraTarget);

class Camera {
public:

private:

};
