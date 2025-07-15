#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
    //color = mix(vec4(spriteColor, 0.0f),texture(image, TexCoords), 0.3);
    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
}