//
// Created by kookie on 28/03/2020.
//

#include "shader.h"
#include "util/graphics_error.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <array>

void meg::shader::check_compile_errors(unsigned int shader, const std::string& type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

meg::shader::shader(const std::string& shader_path, bool save_code)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::ifstream shader_file(shader_path);

    if(shader_file.is_open()){
        enum type{INVALID = -1, FRAGMENT = 0, VERTEX = 1};
        std::array<std::stringstream, 2> shader_streams;
        int type = INVALID;

        std::string line;
        while(std::getline(shader_file, line)){
            if(line.rfind("//type=") == 0){
                if(line.rfind("fragment") == 7) type = type::FRAGMENT;
                else if(line.rfind("vertex") == 7) type = type::VERTEX;
            }
            else{
                if(type != -1){
                    shader_streams[type] << line << '\n';
                }
            }
        }

        vertex_code   = shader_streams[type::VERTEX].str();
        fragment_code = shader_streams[type::FRAGMENT].str();
    }
    else{
        throw meg::graphics_error{"Error opening shader file!"};
    }

    //Shader program vendor
    const char* vertexShaderCode = vertex_code.c_str();
    const char* fragmentShaderCode = fragment_code.c_str();

    //Compile the vertex shader
    const unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    //Compile the fragment shader
    const unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    //Link the shader program
    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    glLinkProgram(id_);
    check_compile_errors(id_, "PROGRAM");

    //Tidy up
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if(!save_code){
        vertex_code.clear();
        vertex_code.shrink_to_fit();
        fragment_code.clear();
        fragment_code.shrink_to_fit();
    }
}

meg::shader::~shader()
{
    glDeleteProgram(id_);
}

void meg::shader::use() const
{
    glUseProgram(id_);
}

void meg::shader::set_bool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

void meg::shader::set_int(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void meg::shader::set1f(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void meg::shader::set2f(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
}

void meg::shader::set3f(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}

void meg::shader::set4f(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
}

void meg::shader::set_mat2f(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void meg::shader::set_mat3f(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void meg::shader::set_mat4f(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

const std::string &meg::shader::get_vertex_code() {
    return vertex_code;
}

const std::string &meg::shader::get_fragment_code() {
    return fragment_code;
}
