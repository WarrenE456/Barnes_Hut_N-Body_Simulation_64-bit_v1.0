#pragma once

#include <string>
#include "shaderPair.h"

static shaderPair parseShader(std::string& filepath, bool threeDimensions)
{
	std::ifstream stream(filepath);
	int mode = -1;
	std::string line;
	std::stringstream shaders[2];

	//-1 for don't write, 0 for vertex, 1 for fragment

	while (getline(stream, line))
	{
		if (line.find("#vertex2d") != std::string::npos)
		{
			if (!threeDimensions)
			{
				mode = 0;
			}
			else
			{
				mode = -1;
			}
		}
		else if (line.find("#vertex3d") != std::string::npos)
		{
			if (threeDimensions)
			{
				mode = 0;
			}
			else
			{
				mode = -1;
			}
		}
		else if (line.find("#fragment") != std::string::npos) {
			mode = 1;
		}
		else
		{
			if (mode != -1)
				shaders[mode] << line << '\n';
		}
	}

	return { shaders[0].str(), shaders[1].str() };
}

static unsigned int compileShader(unsigned int type, std::string& source)
{
	unsigned int shaderID = glCreateShader(type);
	char* scr = &(source[0]);
	glShaderSource(shaderID, 1, &scr, nullptr);
	glCompileShader(shaderID);

	int status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(sizeof(char) * length);
		glGetShaderInfoLog(shaderID, length, &length, message);
		std::cout << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "FAILED TO COMPILE: " << message;
		glDeleteShader(shaderID);
		return 0;
	}

	return shaderID;
}

static unsigned int createShader(std::string& vertexSource, std::string& fragmentSource)
{
	unsigned int program = glCreateProgram();
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glValidateProgram(program);

	int status;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (status != GL_TRUE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(sizeof(char) * length);
		glGetProgramInfoLog(program, length, &length, message);
		std::cout << "ERROR: " << message << "\n";
		glDeleteProgram(program);
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}