R"(#version 330 core
layout(std140) uniform BrushSettings
{
	mat4 transform;
	vec4 color;
};
layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec2 texPos;
out vec2 v_texPos;
void main()
{
	gl_Position = transform * vec4(vertexPos, 0., 1.);
	v_texPos = vec2(texPos.x, 1. - texPos.y);
})"
