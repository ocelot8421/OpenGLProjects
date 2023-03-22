#version 330 core
out vec4 FragColor;

//in vec4 color;                                      //learnopengl.com -- https://learnopengl.com/Getting-started/Shaders#:~:text=creatively%20manage%20vectors.-,Ins%20and%20outs,-Shaders%20are%20nice

void main()
{
    FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    //FragColor = color;
}