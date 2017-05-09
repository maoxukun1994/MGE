#version 450 core

//ins
layout (location = 0) in vec3 position;
layout (location = 3) in vec2 diffuseTextureUV;
//outs
out vec2 tuv;
//uniforms
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(position,1.0f);
    tuv = diffuseTextureUV;
}
