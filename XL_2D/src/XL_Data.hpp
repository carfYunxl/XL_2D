#ifndef XL_DATA_HPP_
#define XL_DATA_HPP_

#include "XL_Core.hpp"

_NAMESPACE_BEGIN

namespace QUAD_DATA {
    static const char* quad_vs = R"glsl(
    #version 450 core
    layout(location = 1) in vec3 aPos;
    layout(location = 2) in vec4 aColor;
    layout(location = 3) in vec2 aLocal;

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

    static const char* quad_tess = R"glsl(
    #type vertex
    #version 450 core
    // 顶点布局与 C++ 的 attribute 位置一致：0=pos,1=color,2=local
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec4 aColor;
    layout(location = 2) in vec2 aLocal;
    layout(location = 3) in float aTessLevel;
    layout(location = 4) in vec2 aBorderThickness;
    layout(location = 5) in ivec2 aSelectCell;

    out vec4 vColor;
    out vec2 vLocal;
    out vec3 vPos;
    out float vTessLevel;
    out vec2 vBorderThickness;
    flat out ivec2 vSelectCell;

    void main()
    {
        vColor = aColor;
        vLocal = aLocal;
        vPos = aPos;
        vTessLevel = aTessLevel;
        vBorderThickness = aBorderThickness;
        vSelectCell = aSelectCell;
        gl_Position = vec4(aPos, 1.0);
    }

    #type tess_control
    #version 450 core
    layout(vertices = 4) out;

    in vec4 vColor[];
    in vec2 vLocal[];
    in vec3 vPos[];
    in float vTessLevel[];
    in vec2 vBorderThickness[];
    flat in ivec2 vSelectCell[];

    out vec4 tcColor[];
    out vec2 tcLocal[];
    out vec3 tcPos[];
    out float tcTessLevel[];
    out vec2 tcBorderThickness[];
    flat out ivec2 tcSelectCell[];

    void main()
    {
        // 传递控制点数据到外部
        tcColor[gl_InvocationID] = vColor[gl_InvocationID];
        tcLocal[gl_InvocationID] = vLocal[gl_InvocationID];
        tcPos[gl_InvocationID] = vPos[gl_InvocationID];
        tcTessLevel[gl_InvocationID] = vTessLevel[gl_InvocationID];
        tcBorderThickness[gl_InvocationID] = vBorderThickness[gl_InvocationID];
        tcSelectCell[gl_InvocationID] = vSelectCell[gl_InvocationID];

        // 仅第一个 invocation 设置 tess level
        if (gl_InvocationID == 0)
        {
            float lvlf = max(1.0, floor(tcTessLevel[0]));
            // outer / inner 使用相同级别，得到 lvl x lvl 网格
            gl_TessLevelOuter[0] = lvlf;
            gl_TessLevelOuter[1] = lvlf;
            gl_TessLevelOuter[2] = lvlf;
            gl_TessLevelOuter[3] = lvlf;

            gl_TessLevelInner[0] = lvlf;
            gl_TessLevelInner[1] = lvlf;
        }
    }

    #type tess_evaluation
    #version 450 core
    layout(quads, equal_spacing, ccw) in;

    in vec4 tcColor[];
    in vec2 tcLocal[];
    in vec3 tcPos[];
    in float tcTessLevel[];
    in vec2 tcBorderThickness[];
    flat in ivec2 tcSelectCell[];

    out vec4 teColor;
    out vec2 teLocal;
    out vec2 teUV;
    out float teLevel;
    out vec2 teBorderThickness;
    flat out ivec2 teSelectCell;

    void main()
    {
        // uv in [0,1] inside the quad
        vec2 uv = gl_TessCoord.xy;

        // bilinear 插值控制点位置
        vec2 top = mix(tcPos[0].xy, tcPos[1].xy, uv.x);
        vec2 bottom = mix(tcPos[3].xy, tcPos[2].xy, uv.x);
        vec2 pos = mix(top, bottom, uv.y);

        // local / color 同样插值
        vec2 localTop = mix(tcLocal[0], tcLocal[1], uv.x);
        vec2 localBottom = mix(tcLocal[3], tcLocal[2], uv.x);
        teLocal = mix(localTop, localBottom, uv.y);

        vec4 colorTop = mix(tcColor[0], tcColor[1], uv.x);
        vec4 colorBottom = mix(tcColor[3], tcColor[2], uv.x);
        //teColor = mix(colorTop, colorBottom, uv.y);
        teColor = tcColor[0];
        teBorderThickness = tcBorderThickness[0];
        teSelectCell = tcSelectCell[0];

        // 传递 uv 与 level 到片段着色器
        teUV = uv;
        teLevel = max(1.0, floor(tcTessLevel[0]));

        gl_Position = vec4(pos, tcPos[0].z, 1.0);
    }

    #type fragment
    #version 450 core
    in vec4 teColor;
    in vec2 teLocal;
    in vec2 teUV;
    in float teLevel;
    in vec2 teBorderThickness;
    flat in ivec2 teSelectCell;

    uniform vec4  u_BorderColor = vec4(0.0, 0.0, 0.0, 1.0); // 边框颜色

    layout(location = 0) out vec4 FragColor;

    void main()
    {
        float lvl = max(1.0, teLevel);
        // cell 内局部坐标 0..1
        vec2 cellUV = fract(teUV * lvl);
        int x = int(floor(teUV.x * lvl));
        int y = int(floor(teUV.y * lvl));

        // 距离最近边界的距离（0 在边界处，0.5 在中心）
        float distToEdgeX = min(cellUV.x, 1.0 - cellUV.x);
        float distToEdgeY = min(cellUV.y, 1.0 - cellUV.y);
        float distToEdge = min(distToEdgeX, distToEdgeY);

        // thickness 表示边框宽度，以 cell 单位：例如 0.04 表示 cell 宽度的 4%
        //float thicknessX = clamp(teBorderThickness.x, 0.0, 0.5);
        //float thicknessY = clamp(teBorderThickness.y, 0.0, 0.5);

        float thicknessX = teBorderThickness.x;
        float thicknessY = teBorderThickness.y;

        if(teUV.x < thicknessX || teUV.x > (1 - thicknessX)) {
            thicknessX = thicknessX;
        } else {
            thicknessX = thicknessX * 0.5;
        }

        if(teUV.y < thicknessY || teUV.y > (1 - thicknessY)) {
            thicknessY = thicknessY;
        } else {
            thicknessY = thicknessY * 0.5;
        }

        vec3 color = teColor.rgb;
        if(ivec2(x,y) == teSelectCell)
            color = vec3(1.0, 0.0, 0.0); // 中心格子填充红色，便于调试
        if( distToEdgeX < thicknessX || distToEdgeY < thicknessY)
            color = u_BorderColor.rgb;

        FragColor = vec4(color,1.0f);
    }
    )glsl";
}

namespace CIRCLE_DATA {
    static const char* circle_vs = R"glsl(
    #version 450 core
    layout(location = 0) in vec3 a_WorldPosition;
    layout(location = 1) in vec3 a_LocalPosition;
    layout(location = 2) in vec4 a_Color;
    layout(location = 3) in float a_Thickness;
    layout(location = 4) in float a_Fade;

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
        // 1. 计算到中心的距离 (0为中心, 1为边界)
        float d = length(Input.LocalPosition);
    
        // 2. 利用 fwidth 获取 LocalPosition 在当前像素下的变化率
        // 这代表了 1.0 个屏幕像素在 Local 坐标系下的长度
        float px = fwidth(Input.LocalPosition.x); 
    
        // 3. 定义目标边框宽度（像素）
        float targetPx = Input.Thickness; 
    
        // 4. 计算外边缘：在 1.0 处进行 1 像素平滑（抗锯齿）
        float outer = smoothstep(1.0, 1.0 - px, d);
        float thickness = targetPx * px;
        float circle;
        if(thickness >= 1.0)
        {
            circle = outer;
        }
        else
        {
            // 5. 计算内边缘：半径减去 (targetPx * px) 即为内环起始点
            // 同样给予 1 像素平滑以防锯齿
            float innerThreshold = 1.0 - thickness;

            if (innerThreshold <= px) {
                // 情况 A：半径太小，2 像素边框已经填满了整个圆
                circle = outer; 
            } else {
                // 情况 B：正常圆环，计算内部镂空，并保持 1 像素抗锯齿
                float inner = smoothstep(innerThreshold - px, innerThreshold, d);
                circle = outer * inner;
            }
        }
        
        if(circle < 0.001)
            discard;        

        o_Color = vec4(Input.Color.rgb, Input.Color.a * circle);
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