#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）
in vec2 TexCoord;
uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量

uniform sampler2D ourTexture;//从程序中获取的纹理贴图

in vec4 ourColor2;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * ourColor * ourColor2;
}