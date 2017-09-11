#version 330 core

out vec4 FragColor;

in vec3 ourPerVertexColor;

void main()
{
   FragColor = vec4(ourPerVertexColor,1.0f);
}
