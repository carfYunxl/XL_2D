#ifndef XL_DATA_HPP_
#define XL_DATA_HPP_

#include "XL_Core.hpp"

_NAMESPACE_BEGIN

namespace QUAD_DATA {
    static const char* quad_vs = R"glsl(
    #version 450 core
    layout(location = 0) in float a_Dummy;
    layout(location = 1) in vec3 aPos;
    layout(location = 2) in vec4 aColor;

    out vec4 v_Color;

    void main()
    {
        v_Color = aColor;
        gl_Position = vec4(aPos, 1.0);
    }
    )glsl";

    static const char* quad_fs = R"glsl(
    #version 450 core
    in vec4 v_Color;

    out vec4 FragColor;
    void main()
    {
        FragColor = v_Color;
    }
    )glsl";
}

namespace CIRCLE_DATA {
    static const char* circle_vs = R"glsl(
    #version 450 core
    layout(location = 0) in float a_Dummy;
    layout(location = 1) in vec3 a_WorldPosition;
    layout(location = 2) in vec3 a_LocalPosition;
    layout(location = 3) in vec4 a_Color;
    layout(location = 4) in float a_Thickness;
    layout(location = 5) in float a_Fade;

    struct VertexOutput
    {
	    vec3 LocalPosition;
	    vec4 Color;
	    float Thickness;
	    float Fade;
    };

    layout (location = 0) out VertexOutput Output;

    void main()
    {
	    Output.LocalPosition = a_LocalPosition;
	    Output.Color = a_Color;
	    Output.Thickness = a_Thickness;
	    Output.Fade = a_Fade;

	    gl_Position = vec4(a_WorldPosition, 1.0);
    }
    )glsl";

    static const char* circle_fs = R"glsl(
    #version 450 core

    layout(location = 0) out vec4 o_Color;
    struct VertexOutput
    {
	    vec3 LocalPosition;
	    vec4 Color;
	    float Thickness;
	    float Fade;
    };

    layout (location = 0) in VertexOutput Input;

    void main()
    {
        // Calculate distance and fill circle with white
        float distance = 1.0 - length(Input.LocalPosition);
        float circle = smoothstep(0.0, Input.Fade, distance);
        circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

        // Set output color
        //o_Color = Input.Color;
	    //o_Color.a *= circle;
        o_Color = vec4( Input.Color.rgb, Input.Color.a * circle );
    }
    )glsl";
}

//if (circle == 0.0)
//discard;

namespace LINE_DATA {
    static const char* line_vs = R"glsl(
    #version 450 core

    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec4 a_Color;

    struct VertexOutput
    {
	    vec4 Color;
    };

    layout (location = 0) out VertexOutput Output;

    void main()
    {
	    Output.Color = a_Color;

	    gl_Position = vec4(a_Position, 1.0);
    }
    )glsl";

    static const char* line_fs = R"glsl(
    #version 450 core

    layout(location = 0) out vec4 o_Color;

    struct VertexOutput
    {
	    vec4 Color;
    };

    layout (location = 0) in VertexOutput Input;

    void main()
    {
	    o_Color = Input.Color;
    }
    )glsl";
}

enum class DrawPlane : uint8_t {
    XY = 0,
    XZ = 1,
    YZ = 2
};

const static glm::vec3 XY_VERTICES_LT{ -0.5f,  0.5f,  0.0f };
const static glm::vec3 XY_VERTICES_RT{  0.5f,  0.5f,  0.0f };
const static glm::vec3 XY_VERTICES_RB{  0.5f, -0.5f,  0.0f };
const static glm::vec3 XY_VERTICES_LB{ -0.5f, -0.5f,  0.0f };

const static glm::vec3 XZ_VERTICES_LT{ -0.5f,  0.0f,  0.5f };
const static glm::vec3 XZ_VERTICES_RT{  0.5f,  0.0f,  0.5f };
const static glm::vec3 XZ_VERTICES_RB{  0.5f,  0.0f, -0.5f };
const static glm::vec3 XZ_VERTICES_LB{ -0.5f,  0.0f, -0.5f };

const static glm::vec3 YZ_VERTICES_LT{  0.0f, -0.5f,  0.5f };
const static glm::vec3 YZ_VERTICES_RT{  0.0f,  0.5f,  0.5f };
const static glm::vec3 YZ_VERTICES_RB{  0.0f,  0.5f, -0.5f };
const static glm::vec3 YZ_VERTICES_LB{  0.0f, -0.5f, -0.5f };

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

const static CubeVertex      g_sCubeVertices;
const static LineVertex      g_sLineVertices;

_NAMESPACE_END



#endif //XL_DATA_HPP_