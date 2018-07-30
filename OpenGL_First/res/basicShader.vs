#version 330 core
layout (location = 0) in vec3 aPos;   // λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec4 aColor; // ��ɫ����������λ��ֵΪ 1

out vec4 ourColor2; // ��Ƭ����ɫ�����һ����ɫ

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor2 = aColor; // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
}