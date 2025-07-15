#include "shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(){}

Shader& Shader::Use() 
{
	glUseProgram(this->ID);

	return *this;
}

//根据vs，fs，gs编译shader。gs可选
void Shader::Compile(const GLchar* vertexSource, 
					 const GLchar* fragmentSource,
	                 const GLchar* geometrySource) 
{
	//创建并编译shader
	GLuint vs, gs, fs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexSource, NULL);
	glCompileShader(vs);
	checkCompileErrors(vs, "SHADER");

	if (geometrySource != nullptr) {
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gs, 1, &geometrySource, NULL);
		glCompileShader(gs);
		checkCompileErrors(gs, "SHADER");
	}

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentSource, NULL);
	glCompileShader(fs);
	checkCompileErrors(fs, "SHADER");

	//Program添加shader
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vs);
	if (geometrySource != nullptr) {
		glAttachShader(this->ID, gs);
	}
	glAttachShader(this->ID, fs);

	//链接Program
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");

	//释放Shader
	glDeleteShader(fs);
	if (geometrySource != nullptr) {
		glDeleteShader(gs);
	}
	glDeleteShader(fs);
}

//设置一维浮点数uniform
void Shader::SetFloat(const GLchar* name, GLfloat value, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

//设置一维整数uniform
void Shader::SetInteger(const GLchar* name, GLint value, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

//设置二维浮点数uniform
void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

//设置三维浮点数uniform
void Shader::SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

//设置四维浮点数uniform
void Shader::SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w,GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

//设置矩阵uniform
void Shader::SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader) {
	if (useShader) {
		this->Use();
	}
	//注意一定要包含type_ptr.hpp，不然没有value_ptr，无法对矩阵取地址
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix)); 
}








//检查编译结果
void Shader::checkCompileErrors(GLuint object, std::string type) {
	GLint success;
	GLchar infolog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infolog);
			std::cout << "COMPILE SHADER ERROR:" << infolog << std::endl;
		}
	}
	else {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, NULL, infolog);
			std::cout << "LINK PROGRAM ERROR:" << infolog << std::endl;
		}
	}
}
