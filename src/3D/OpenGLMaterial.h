///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLMaterial.h
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

#ifndef L3D_OPENGLMATERIAL_H
#define L3D_OPENGLMATERIAL_H

/*!\class OpenGLMaterial
 * \brief Simple Material definition for OpenGL
 * \ingroup OpenGL
 *
 * This class contains the ambient, diffuse and specular values for classic
 * material in OpenGL. The Function UseMaterial activates the material in
 * the OpenGL context. The static Function UseColor switches back to a glColor
 * based setting of colors / materials.
 *
 * The functions only change the material definition, if GL_COLOR_WRITEMASK is
 * all enabled. If not all channels (red, green blue, alpha) are activated in
 * the mask this hints that the 3D stereo display is calculated and everythig
 * is rendered in a single color.
 */

#include "Vector3.h"

#include <string.h>

class OpenGLMaterial {
public:
	enum class Preset {
		Emerald,
		Jade,
		Obsidian,
		Pearl,
		Ruby,
		Turquoise,
		Brass,
		Bronze,
		Chrome,
		Copper,
		Gold,
		Silver,
		BlackPlastic,
		CyanPlastic,
		GreenPlastic,
		RedPlastic,
		WhitePlastic,
		YellowPlastic,
		BlackRubber,
		CyanRubber,
		GreenRubber,
		RedRubber,
		WhiteRubber,
		YellowRubber,
		cRed,
		cYellow,
		cGreen,
		cLime,
		cTeal,
		cOlive,
		cBlue,
		cAqua,
		cNavy,
		cPurple,
		cFuchsia,
		cMaroon,
		cGray,
		cSilver,
		cBlack,
		cWhite
	}; //<! Preset materials

	OpenGLMaterial();
	explicit OpenGLMaterial(Preset preset, float emit = 0.0,
			float opacity = 1.0);
	OpenGLMaterial(float r, float g, float b, float emit = 0.0, float opacity =
			1.0);
	virtual ~OpenGLMaterial() = default;

	void Set(Preset preset); ///< Set the material to a preset.
	static std::string GetPresetName(Preset preset); ///< Return the name of a preset.

	void SetSimpleColor(float r, float g, float b, float emit = 0.0,
			float opacity = 1.0); ///< Equivalent to using the glColor command.
	void SetSimpleColor(Vector3 c, float emit = 0.0, float opacity = 1.0); ///< Uses x,y,z of a vector as the color components.

	void UseMaterial() const; ///< Start using this material. Turns off glColor command.
	void UseColor() const; ///< Activates glColor and sets the diffuse color of this material.
	void UseColor(float emit) const; ///< Activates glColor and sets the diffuse color of this material.
	static void EnableColors(); ///< Switch back to using glColor.

	static bool ColorsAllowed(); ///< Test, if colors are allowed to use.

	std::string ToString() const;

	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 emission;
	float shininess;
	float opacity;
};

#endif /* L3D_OPENGLMATERIAL_H */

