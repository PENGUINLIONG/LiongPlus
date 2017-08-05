R"(#version 330 core
layout(std140) uniform BrushSettings
{
	mat4 transform;
	vec4 color;
};
uniform sampler2D tex;
in vec2 v_texPos;
out vec4 colorOut;
void main()
{
	vec4 sample = texture(tex, v_texPos);
	colorOut = sample.rgba * color.rgba;
})"
