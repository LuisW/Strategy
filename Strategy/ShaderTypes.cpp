#include "ShaderTypes.h"

const unsigned int STypeSize[] = { sizeof(GLint), sizeof(GLfloat), sizeof(GLuint), sizeof(GLfloat)* 2, sizeof(GLfloat)* 3, sizeof(GLfloat)* 4, sizeof(GLint)* 2, sizeof(GLint)* 3,
sizeof(GLint)* 4, sizeof(GLuint)* 2, sizeof(GLuint)* 3, sizeof(GLuint)* 4, sizeof(GLfloat)* 16, sizeof(GLfloat)* 9, sizeof(GLfloat)* 4,
sizeof(GLfloat)* 6, sizeof(GLfloat)* 8, sizeof(GLfloat)* 6, sizeof(GLfloat)* 12, sizeof(GLfloat)* 8, sizeof(GLfloat)* 12, sizeof(GLint), sizeof(GLbyte), sizeof(GLubyte) };

const char STypeElements[] = { 1, 1, 1, 2, 3, 4, 2, 3, 4, 2, 3, 4, 16, 9, 4, 6, 8, 6, 12, 8, 12, 1, 1, 1 };

const GLenum STypeSub[] = { GL_INT, GL_FLOAT, GL_UNSIGNED_INT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_INT, GL_INT, GL_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT
, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_INT, GL_BYTE, GL_UNSIGNED_BYTE };

ShaderType MapGLEnumToShaderType(GLenum type)
{
	switch (type)
	{
	case GL_FLOAT:
		return ST1f;
	case GL_FLOAT_VEC2:
		return ST2fv;
	case GL_FLOAT_VEC3:
		return ST3fv;
	case GL_FLOAT_VEC4:
		return ST4fv;
	case GL_INT:
		return ST1i;
	case GL_INT_VEC2:
		return ST2iv;
	case GL_INT_VEC3:
		return ST3iv;
	case GL_INT_VEC4:
		return ST4iv;
	case GL_UNSIGNED_INT:
		return ST1ui;
	case GL_UNSIGNED_INT_VEC2:
		return ST2uiv;
	case GL_UNSIGNED_INT_VEC3:
		return ST3uiv;
	case GL_UNSIGNED_INT_VEC4:
		return ST4uiv;
	case GL_BOOL:
		return STbyte;
	case GL_FLOAT_MAT2:
		return STm2;
	case GL_FLOAT_MAT3:
		return STm3;
	case GL_FLOAT_MAT4:
		return STm4;
	case GL_FLOAT_MAT2x3:
		return STm23;
	case GL_FLOAT_MAT3x2:
		return STm32;
	case GL_FLOAT_MAT4x2:
		return STm42;
	case GL_FLOAT_MAT2x4:
		return STm24;
	case GL_FLOAT_MAT3x4:
		return STm34;
	case GL_FLOAT_MAT4x3:
		return STm43;
	case GL_SAMPLER_2D:
		return STtex;
	default:
		return STunknown;
	}
}

GLenum MapShaderTypeToGLEnum(ShaderType type)
{
	switch (type)
	{
	case ST1f:
		return GL_FLOAT;
	case ST2fv:
		return GL_FLOAT_VEC2;
	case ST3fv:
		return GL_FLOAT_VEC3;
	case ST4fv:
		return GL_FLOAT_VEC4;
	case ST1i:
		return GL_INT;
	case ST2iv:
		return GL_INT_VEC2;
	case ST3iv:
		return GL_INT_VEC3;
	case ST4iv:
		return GL_INT_VEC4;
	case ST1ui:
		return GL_UNSIGNED_INT;
	case ST2uiv:
		return GL_UNSIGNED_INT_VEC2;
	case ST3uiv:
		return GL_UNSIGNED_INT_VEC3;
	case ST4uiv:
		return GL_UNSIGNED_INT_VEC4;
	case STbyte:
		return GL_BOOL;
	case STm2:
		return GL_FLOAT_MAT2;
	case STm3:
		return GL_FLOAT_MAT3;
	case STm4:
		return GL_FLOAT_MAT4;
	case STm23:
		return GL_FLOAT_MAT2x3;
	case STm32:
		return GL_FLOAT_MAT3x2;
	case STm42:
		return GL_FLOAT_MAT4x2;
	case STm24:
		return GL_FLOAT_MAT2x4;
	case STm34:
		return GL_FLOAT_MAT3x4;
	case STm43:
		return GL_FLOAT_MAT4x3;
	case STtex:
		return GL_SAMPLER_2D;
	default:
		return 0;
	}
}