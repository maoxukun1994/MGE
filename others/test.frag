#version 450 core
uniform sampler2D tex;
in vec2 tuv;
out vec4 color;
void main()
{
    color = vec4(texture(tex,tuv));
} 
