R"(#version 330 core

const int MAX_BONE_COUNT = 500;

const int BDEF1 = 0;
const int BDEF2 = 1;
const int BDEF4 = 2;
const int SDEF = 3;
const int QDEF = 4;

layout(std140) uniform Bones
{
	mat4 bones[MAX_BONE_COUNT];
}

layout(location = 0) vec4 matWorld;
layout(location = 1) vec4 matWorldInverse;
layout(location = 2) vec4 matWorldView;
layout(location = 3) vec4 matWorldViewProject;
layout(location = 4) vec4 matView;
layout(location = 5) vec4 matViewInverse;
layout(location = 6) vec4 matProject;
layout(location = 7) vec4 matProjectInverse;
layout(location = 8) vec4 matViewProject;
layout(location = 9) vec4 matViewProjectInverse;

layout(location = 10) vec3 cameraPosition;
layout(location = 11) vec3 cameraDirection;

layout(location = 12) vec4 materialDiffuse;
layout(location = 13) vec3 materialAmbient;
layout(location = 14) vec3 materialSpecular;
layout(location = 15) vec3 materialToon;
layout(location = 16) float materialPower;

layout(location = 17) in vec3 pos;
layout(location = 18) in vec3 normal;
layout(location = 19) in vec2 uv;

// THIS IMPLEMENTATION IGNORES ADDITIONAL VECTORS!!!

// Bones.
layout(location = 29) in int defType;
layout(location = 21) in ivec4 boneIndicies;
layout(location = 22) in vec4 weightIndicies;
layout(location = 23) in vec3 c;
layout(location = 24) in vec3 r0;
layout(location = 25) in vec3 r1;

out vec3 pos;

void main()
{
})"