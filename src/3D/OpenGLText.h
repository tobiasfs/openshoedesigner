///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLText.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 07.03.2017
// Copyright          : (C) 2017 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef OPENGLTEXT_H
#define OPENGLTEXT_H

/*!\class OpenGLText
 * \brief Simple 3D text with block-front
 * \ingroup View3D
 *
 * This class uses a simple low ascii pixel front to render texts in 3D opengl
 * space.
 */

#include "OpenGLImage.h"

#include <wx/string.h>
#include <wx/image.h>
#include <GL/gl.h>
#include <stdint.h>

class OpenGLText: public OpenGLImage {
public:
	OpenGLText();
	virtual ~OpenGLText();

	void Paint(wxString text) const;

private:
	unsigned int nx;
	unsigned int ny;
	double rx;
	double ry;
};

#endif /* OPENGLTEXT_H */
