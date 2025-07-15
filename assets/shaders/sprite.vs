#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform float time;
uniform int shake;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    if (shake == 1)
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 40) * strength;        
        gl_Position.y += cos(time * 60) * strength;        
    }
}