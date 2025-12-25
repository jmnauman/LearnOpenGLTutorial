#include "shader.h"

bool checkCompilationStatus(GLuint shaderId, const std::string& path);

Shader::Shader(const char* vertPath, const char* fragPath)
{
	id = -1;

	std::string vertSrc;
	std::string fragSrc;
	if (!readFile(vertPath, vertSrc) || !readFile(fragPath, fragSrc)) return;

	const char* vertCStr = vertSrc.c_str();
	const char* fragCStr = fragSrc.c_str();

	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertCStr, nullptr);
	glCompileShader(vert);
	if (!checkCompilationStatus(vert, vertPath)) return;

	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragCStr, nullptr);
	glCompileShader(frag);
	if (!checkCompilationStatus(frag, fragPath)) return;

	GLuint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	int success = 0;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cout << "Could not link shader program compiled from " << vertPath << " and " << fragPath << '\n' << infoLog << 'n';
		return;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	id = program;
}

void Shader::use() const
{
	glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform1f(location, value);
}

void Shader::setMatrix4(const std::string& name, glm::mat4 mat) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMatrix3(const std::string& name, glm::mat3 mat) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVector3(const std::string& name, glm::vec3 vec) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform3fv(location, 1, &vec[0]);
}

bool checkCompilationStatus(GLuint shaderId, const std::string& path)
{
	char infoLog[512];
	int success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
		std::cout << "Failed to compile shader " << path << "\n" << infoLog << '\n';
		return false;
	}

	return true;
}
