#include "Shader.h"

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
	std::string vShaderCode = readFile(vertexPath);
	std::string fShaderCode = readFile(fragmentPath);
	Compile(vShaderCode.c_str(), fShaderCode.c_str(), nullptr);
}

Shader::Shader() {
}

void Shader::Compile(const GLchar* vertexCode, const GLchar* fragmentCode, const GLchar *gShaderCode) {
	GLuint vShader = createShader(vertexCode, GL_VERTEX_SHADER);
	GLuint fShader = createShader(fragmentCode, GL_FRAGMENT_SHADER);
	ID = linkShaderProgram(vShader, fShader);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

Shader &Shader::Use() {
	glUseProgram(ID);
	return *this;
}

void Shader::SetBool(const std::string &name, bool value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetInteger(const std::string &name, int value)  {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string &name, float value) {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetMatrix4(const std::string &name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVector3f(const GLchar *name, glm::vec3 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

std::string Shader::readFile(const GLchar *path) const {
	try {
		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(path);

		std::stringstream fileStream;
		fileStream << file.rdbuf();
		file.close();
		return fileStream.str();
	}
	catch (std::ifstream::failure &e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		throw new std::exception("Could not open shader file.");
	}
}

GLuint Shader::createShader(const char *sourceCode, GLuint shaderType) {
	GLuint shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &sourceCode, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		throw new std::logic_error("Shader compliation failed: " + shaderType);
	}

	return shader;
}

GLuint Shader::linkShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		throw new std::logic_error("Shader program linking failed.");
	}

	return shaderProgram;
}