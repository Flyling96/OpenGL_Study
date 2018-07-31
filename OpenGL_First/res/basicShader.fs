#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // �Ӷ�����ɫ�����������������������ͬ��������ͬ��
in vec2 TexCoord;
uniform vec4 ourColor; // ��OpenGL����������趨�������

uniform sampler2D ourTexture;//�ӳ����л�ȡ��������ͼ

in vec4 ourColor2;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * ourColor * ourColor2;
}