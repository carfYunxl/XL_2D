#ifndef __XL_SHADER_SOURCE_HPP__
#define __XL_SHADER_SOURCE_HPP__

#include "XL_Core.hpp"

_NAMESPACE_BEGIN

static const char* batch_vs = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 v_Color;

void main()
{
    v_Color = aColor;
    gl_Position = u_Projection * u_View * vec4(aPos, 1.0);
}
)glsl";

static const char* batch_fs = R"glsl(
#version 330 core
in vec4 v_Color;
out vec4 FragColor;
void main()
{
    FragColor = v_Color;
}
)glsl";

enum class DrawPlane
{
    XY = 0,
    XZ = 1,
    YZ = 2
};

struct TriangleVertex
{
    std::array<std::array<float, 6>, 3> vertices_XY{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f ,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f ,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    std::array<std::array<float, 6>, 3> vertices_XZ{
        -0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f ,
         0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f ,
         0.0f, 0.0f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    std::array<std::array<float, 6>, 3> vertices_YZ{
         0.0f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f ,
         0.0f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f ,
         0.0f,  0.0f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    std::array<unsigned int, 3> indices{
        0, 1, 2
    };
};

struct RectangleVertex
{
    std::array<std::array<float, 6>, 4> vertices_XY{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    };

    std::array<std::array<float, 6>, 4> vertices_XZ{
        -0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, 0.0f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.0f,  0.5f, 1.0f, 1.0f, 0.0f
    };

    std::array<std::array<float, 6>, 4> vertices_YZ{
         0.0f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.0f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.0f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f
    };

    std::array<unsigned int, 6> indices{
        0, 1, 2, 2, 3, 0
    };
};

struct CubeVertex
{
    std::array<std::array<float, 6>, 8> vertices{
       -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
       -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f
    };
    std::array<unsigned int, 36> indices{
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        0,1,5, 5,4,0,
        2,3,7, 7,6,2,
        0,3,7, 7,4,0,
        1,2,6, 6,5,1
    };
};

struct LineVertex
{
    std::array<std::array<float, 6>, 2> vertices{
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };
    std::array<unsigned int, 2> indices{
        0, 1
    };
};

const static TriangleVertex  g_sTriangleVertices;
const static RectangleVertex g_sRectangleVertices;
const static CubeVertex      g_sCubeVertices;
const static LineVertex      g_sLineVertices;

_NAMESPACE_END



#endif //__XL_SHADER_SOURCE_HPP__