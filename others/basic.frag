#version 450 core
//ins
in vec2 tuv;
//outs
out vec4 color;
//uniforms
uniform sampler2D tex;

void main()
{
    color = vec4(texture(tex,tuv));
} 
 
