#include "Texture.h"


Texture::Texture() 
	:Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), 
	 Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Max(GL_LINEAR_MIPMAP_LINEAR), Filter_Min(GL_LINEAR_MIPMAP_LINEAR)
{
	glGenTextures(1, &this->ID);
}

//生成一张纹理
void Texture::Generate(GLuint Width, GLuint Height, const unsigned char* data)
{	
	this->Width = Width; this->Height = Height;
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, this->Width, this->Height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	//如果纹理过滤方式是mipmap就一定要加上这个，不然是黑色
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

}

//绑定纹理
void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}