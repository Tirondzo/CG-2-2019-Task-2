#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>

// OpenGL 4.3 and later support the glDebugMessageCallback API
// This is some sort of implementation on this API

// Useful error checking macros
// In the line where it was used throws ThrowExceptionOnGLError if opengl error occurred
// Writes line number, file name and error code in the console
#define GL_CHECK_ERRORS ThrowExceptionOnGLError(__LINE__, __FILE__);
#define GL_CHECK(_call) do{_call; GL_CHECK_ERRORS;}while(0);

//#define PI 3.1415926535897932384626433832795f

static void ThrowExceptionOnGLError(int line, const char *file)
{

	static char errMsg[512];

	// Check opengl for error occurrence, get error code
	GLenum gl_error = glGetError();

	if (gl_error == GL_NO_ERROR)
		return;

	switch (gl_error)
	{
	case GL_INVALID_ENUM:
		std::cerr << "GL_INVALID_ENUM file " << file << " line " << line << std::endl;
		break;

	case GL_INVALID_VALUE:
		std::cerr << "GL_INVALID_VALUE file " << file << " line " << line << std::endl;
		break;

	case GL_INVALID_OPERATION:
		std::cerr << "GL_INVALID_OPERATION file " << file << " line " << line << std::endl;
		break;

	case GL_STACK_OVERFLOW:
		std::cerr << "GL_STACK_OVERFLOW file " << file << " line " << line << std::endl;
		break;

	case GL_STACK_UNDERFLOW:
		std::cerr << "GL_STACK_UNDERFLOW file " << file << " line " << line << std::endl;
		break;

	case GL_OUT_OF_MEMORY:
		std::cerr << "GL_OUT_OF_MEMORY file " << file << " line " << line << std::endl;
		break;

	case GL_NO_ERROR:
		break;

	default:
		std::cerr << "Unknown error @ file " << file << " line " << line << std::endl;
		break;
	}

	if (gl_error != GL_NO_ERROR)
		throw std::runtime_error(errMsg);
}