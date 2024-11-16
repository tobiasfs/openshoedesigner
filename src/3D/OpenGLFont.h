///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLFont.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 29.04.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef L3D_OPENGLFONT_H
#define L3D_OPENGLFONT_H

/** \class OpenGLFont
 * 	\code #include "OpenGLFont.h"\endcode
 * 	\ingroup Base3D
 *  \brief Text rendered from predefined 3D letters
 *
 * Reads a file with 3D objects. Parses them into one or more fonts.
 *
 * Renders text in 3D based on the font.
 */

#include "BoundingBox.h"
#include "GeometrySplitter.h"

#include <map>

class OpenGLFont {
public:
	/**\brief Factory function to generate a vector of OpenGLFont%s
	 *
	 * \param geometry 	A geometry object, that contains the letters of one or
	 * 					more font. Note, that the groups assigned in the object
	 * 					are modified.
	 * \param glyph_names_  List of all characters in the font
	 * \param font_count  Number of fonts beneath each other
	 * \return A vector of OpenGLFont
	 */
	static std::vector<OpenGLFont> ParseGeometry(GeometrySplitter &geometry,
			const std::string &glyph_names_, const size_t font_count);

	class Glyph: public Geometry {
	public:
		Glyph() = default;
		explicit Glyph(const Geometry &geometry);
		BoundingBox bb;
	};

	explicit OpenGLFont(const std::string &glyph_names_, double detailoverlap =
			-0.25);
	void AddGeometry(const Geometry &geometry, const BoundingBox &bb);

	void Paint(const std::string &txt) const;
	BoundingBox Size(const std::string &txt) const;
	Vector3 GapCenter(const std::string &txt) const;

	double glyph_distance = 0.05;

	double ascent = 0.5;
	double descent = -0.5;

private:
	std::map<uint32_t, Glyph> glyphs;
	double detail_overlap = 0.0;

	std::string glyph_names;
	std::vector<uint32_t> glyph_codepoints;
};

#endif /* L3D_OPENGLFONT_H */
