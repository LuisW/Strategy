#pragma once

#include "ShaderTypes.h"
#include <vector>

class VertexFormat
{
private:
	struct FormatElem
	{
		ShaderType type;
		unsigned int index;
	};

	std::vector<FormatElem> elems;
	unsigned int vertSize;
public:
	VertexFormat()
	{
		
	}

	void SetFormat()
	{
		unsigned int offset = 0;
		for (int n = 0; n < elems.size(); n++)
		{
			glVertexAttribPointer(elems[n].index, STypeElements[elems[n].type], STypeSub[elems[n].type], GL_FALSE, vertSize, (void*)(offset));
			offset += STypeSize[elems[n].type];
		}
	}
};