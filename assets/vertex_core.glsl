#version 330 core
layout (location = 0) in vec3 aPos;

//out vec4 color;                                 //learnopengl.com -- https://learnopengl.com/Getting-started/Shaders#:~:text=creatively%20manage%20vectors.-,Ins%20and%20outs,-Shaders%20are%20nice

void main()
{
    gl_Position = vec4(aPos, 1.0);
    //if(aPos.x < 0.0){
    //    color = vec4(1.0, 0.0, 0.0, 1.0);
    //} else if(aPos.x == 0){
    //    color = vec4(0.0, 1.0, 0.0, 1.0);
    //} else {
    //    color = vec4(0.0, 0.0, 1.0, 1.0);       // color = vec4(vec2(0.0), vec2(1.0));
    //}
}