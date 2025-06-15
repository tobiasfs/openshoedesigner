///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLText.cpp
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

#include "OpenGLText.h"

#include "font.xpm"

#include "OpenGL.h"

OpenGLText::OpenGLText() :
		OpenGLImage(wxImage(font_xpm)) {
	nx = 16;
	ny = 8;
	rx = 1.0 / (double) nx;
	ry = 1.0 / (double) ny;
}

OpenGLText::~OpenGLText() {
}

void OpenGLText::Paint(const std::string &text) const {
	if (text.empty())
		return;
	Update();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);

	for (size_t n = 0; n < text.length(); n++) {
		const unsigned char c = text[n];
		if (c >= 128)
			continue;
		const double x = ((double) (c % nx) * rx);
		const double y = ((double) (c / nx) * ry);
		glTexCoord2f(this->tex_w * (x), this->tex_h * (y));
		glVertex3f((float) n, 0.0, 1.0);
		glTexCoord2f(this->tex_w * (x), this->tex_h * (y + ry));
		glVertex3f((float) n, 0.0, 0.0);
		glTexCoord2f(this->tex_w * (x + rx), this->tex_h * (y + ry));
		glVertex3f((float) n + 1.0, 0.0, 0.0);
		glTexCoord2f(this->tex_w * (x + rx), this->tex_h * (y));
		glVertex3f((float) n + 1.0, 0.0, 1.0);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
