//prevzato z https://learnopengl.com/#!Getting-started/Shaders a modifikovano pro me potreby
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		if (fragmentPath != 0) fShaderFile.exceptions(std::ifstream::badbit);
		if (geometryPath != 0) fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			if (fragmentPath != 0) fShaderFile.open(fragmentPath);
			if (geometryPath !=0) gShaderFile.open(geometryPath);
			std::stringstream vShaderStream, fShaderStream, gShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			if (fragmentPath != 0) fShaderStream << fShaderFile.rdbuf();
			if (geometryPath != 0) gShaderStream << gShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			if (fragmentPath != 0) fShaderFile.close();
			if (geometryPath != 0) gShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			if (fragmentPath != 0) fragmentCode = fShaderStream.str();
			if (geometryPath != 0) geometryCode = gShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar * fShaderCode = fragmentCode.c_str();
		const GLchar * gShaderCode = geometryCode.c_str();
		// 2. Compile shaders
		GLuint vertex, fragment, geometry;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Fragment Shader
		if (fragmentPath != 0)
		{
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fShaderCode, NULL);
			glCompileShader(fragment);
			// Print compile errors if any
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragment, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}
		
		//Geometry Shader
		if (geometryPath != 0)
		{
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			// Print compile errors if any
			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(geometry, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}
		
		// Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);

		if (geometryPath != 0) glAttachShader(this->Program, geometry);
		if (fragmentPath != 0) glAttachShader(this->Program, fragment);
		
		if ((geometryPath == 0) && (fragmentPath == 0))
		{
			const GLchar* feedbackVaryings[] = { "newPosition" };
			glTransformFeedbackVaryings(this->Program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
		}
		
		glLinkProgram(this->Program);
		// Print linking errors if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR2::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		if (fragmentPath != 0) glDeleteShader(fragment);
		if (geometryPath != 0) glDeleteShader(geometry);

	}
	// Uses the current shader
	void Use()
	{
		glUseProgram(this->Program);
	}
};

#endif