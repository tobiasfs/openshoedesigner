///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLShader.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.04.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
// Licence            : GNU General Public License version 3.0 (GPLv3)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////

//#define GL_GLEXT_PROTOTYPES
#include "OpenGLShader.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <sstream>

OpenGLShader::~OpenGLShader() {
	Delete();
}

void OpenGLShader::Delete() {
	Stop();
	if (program > 0)
		glDeleteProgram(program);
	program = 0;
	for (auto &it : shader)
		glDeleteShader(it.second);
	shader.clear();
}

void OpenGLShader::AddShader(GLenum type, const std::string &program_) {
	GLuint shadernum = 0;
	if (shader.find(type) == shader.end()) {
		shadernum = glCreateShader(type);
		shader[type] = shadernum;
	} else {
		shadernum = shader[type];
	}
	const GLchar *temp[] = { program_.c_str() };
	glShaderSource(shadernum, 1, temp, NULL);
	glCompileShader(shadernum);

	GLint success;
	glGetShaderiv(shadernum, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar txt[512];
		glGetShaderInfoLog(shadernum, 512, NULL, txt);
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		out << "Error while compiling shader:";
		out << ' ' << txt << '\n';
		throw std::runtime_error(out.str());
	}
}

bool OpenGLShader::LinkShader() {
	glDeleteProgram(program);
	program = glCreateProgram();

	for (auto &it : shader)
		glAttachShader(program, it.second);

	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
		GLchar info[512];
		glGetProgramInfoLog(program, 512, NULL, info);
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		out << "Error while linking shader program:";
		out << ' ' << info << '\n';
		out << std::endl;

		glDeleteProgram(program);
		program = 0;
		throw std::runtime_error(out.str());
	}
	return true;
}

bool OpenGLShader::Start() {
	if (program == 0)
		return false;
	glUseProgram(program);
	return true;
}

void OpenGLShader::AddShaderFromFile(GLenum type,
		const std::string &filename_) {
	std::ifstream in(filename_);
	if (!in) {
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		out << " Could not open file: " << filename_;
		throw std::runtime_error(out.str());
	}
	std::string program_;
	std::string buffer(4096, '\0');
	while (in.read(&buffer[0], 4096)) {
		program_.append(buffer, 0, in.gcount());
	}
	program_.append(buffer, 0, in.gcount());
	AddShader(type, program_);
}

void OpenGLShader::Stop() {
	if (program >= 1)
		glUseProgram(0);
}

bool OpenGLShader::SetUniformBool(const std::string &name,
		const GLboolean x) const {
	if (program == 0)
		return false;
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		return false;
	glUniform1i(location, x ? -1 : 0);
	return true;
}

bool OpenGLShader::SetUniformInt(const std::string &name, const GLint x) const {
	if (program == 0)
		return false;
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		return false;
	glUniform1i(location, x);
	return true;
}

bool OpenGLShader::SetUniform(const std::string &name, const Vector3 &x) const {
	if (program == 0)
		return false;
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		return false;
	glUniform3f(location, x.x, x.y, x.z);
	return true;
}

bool OpenGLShader::SetUniform(const std::string &name, const GLfloat x) const {
	if (program == 0)
		return false;
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		return false;
	glUniform1f(location, x);
	return true;
}

bool OpenGLShader::SetUniform(const std::string &name, const GLfloat x,
		const GLfloat y) const {
	if (program == 0)
		return false;
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		return false;
	glUniform2f(location, x, y);
	return true;
}

bool OpenGLShader::SetUniform(const std::string &name, const GLfloat x,
		const GLfloat y, const GLfloat z) const {
	if (program == 0)
		return false;
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		return false;
	glUniform3f(location, x, y, z);
	return true;
}

bool OpenGLShader::SetUniform(const std::string &name, const GLfloat x,
		const GLfloat y, const GLfloat z, const GLfloat w) const {
	if (program == 0)
		return false;
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		return false;
	glUniform4f(location, x, y, z, w);
	return true;
}

bool OpenGLShader::SetUniformMatrix3(const std::string &name,
		const AffineTransformMatrix &M) const {
	if (program == 0)
		return false;
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		return false;
	M.GLSetUniformMatrix3(location);
	return true;
}

bool OpenGLShader::SetUniformMatrix4(const std::string &name,
		const AffineTransformMatrix &M) const {
	if (program == 0)
		return false;
	GLint location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		return false;
	M.GLSetUniformMatrix4(location);
	return true;
}

GLint OpenGLShader::GetUniformLocation(const std::string &name) const {
	if (program == 0)
		return -1;
	return glGetUniformLocation(program, name.c_str());
}

std::string OpenGLShader::FieldString() {
	return "XYZRGB";
}
