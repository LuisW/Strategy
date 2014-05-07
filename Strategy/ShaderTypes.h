#pragma once

#include "ei/GL/glew.h"

enum ShaderType { ST1i, ST1f, ST1ui, ST2fv, ST3fv, ST4fv, ST2iv, ST3iv, ST4iv, ST2uiv, ST3uiv, ST4uiv, STm4, STm3, STm2, STm23, STm24, STm32, STm34, STm42, STm43, STtex, STbyte, STubyte, STunknown };	//internal enum for OpenGL shader types. can be mapped using "MapGLEnumToShaderType" and "MapShaderTypeToGLEnum"
extern const unsigned int STypeSize[];
extern const char STypeElements[];
extern const GLenum STypeSub[];

ShaderType MapGLEnumToShaderType(GLenum type);
GLenum MapShaderTypeToGLEnum(ShaderType type);
