#version 450 core
layout (location = 0) in vec3 position;
layout (location = 3) in vec2 diffuseTextureUV;
out vec2 tuv;
uniform float rad;
void main()
{
    vec3 npos;
    npos.x=position.x*cos(rad)-position.y*sin(rad);
    npos.y=position.x*sin(rad)+position.y*cos(rad);
    npos.z=0.0f;
    gl_Position = vec4(npos,1.0f);
    tuv = diffuseTextureUV;
}
