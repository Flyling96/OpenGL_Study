#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // �Ӷ�����ɫ�����������������������ͬ��������ͬ��
uniform vec4 ourColor; // ��OpenGL����������趨�������
in vec4 ourColor2;

void main()
{
    FragColor = ourColor2;
}