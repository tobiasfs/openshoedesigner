///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLMaterial.cpp
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

#include "OpenGLMaterial.h"

#include <sstream>

#include "OpenGL.h"

OpenGLMaterial::OpenGLMaterial() {
	ambient.Set(0.2, 0.2, 0.2);
	diffuse.Set(0.8, 0.8, 0.8);
	specular.Set(0.7, 0.7, 0.7);
	emission.Set(0.0, 0.0, 0.0);
	shininess = 32;
}

OpenGLMaterial::OpenGLMaterial(Preset preset, float emit) {
	Set(preset);
	emission = diffuse * emit;
}

OpenGLMaterial::OpenGLMaterial(float r, float g, float b, float emit) {
	SetSimpleColor(r, g, b, emit);
}

void OpenGLMaterial::Set(Preset preset) {
	// Preset-materials from http://devernay.free.fr/cours/opengl/materials.html
	switch (preset) {
	case Preset::Emerald:
		ambient.Set(0.0215, 0.1745, 0.0215);
		diffuse.Set(0.07568, 0.61424, 0.07568);
		specular.Set(0.633, 0.727811, 0.633);
		shininess = 38.4;
		break;
	case Preset::Jade:
		ambient.Set(0.135, 0.2225, 0.1575);
		diffuse.Set(0.54, 0.89, 0.63);
		specular.Set(0.316228, 0.316228, 0.316228);
		shininess = 6.4;
		break;
	case Preset::Obsidian:
		ambient.Set(0.05375, 0.05, 0.06625);
		diffuse.Set(0.18275, 0.17, 0.22525);
		specular.Set(0.332741, 0.328634, 0.346435);
		shininess = 19.2;
		break;
	case Preset::Pearl:
		ambient.Set(0.25, 0.20725, 0.20725);
		diffuse.Set(1, 0.829, 0.829);
		specular.Set(0.296648, 0.296648, 0.296648);
		shininess = 5.632;
		break;
	case Preset::Ruby:
		ambient.Set(0.1745, 0.01175, 0.01175);
		diffuse.Set(0.61424, 0.04136, 0.04136);
		specular.Set(0.727811, 0.626959, 0.626959);
		shininess = 38.4;
		break;
	case Preset::Turquoise:
		ambient.Set(0.1, 0.18725, 0.1745);
		diffuse.Set(0.396, 0.74151, 0.69102);
		specular.Set(0.297254, 0.30829, 0.306678);
		shininess = 6.4;
		break;
	case Preset::Brass:
		ambient.Set(0.329412, 0.223529, 0.027451);
		diffuse.Set(0.780392, 0.568627, 0.113725);
		specular.Set(0.992157, 0.941176, 0.807843);
		shininess = 13.9487;
		break;
	case Preset::Bronze:
		ambient.Set(0.2125, 0.1275, 0.054);
		diffuse.Set(0.714, 0.4284, 0.18144);
		specular.Set(0.393548, 0.271906, 0.166721);
		shininess = 12.8;
		break;
	case Preset::Chrome:
		ambient.Set(0.25, 0.25, 0.25);
		diffuse.Set(0.4, 0.4, 0.4);
		specular.Set(0.774597, 0.774597, 0.774597);
		shininess = 38.4;
		break;
	case Preset::Copper:
		ambient.Set(0.19125, 0.0735, 0.0225);
		diffuse.Set(0.7038, 0.27048, 0.0828);
		specular.Set(0.256777, 0.137622, 0.086014);
		shininess = 6.4;
		break;
	case Preset::Gold:
		ambient.Set(0.24725, 0.1995, 0.0745);
		diffuse.Set(0.75164, 0.60648, 0.22648);
		specular.Set(0.628281, 0.555802, 0.366065);
		shininess = 25.6;
		break;
	case Preset::Silver:
		ambient.Set(0.19225, 0.19225, 0.19225);
		diffuse.Set(0.50754, 0.50754, 0.50754);
		specular.Set(0.508273, 0.508273, 0.508273);
		shininess = 25.6;
		break;
	case Preset::BlackPlastic:
		ambient.Set(0.0, 0.0, 0.0);
		diffuse.Set(0.01, 0.01, 0.01);
		specular.Set(0.50, 0.50, 0.50);
		shininess = 16;
		break;
	case Preset::CyanPlastic:
		ambient.Set(0.0, 0.1, 0.06);
		diffuse.Set(0.0, 0.50980392, 0.50980392);
		specular.Set(0.50196078, 0.50196078, 0.50196078);
		shininess = 16;
		break;
	case Preset::GreenPlastic:
		ambient.Set(0.0, 0.0, 0.0);
		diffuse.Set(0.1, 0.35, 0.1);
		specular.Set(0.45, 0.55, 0.45);
		shininess = 16;
		break;
	case Preset::RedPlastic:
		ambient.Set(0.0, 0.0, 0.0);
		diffuse.Set(0.5, 0.0, 0.0);
		specular.Set(0.7, 0.6, 0.6);
		shininess = 16;
		break;
	case Preset::WhitePlastic:
		ambient.Set(0.0, 0.0, 0.0);
		diffuse.Set(0.55, 0.55, 0.55);
		specular.Set(0.70, 0.70, 0.70);
		shininess = 16;
		break;
	case Preset::YellowPlastic:
		ambient.Set(0.0, 0.0, 0.0);
		diffuse.Set(0.5, 0.5, 0.0);
		specular.Set(0.60, 0.60, 0.50);
		shininess = 16;
		break;
	case Preset::BlackRubber:
		ambient.Set(0.02, 0.02, 0.02);
		diffuse.Set(0.01, 0.01, 0.01);
		specular.Set(0.4, 0.4, 0.4);
		shininess = 5;
		break;
	case Preset::CyanRubber:
		ambient.Set(0.0, 0.05, 0.05);
		diffuse.Set(0.4, 0.5, 0.5);
		specular.Set(0.04, 0.7, 0.7);
		shininess = 5;
		break;
	case Preset::GreenRubber:
		ambient.Set(0.0, 0.05, 0.0);
		diffuse.Set(0.4, 0.5, 0.4);
		specular.Set(0.04, 0.7, 0.04);
		shininess = 5;
		break;
	case Preset::RedRubber:
		ambient.Set(0.05, 0.0, 0.0);
		diffuse.Set(0.5, 0.4, 0.4);
		specular.Set(0.7, 0.04, 0.04);
		shininess = 5;
		break;
	case Preset::WhiteRubber:
		ambient.Set(0.05, 0.05, 0.05);
		diffuse.Set(0.5, 0.5, 0.5);
		specular.Set(0.7, 0.7, 0.7);
		shininess = 5;
		break;
	case Preset::YellowRubber:
		ambient.Set(0.05, 0.05, 0.0);
		diffuse.Set(0.5, 0.5, 0.4);
		specular.Set(0.7, 0.7, 0.04);
		shininess = 5;
		break;
	case Preset::cRed: //Color Red
		ambient.Set(1.0, 0.0, 0.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;

	case Preset::cYellow: //Color Yellow
		ambient.Set(1.0, 1.0, 0.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cGreen: //Color Green
		ambient.Set(0.0, 0.5, 0.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cLime: //Color Lime
		ambient.Set(0.0, 1.0, 0.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cTeal: //Color Teal
		ambient.Set(0.0, 0.5, 0.5);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cOlive: //Color Olive
		ambient.Set(0.5, 0.5, 0.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cBlue: //Color Blue
		ambient.Set(0.0, 0.0, 1.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cAqua: //Color Aqua
		ambient.Set(0.0, 1.0, 1.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cNavy: //Color Navy
		ambient.Set(0.0, 0.0, 0.5);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cPurple: //Color Purple
		ambient.Set(0.5, 0.0, 0.5);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cFuchsia: //Color Fuchsia
		ambient.Set(1.0, 0.0, 1.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cMaroon: //Color Maroon
		ambient.Set(0.5, 0.0, 0.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cGray: //Color Gray
		ambient.Set(0.75, 0.75, 0.75);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cSilver: //Color Silver
		ambient.Set(0.5, 0.5, 0.5);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cBlack: //Color Black
		ambient.Set(0.0, 0.0, 0.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;
	case Preset::cWhite: //Color White
		ambient.Set(1.0, 1.0, 1.0);
		diffuse = ambient;
		specular.Zero();
		shininess = 0;
		break;

	}
	emission.Zero();
}

std::string OpenGLMaterial::GetPresetName(OpenGLMaterial::Preset preset) {
	switch (preset) {
	case Preset::Emerald:
		return ("Emerald");
	case Preset::Jade:
		return ("Jade");
	case Preset::Obsidian:
		return ("Obsidian");
	case Preset::Pearl:
		return ("Pearl");
	case Preset::Ruby:
		return ("Ruby");
	case Preset::Turquoise:
		return ("Turquoise");
	case Preset::Brass:
		return ("Brass");
	case Preset::Bronze:
		return ("Bronze");
	case Preset::Chrome:
		return ("Chrome");
	case Preset::Copper:
		return ("Copper");
	case Preset::Gold:
		return ("Gold");
	case Preset::Silver:
		return ("Silver");
	case Preset::BlackPlastic:
		return ("Black plastic");
	case Preset::CyanPlastic:
		return ("Cyan plastic");
	case Preset::GreenPlastic:
		return ("Green plastic");
	case Preset::RedPlastic:
		return ("Red plastic");
	case Preset::WhitePlastic:
		return ("White plastic");
	case Preset::YellowPlastic:
		return ("Yellow plastic");
	case Preset::BlackRubber:
		return ("Black rubber");
	case Preset::CyanRubber:
		return ("Cyan rubber");
	case Preset::GreenRubber:
		return ("Green rubber");
	case Preset::RedRubber:
		return ("Red rubber");
	case Preset::WhiteRubber:
		return ("White rubber");
	case Preset::YellowRubber:
		return ("Yellow rubber");
	case Preset::cRed:
		return ("Red");
	case Preset::cYellow:
		return ("Yellow");
	case Preset::cGreen:
		return ("Green");
	case Preset::cLime:
		return ("Lime");
	case Preset::cTeal:
		return ("Teal");
	case Preset::cOlive:
		return ("Olive");
	case Preset::cBlue:
		return ("Blue");
	case Preset::cAqua:
		return ("Aqua");
	case Preset::cNavy:
		return ("Navy");
	case Preset::cPurple:
		return ("Purple");
	case Preset::cFuchsia:
		return ("Fuchsia");
	case Preset::cMaroon:
		return ("Maroon");
	case Preset::cGray:
		return ("Gray");
	case Preset::cSilver:
		return ("Silver");
	case Preset::cBlack:
		return ("Black");
	case Preset::cWhite:
		return ("White");
	}
	return ("");
}

void OpenGLMaterial::SetSimpleColor(float r, float g, float b, float emit) {
	ambient.Set(r, g, b);
	diffuse.Set(r, g, b);
	specular.Zero();
	emission = diffuse * emit;
	shininess = 0;
}

void OpenGLMaterial::SetSimpleColor(Vector3 c, float emit) {
	SetSimpleColor(c.x, c.y, c.z, emit);
}

bool OpenGLMaterial::ColorsAllowed() {
	GLboolean colormask[4];
	glGetBooleanv(GL_COLOR_WRITEMASK, colormask);
	return (colormask[0] && colormask[1] && colormask[2] && colormask[3]);
}

void OpenGLMaterial::UseMaterial() const {
	if (!ColorsAllowed())
		return;

	double opacity = 0.5;

	glDisable(GL_COLOR_MATERIAL);
	GLfloat buffer[4];
	buffer[0] = ambient.x;
	buffer[1] = ambient.y;
	buffer[2] = ambient.z;
	buffer[3] = opacity;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, buffer);
	buffer[0] = diffuse.x;
	buffer[1] = diffuse.y;
	buffer[2] = diffuse.z;
	buffer[3] = opacity;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, buffer);
	buffer[0] = specular.x;
	buffer[1] = specular.y;
	buffer[2] = specular.z;
	buffer[3] = opacity;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, buffer);
	buffer[0] = emission.x;
	buffer[1] = emission.y;
	buffer[2] = emission.z;
	buffer[3] = opacity;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, buffer);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void OpenGLMaterial::UseColor(float emit) const {
	if (!ColorsAllowed())
		return;
	glEnable(GL_COLOR_MATERIAL);
	GLfloat buffer[] = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, buffer);
	buffer[0] = diffuse.x * emit;
	buffer[1] = diffuse.y * emit;
	buffer[2] = diffuse.z * emit;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, buffer);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	glColor3f(diffuse.x, diffuse.y, diffuse.z);
}

void OpenGLMaterial::UseColor() const {
	if (!ColorsAllowed())
		return;
	glEnable(GL_COLOR_MATERIAL);
	GLfloat buffer[] = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, buffer);
	buffer[0] = emission.x;
	buffer[1] = emission.y;
	buffer[2] = emission.z;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, buffer);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	glColor3f(diffuse.x, diffuse.y, diffuse.z);
}

void OpenGLMaterial::EnableColors() {
	if (!ColorsAllowed())
		return;
	glEnable(GL_COLOR_MATERIAL);
	GLfloat buffer[] = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, buffer);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, buffer);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
}

std::string OpenGLMaterial::ToString() const {
	std::ostringstream out;
	out << "{ambient:[";
	out << ambient.x << ',' << ambient.y << ',' << ambient.z;
	out << "],diffuse:[";
	out << diffuse.x << ',' << diffuse.y << ',' << diffuse.z;
	out << "],specular:[";
	out << specular.x << ',' << specular.y << ',' << specular.z;
	out << "],emission:[";
	out << emission.x << ',' << emission.y << ',' << emission.z;
	out << "],shininess:";
	out << shininess;
	out << "]}";
	return out.str();
}
