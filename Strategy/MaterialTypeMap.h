#pragma once

#include "ei/GL/glew.h"
#include <string>

template < typename T > GLenum getShaderType();
template < GLenum type > size_t getShaderTypeSize();

#define LINK_GLSL_TYPE0(type, glsl) template<> GLenum getShaderType<type>(){ return glsl; } 
#define LINK_GLSL_TYPE1(type, glsl) template<> size_t getShaderTypeSize<glsl>(){ return sizeof(type); } 

#define LINK_GLSL_TYPE(type, glsl) LINK_GLSL_TYPE0(type, glsl) LINK_GLSL_TYPE1(type, glsl)

size_t getShaderTypeSize(GLenum type);
GLenum getShaderBaseType(GLenum type);
size_t getShaderBaseTypeSize(GLenum type);

const char* SemanticNames[];
unsigned int SemNameLen[];