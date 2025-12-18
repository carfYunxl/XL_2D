#ifndef __XL_SHADER_SOURCE_HPP__
#define __XL_SHADER_SOURCE_HPP__

#include "XL_Core.hpp"

_NAMESPACE_BEGIN

static const char* g_sVertexShader = R"glsl(
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
out vec4 v_Color;
uniform vec4 u_Color;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
	v_Color = u_Color;
}
)glsl";

static const char* g_sFragShader = R"glsl(
#version 330 core
in vec4 v_Color;
layout(location = 0) out vec4 color;
void main()
{
	color = v_Color;
}
)glsl";

_NAMESPACE_END



#endif //__XL_SHADER_SOURCE_HPP__