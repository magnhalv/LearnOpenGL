#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader();
	Shader &Use();
	void Compile(const GLchar* vertexCode, const GLchar* fragmentCode, const GLchar *gShaderCode);
	void SetBool(const std::string &name, bool value);
	void SetInteger(const std::string &name, int value);
	void SetFloat(const std::string &name, float value);
	void SetMatrix4(const std::string &name, glm::mat4 value);
	void SetVector2f(const GLchar *name, glm::vec2 & value, GLboolean useShader = false);
	void SetVector3f(const GLchar *name, glm::vec3 & value, GLboolean useShader = false);	
	void SetVector4f(const GLchar *name, glm::vec4 & value, GLboolean useShader = false);
private:
	std::string readFile(const GLchar *filePath) const;
	GLuint createShader(const char *sourceCode, GLuint shaderType);
	GLuint linkShaderProgram(GLuint vertexShader, GLuint fragmentShader);
};