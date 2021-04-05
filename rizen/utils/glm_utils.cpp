#include "glm_utils.h"

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
  return os << vec.x << " ; " << vec.y;
}
