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
	void Bind() const; //const结尾修饰函数，表示该函数不会修改类中变量。当用const实例化类时，仍可调用Bind函数

};