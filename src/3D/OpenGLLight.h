///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLLight.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 26.03.2019
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

#ifndef L3D_OPENGLLIGHT_H
#define L3D_OPENGLLIGHT_H

/*!\class OpenGLLight
 * \brief Definition of an OpenGL light source.
 * \ingroup OpenGL
 *
 * When constructing a light source from this class one of the OpenGL light
 * source handles has to be passed.
 *
 * lightsource can be set from GL_LIGHT0 to GL_MAX_LIGHTS - 1
 * 
 * \code
 * GLint maxLights;
 * glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
 * \endcode
 * 
 */

#include "Vector3.h"
#include "OpenGL.h"

#include <array>

class OpenGLLight {
public:
	OpenGLLight() = delete;
	explicit OpenGLLight(GLenum lightsource_);

	void SetAmbient(GLfloat r, GLfloat g, GLfloat b);
	void SetAmbient(const Vector3 &c);
	void SetDiffuse(GLfloat r, GLfloat g, GLfloat b);
	void SetDiffuse(const Vector3 &c);
	void SetSpecular(GLfloat r, GLfloat g, GLfloat b);
	void SetSpecular(const Vector3 &c);
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	void SetPosition(const Vector3 &p);
	void Enable(bool on = true);
	void Update(bool preRender = false);

	bool moveWithCamera = false; ///< Move the light together with the camera.

private:
	GLenum lightsource;
	Vector3 position;

	std::array<GLfloat, 4> buffer; ///< Internal buffer
};

#endif /* L3D_OPENGLLIGHT_H */
