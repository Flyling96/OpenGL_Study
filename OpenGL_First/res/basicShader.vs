#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec4 aColor; // 颜色变量的属性位置值为 1
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor2; // 向片段着色器输出一个颜色
out vec2 TexCoord; //纹理信息

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos * 0.5f, 1.0);
    ourColor2 = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
	TexCoord = aTexCoord;
}