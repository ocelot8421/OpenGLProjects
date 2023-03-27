#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    //FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    //FragColor = vec4(ourColor, 1.0);
    FragColor = texture(texture1, TexCoord);
}