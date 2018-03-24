#include "Shader.h"

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
	std::string vShaderCode = readFile(vertexPath);
	std::string fShaderCode = readFile(fragmentPath);
	GLuint vShader = createShader(vShaderCode.c_str(), GL_VERTEX_SHADER);
	GLuint fShader = createShader(fShaderCode.c_str(), GL_FRAGMENT_SHADER);
	ID = linkShaderProgram(vShader, fShader);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
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