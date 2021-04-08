#include "shader.h"

Shader::Shader() {}
Shader::~Shader() 
{
    glDeleteProgram(m_program);
}

bool Shader::init(const std::string& vertex_file, const std::string& fragment_file)
{
	m_program = glCreateProgram();

	if (!load_vertex(std::move(vertex_file)))
        return false;
	if (!load_fragment(std::move(fragment_file)))
        return false;

	glLinkProgram(m_program);
	if (!check_link())
        return false;

	glDeleteShader(m_vertex);
	glDeleteShader(m_fragment);
    return true;
}

/*
    Uniforms
*/

GLuint Shader::uniform_loc(const char* name) {
	return glGetUniformLocation(m_program, name);
}

void Shader::set_mat4(const std::string& name, const glm::mat4& value) {
	glUniformMatrix4fv(uniform_loc(name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_vec2(const std::string& name, const glm::vec2& value) {
	glUniform2f(uniform_loc(name.c_str()), value.x, value.y);
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value) {
	glUniform3f(uniform_loc(name.c_str()), value.x, value.y, value.z);
}

void Shader::set_vec4(const std::string& name, const glm::vec4& value) {
	glUniform4f(uniform_loc(name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::set_bool(const std::string& name, bool value) {
	glUniform1i(uniform_loc(name.c_str()), value);
}

void Shader::set_int(const std::string& name, int value) {
	glUniform1i(uniform_loc(name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value) {
	glUniform1f(uniform_loc(name.c_str()), value);
}

/* 
    Methods
*/

bool Shader::load_vertex(const std::string& filename)
{
	m_vertex = glCreateShader(GL_VERTEX_SHADER);
	
	std::string vertex_source;
	if (!read_from_file(shader_path + filename, &vertex_source))
        return false;
	if (!compile_shader(vertex_source.c_str(), m_vertex, "vertex"))
        return false;

	glAttachShader(m_program, m_vertex);
    return true;
}

bool Shader::load_fragment(const std::string& filename)
{
	m_fragment = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fragment_source;
	if (!read_from_file(shader_path + filename, &fragment_source))
        return false;
	if (!compile_shader(fragment_source.c_str(), m_fragment, "fragment"))
        return false;

	glAttachShader(m_program, m_fragment);
    return true;
}

void Shader::bind()
{
	glUseProgram(m_program);
}

bool Shader::compile_shader(const char* source, GLuint shader, const std::string& name)
{
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char info[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, info);
        return rizen_error("shader compilation error [" + name + "]: " + info);
	}
    return true;
}

bool Shader::check_link()
{
	int success;
	char info[512];
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(m_program, 512, NULL, info);
        return rizen_error("shader compilation error [program]: " + std::string(info));
	}
    return true;
}

bool Shader::read_from_file(const std::string& filename, std::string* source)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
        return rizen_error("shader file opening error: [" + filename + "]");

	std::string line;
	while (getline(file, line)) {
		*source += line + '\n';
	}

	file.close();
    return true;
}