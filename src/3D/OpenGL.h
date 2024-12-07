///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGL.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.02.2020
// Copyright          : (C) 2020 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_OPENGL_H
#define L3D_OPENGL_H

/**\file Opengl.h
 * \code #include "OpenGL.h"\endcode
 * \ingroup OpenGL
 * \brief Header to include all the relevant OpenGL support classes
 *
 * Only include this header in the classes to use OpenGL. If something is
 * missing, extend this header.
 */


#ifdef USE_GLAD

#if defined(__gl_h_) || defined(__GL_H__) || defined(_GL_H) || defined(__X_GL_H)
    #error "Please do not include gl.h before this file."
#endif

#ifdef __APPLE__
    // Suppress warnings when including gl3.h
    #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
    #include <OpenGL/gl3.h> // For Core Profile
#else

#include "glad/glad.h"

#endif

#else

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#define __WIN
#elif defined(linux) || defined(__linux)
#define __LINUX
#else
#error "Neither a Linux nor a Windows system was identified!"
#endif

#ifdef __WIN
#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#endif

#ifdef __WXMAC__
#include "OpenGL/gl.h"
#ifdef GL_GLEXT_PROTOTYPES
#include "OpenGL/glext.h"
#endif
#endif

#ifdef __LINUX
#include <GL/glew.h>
#include <GL/gl.h>
#ifdef GL_GLEXT_PROTOTYPES
#include <GL/glext.h>
#endif
#endif

#endif

#endif /* L3D_OPENGL_H */

