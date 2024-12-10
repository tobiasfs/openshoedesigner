///////////////////////////////////////////////////////////////////////////////
// Name               : BackgroundImage.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 30.01.2017
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

#include "BackgroundImage.h"

#include "OpenGL.h"

void BackgroundImage::Paint() const {
	if (showFront && showBack)
		glDisable(GL_CULL_FACE);
	if (showFront && !showBack)
		glCullFace(GL_BACK);
	if (!showFront && showBack)
		glCullFace(GL_FRONT);
	if (!showFront && !showBack)
		glCullFace(GL_FRONT_AND_BACK);

	glPushMatrix();
	switch (orientation) {
	case Orientation::Top:
		break;
	case Orientation::Bottom:
		glRotatef(180, 1, 0, 0);
		break;
	case Orientation::Right:
		glRotatef(90, 1, 0, 0);
		break;
	case Orientation::Left:
		glRotatef(90, 1, 0, 0);
		glRotatef(180, 0, 0, 1);
		break;
	case Orientation::Front:
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 0, 1);
		break;
	case Orientation::Back:
		glRotatef(90, 1, 0, 0);
		glRotatef(-90, 0, 0, 1);
		break;
	}

	glRotatef(rotate, 0.0, 0.0, 1.0);
	glScalef(scale, scale, 1.0);
	glTranslatef(-centerx, -centery, 0.0);
	OpenGLImage::Paint();
	glPopMatrix();

	if (showFront && showBack)
		glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}
