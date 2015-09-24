#include "MaterialTypeMap.h"
#include "AssetManager.h"

LINK_GLSL_TYPE(float, GL_FLOAT);
LINK_GLSL_TYPE(glm::vec2, GL_FLOAT_VEC2);
LINK_GLSL_TYPE(glm::vec3, GL_FLOAT_VEC3);
LINK_GLSL_TYPE(glm::vec4, GL_FLOAT_VEC4);
LINK_GLSL_TYPE(double, GL_DOUBLE);
LINK_GLSL_TYPE(glm::highp_vec2, GL_DOUBLE_VEC2);
LINK_GLSL_TYPE(glm::highp_vec3, GL_DOUBLE_VEC3);
LINK_GLSL_TYPE(glm::highp_vec4, GL_DOUBLE_VEC4);
LINK_GLSL_TYPE(int, GL_INT);
LINK_GLSL_TYPE(glm::ivec2, GL_INT_VEC2);
LINK_GLSL_TYPE(glm::ivec3, GL_INT_VEC3);
LINK_GLSL_TYPE(glm::ivec4, GL_INT_VEC4);
LINK_GLSL_TYPE(unsigned int, GL_UNSIGNED_INT);
LINK_GLSL_TYPE(glm::uvec2, GL_UNSIGNED_INT_VEC2);
LINK_GLSL_TYPE(glm::uvec3, GL_UNSIGNED_INT_VEC3);
LINK_GLSL_TYPE(glm::uvec4, GL_UNSIGNED_INT_VEC4);
LINK_GLSL_TYPE(bool, GL_BOOL);
LINK_GLSL_TYPE(glm::bvec2, GL_BOOL_VEC2);
LINK_GLSL_TYPE(glm::bvec3, GL_BOOL_VEC3);
LINK_GLSL_TYPE(glm::bvec4, GL_BOOL_VEC4);
LINK_GLSL_TYPE(glm::mat2, GL_FLOAT_MAT2);
LINK_GLSL_TYPE(glm::mat3, GL_FLOAT_MAT3);
LINK_GLSL_TYPE(glm::mat4, GL_FLOAT_MAT4);
LINK_GLSL_TYPE(glm::mat2x3, GL_FLOAT_MAT2x3);
LINK_GLSL_TYPE(glm::mat2x4, GL_FLOAT_MAT2x4);
LINK_GLSL_TYPE(glm::mat3x2, GL_FLOAT_MAT3x2);
LINK_GLSL_TYPE(glm::mat3x4, GL_FLOAT_MAT3x4);
LINK_GLSL_TYPE(glm::mat4x2, GL_FLOAT_MAT4x2);
LINK_GLSL_TYPE(glm::mat4x3, GL_FLOAT_MAT4x3);
//LINK_GLSL_TYPE(int, GL_SAMPLER_1D);
//LINK_GLSL_TYPE(int, GL_SAMPLER_2D);
//LINK_GLSL_TYPE(int, GL_SAMPLER_3D);

size_t getShaderTypeSize(GLenum type)
{
	switch (type)
	{
	case GL_FLOAT:
		return sizeof(GLfloat);
	case GL_FLOAT_VEC2:
		return 2 * sizeof(GLfloat);
	case GL_FLOAT_VEC3:
		return 3 * sizeof(GLfloat);
	case GL_FLOAT_VEC4:
		return 4 * sizeof(GLfloat);
	case GL_DOUBLE:
		return sizeof(GLdouble);
	case GL_DOUBLE_VEC2:
		return 2 * sizeof(GLdouble);
	case GL_DOUBLE_VEC3:
		return 3 * sizeof(GLdouble);
	case GL_DOUBLE_VEC4:
		return 4 * sizeof(GLdouble);
	case GL_INT:
		return sizeof(GLint);
	case GL_INT_VEC2:
		return 2 * sizeof(GLint);
	case GL_INT_VEC3:
		return 3 * sizeof(GLint);
	case GL_INT_VEC4:
		return 4 * sizeof(GLint);
	case GL_UNSIGNED_INT:
		return sizeof(GLuint);
	case GL_UNSIGNED_INT_VEC2:
		return 2 * sizeof(GLuint);
	case GL_UNSIGNED_INT_VEC3:
		return 3 * sizeof(GLuint);
	case GL_UNSIGNED_INT_VEC4:
		return 4 * sizeof(GLuint);
	case GL_BOOL:
		return sizeof(GLboolean);
	case GL_BOOL_VEC2:
		return 2 * sizeof(GLboolean);
	case GL_BOOL_VEC3:
		return 3 * sizeof(GLboolean);
	case GL_BOOL_VEC4:
		return 4 * sizeof(GLboolean);
	case GL_FLOAT_MAT2:
		return 4 * sizeof(GLfloat);
	case GL_FLOAT_MAT3:
		return 9 * sizeof(GLfloat);
	case GL_FLOAT_MAT4:
		return 16 * sizeof(GLfloat);
	case GL_FLOAT_MAT2x3:
		return 6 * sizeof(GLfloat);
	case GL_FLOAT_MAT2x4:
		return 8 * sizeof(GLfloat);
	case GL_FLOAT_MAT3x2:
		return 6 * sizeof(GLfloat);
	case GL_FLOAT_MAT3x4:
		return 12 * sizeof(GLfloat);
	case GL_FLOAT_MAT4x2:
		return 8 * sizeof(GLfloat);
	case GL_FLOAT_MAT4x3:
		return 12 * sizeof(GLfloat);
	}
}

GLenum getShaderBaseType(GLenum type)
{
	switch (type)
	{
	case GL_DOUBLE:
	case GL_DOUBLE_VEC2:
	case GL_DOUBLE_VEC3:
	case GL_DOUBLE_VEC4:
		return GL_DOUBLE;

	case GL_INT:
	case GL_INT_VEC2:
	case GL_INT_VEC3:
	case GL_INT_VEC4:
		return GL_INT;

	case GL_UNSIGNED_INT:
	case GL_UNSIGNED_INT_VEC2:
	case GL_UNSIGNED_INT_VEC3:
	case GL_UNSIGNED_INT_VEC4:
		return GL_UNSIGNED_INT;

	case GL_BOOL:
	case GL_BOOL_VEC2:
	case GL_BOOL_VEC3:
	case GL_BOOL_VEC4:
		return GL_BOOL;

	case GL_FLOAT:
	case GL_FLOAT_VEC2:
	case GL_FLOAT_VEC3:
	case GL_FLOAT_VEC4:
	case GL_FLOAT_MAT2:
	case GL_FLOAT_MAT3:
	case GL_FLOAT_MAT4:
	case GL_FLOAT_MAT2x3:
	case GL_FLOAT_MAT2x4:
	case GL_FLOAT_MAT3x2:
	case GL_FLOAT_MAT3x4:
	case GL_FLOAT_MAT4x2:
	case GL_FLOAT_MAT4x3:
		return GL_FLOAT;

	case GL_SAMPLER_1D:
	case GL_SAMPLER_2D:
	case GL_SAMPLER_3D:
		return GL_SAMPLER_1D;
	}
}

size_t getShaderBaseTypeSize(GLenum type)
{
	switch (type)
	{
	case GL_FLOAT:
	case GL_DOUBLE:
	case GL_INT:
	case GL_UNSIGNED_INT:
	case GL_BOOL:
	case GL_SAMPLER_1D:
	case GL_SAMPLER_2D:
	case GL_SAMPLER_3D:
		return 1;

	case GL_FLOAT_VEC2:
	case GL_DOUBLE_VEC2:
	case GL_INT_VEC2:
	case GL_UNSIGNED_INT_VEC2:
	case GL_BOOL_VEC2:
		return 2;

	case GL_FLOAT_VEC3:
	case GL_DOUBLE_VEC3:
	case GL_INT_VEC3:
	case GL_UNSIGNED_INT_VEC3:
	case GL_BOOL_VEC3:
		return 3;

	case GL_FLOAT_VEC4:
	case GL_DOUBLE_VEC4:
	case GL_INT_VEC4:
	case GL_UNSIGNED_INT_VEC4:
	case GL_BOOL_VEC4:
	case GL_FLOAT_MAT2:
		return 4;

	case GL_FLOAT_MAT2x3:
	case GL_FLOAT_MAT3x2:
		return 6;

	case GL_FLOAT_MAT2x4:
	case GL_FLOAT_MAT4x2:
		return 8;

	case GL_FLOAT_MAT3:
		return 9;

	case GL_FLOAT_MAT3x4:
	case GL_FLOAT_MAT4x3:
		return 12;

	case GL_FLOAT_MAT4:
		return 16;
	}
}

const char* SemanticNames[] = { "in_Position", "in_Normal", "in_TexCoord", "in_Tangent", "in_Color", "in_Weight", "in_User" };
unsigned int SemNameLen[] = { strlen(SemanticNames[0]), strlen(SemanticNames[1]), strlen(SemanticNames[2]), strlen(SemanticNames[3]), strlen(SemanticNames[4]),
strlen(SemanticNames[5]), strlen(SemanticNames[6]) };