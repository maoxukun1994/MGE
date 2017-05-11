#ifndef MGE_MATH_H
#define MGE_MATH_H



//math headers includer
//currently using glm

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vector_relational.hpp"

//definations for convient usage
typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

typedef glm::mat3x3 Mat3;
typedef glm::mat4x4 Mat4;

#define matTranslate glm::translate

#endif //MGE_MATH_H
