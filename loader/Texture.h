#pragma once 

#include<glad/glad.h>

class Texture 
{
public:
	GLuint ID;
	GLuint Width, Height;
	GLuint Internal_Format, Image_Format;
	GLuint Wrap_S, Wrap_T;
	GLuint Filter_Min, Filter_Max;

	Texture();
	void Generate(GLuint Width, GLuint Height, const unsigned char* data);
	void Bind() const; //const��β���κ�������ʾ�ú��������޸����б���������constʵ������ʱ���Կɵ���Bind����

};