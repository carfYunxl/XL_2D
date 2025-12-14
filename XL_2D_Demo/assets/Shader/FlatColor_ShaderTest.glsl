// Flat Color Shader
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

out vec3 v_Position;

void main()
{
	gl_Position = vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
	v_Position = a_Position;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_Position;

void main()
{
	vec3 sc = vec3(0.0); 

	vec2 bl = smoothstep(vec2(0.1),vec2(0.2),vec2(abs(v_Position.x), abs(v_Position.y)));
	vec2 tr = smoothstep(vec2(0.1),vec2(0.2),1.0-vec2(abs(v_Position.x), abs(v_Position.y)));
	float pct = bl.x * bl.y * tr.x * tr.y;
	
	sc = vec3(pct);

	color = vec4(sc, 1.0);
}
