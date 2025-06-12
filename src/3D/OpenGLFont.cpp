///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLFont.cpp
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
#include "OpenGLFont.h"

#include "../system/UTF8.h"

#include <algorithm>
#include <iostream>

#include "OpenGL.h"

std::vector<OpenGLFont> OpenGLFont::ParseGeometry(GeometrySplitter &geometry,
		const std::string &glyph_names_, const size_t font_count) {

	//TODO Derive the following two value from the 3D-data itself.
	double fontdistance = 1.0;
	double fontlowline = -0.15;

	geometry.Split();

	// Find all parts in the geometry
	size_t groupCount = geometry.objects.size();

	for (size_t n = 0; n < groupCount; ++n) {
		if (!geometry.objects[n].PassedSelfCheck()) {
			std::cerr << "The glyphe in object " << n << " is broken.\n";
		}
	}

	// Loop over all fonts select all letter-parts of a font and copy them into
	// a OpenGLFont object.
	std::vector<OpenGLFont> ret;
	ret.assign(font_count, OpenGLFont(glyph_names_));

	for (size_t font_idx = 0; font_idx < font_count; ++font_idx) {
		const double limit0 = 1.0 - fontdistance * (double) font_idx
				+ fontlowline;

//		size_t c = 0;
		for (size_t n = 0; n < groupCount; ++n)
			if (geometry.bbs[n].GetCenter().y <= limit0
					&& geometry.bbs[n].GetCenter().y > (limit0 - 1.0)) {
//				std::cout << c++ << '/' << groupCount << '\n';
				ret[font_idx].AddGeometry(geometry.objects[n], geometry.bbs[n]);
			}
		for (auto &glyph : ret[font_idx].glyphs) {
			BoundingBox &bb = glyph.second.bb;
			const double cy = std::round(
					(bb.ymin + bb.ymax) / 2.0 - fontlowline - 0.5);
			const double cx = bb.xmin;
			glyph.second.matrix.TranslateGlobal(-cx, -cy, 0.0);
			bb.xmin -= cx;
			bb.xmax -= cx;
			bb.ymin -= cy;
			bb.ymax -= cy;
		}
		// Determine the hp-vertical height ("hp-Vertikalhöhe").
		BoundingBox fd;
		auto test = UTF8::GetCodepoints("hpHP");
		// Add the first glyph as well to always have some size > 0.
		test.push_back(ret[font_idx].glyphs.begin()->first);
		for (auto cp : test) {
			const Glyph &glyph = ret[font_idx].glyphs.at(cp);
			fd.Insert(glyph.bb);
		}
		double hpv = fd.GetSizeY();
		if (hpv < FLT_EPSILON)
			throw std::runtime_error(
					"The hp vertical height could not be determined.");
		ret[font_idx].ascent = fd.ymax / hpv;
		ret[font_idx].descent = fd.ymin / hpv;

		// Rescale all fonts to the same unit-height.
		const double sf = 1.0 / hpv;
		for (auto &glyph : ret[font_idx].glyphs) {
			BoundingBox &bb = glyph.second.bb;
			glyph.second.matrix.ScaleGlobal(sf, sf, sf);
			bb.xmin *= sf;
			bb.xmax *= sf;
			bb.ymin *= sf;
			bb.ymax *= sf;
			bb.zmin *= sf;
			bb.zmax *= sf;
		}

		for (auto &glyph : ret[font_idx].glyphs)
			glyph.second.ApplyTransformationMatrix();

//		std::cout << fd.ToString() << '\n';
//		ret[font_idx].line_distance = fd.GetSizeY() * 1.2;
	}

	return ret;
}

OpenGLFont::OpenGLFont(const std::string &glyphnames_, double detailoverlap) {
	this->glyph_names = glyphnames_;
	this->glyph_codepoints = UTF8::GetCodepoints(glyph_names);
	this->detail_overlap = detailoverlap;

	if (glyph_codepoints.front() <= 32 || glyph_codepoints.back() <= 32)
		throw std::runtime_error(
				"OpenGLFont - The first and last codepoint have to be printable.");
}

void OpenGLFont::AddGeometry(const Geometry &geometry, const BoundingBox &bb) {

	Glyph glyph(geometry);
	glyph.bb = bb;
	if (glyphs.empty()) {
		if (glyph_codepoints.empty())
			throw std::runtime_error(
					"OpenGLFont::AddGeometry - The codepoint list is empty.");
		glyphs[glyph_codepoints[0]] = glyph;
		return;
	}
	const size_t last_codepoint = glyph_codepoints[glyphs.size() - 1];
	BoundingBox last_bb = glyphs[last_codepoint].bb;
	const double overlap = (last_bb.xmax - bb.xmin) / last_bb.GetSizeX();

//	if (overlap >= -0.1)
//		std::cout << "Ovl = " << overlap << '\n';

	if (overlap > detail_overlap) {
		glyphs[last_codepoint].AddFrom(geometry);
		glyphs[last_codepoint].bb.Insert(bb);
		return;
	}
	if (glyphs.size() >= glyph_codepoints.size()) {
		std::cout << "Glyphs: " << glyphs.size() << '\n';
		std::cout << "Codepoints: " << glyph_codepoints.size() << '\n';
		throw std::runtime_error(
				"OpenGLFont - More glyphs in 3D-data than codepoints available.");
	}

	uint32_t next_codepoint = glyph_codepoints[glyphs.size()];
	while (next_codepoint <= 32) {
		glyphs[next_codepoint] = Glyph();
		glyphs[next_codepoint].bb = glyphs[last_codepoint].bb;
		next_codepoint = glyph_codepoints[glyphs.size()];
	}
	glyphs[next_codepoint] = glyph;
}

OpenGLFont::Glyph::Glyph(const Geometry &geometry) {
	CopyPropertiesFrom(geometry);
	AddFrom(geometry);
}

void OpenGLFont::Paint(const std::string &txt) const {
	const auto cps = UTF8::GetCodepoints(txt);
	double x = 0.0;
	double y = 0.0;
	for (const auto &cp : cps) {
		if (!glyphs.count(cp))
			continue;
		const Glyph &glyph = glyphs.at(cp);
		const BoundingBox &bb = glyph.bb;
		glPushMatrix();
		glTranslated(x, y, 0.0);
		glyph.Paint();
		glPopMatrix();
		x += bb.GetSizeX() + glyph_distance;
	}
}

BoundingBox OpenGLFont::Size(const std::string &txt) const {
	BoundingBox ret;
	const auto cps = UTF8::GetCodepoints(txt);
	double x = 0.0;
	double y = 0.0;
	for (const auto &cp : cps) {
		if (!glyphs.count(cp))
			continue;
		const Glyph &glyph = glyphs.at(cp);
		BoundingBox bb = glyph.bb;
		bb.xmin += x;
		bb.xmax += x;
		bb.ymin += y;
		bb.ymax += y;
		ret.Insert(bb);
		x += bb.GetSizeX() + glyph_distance;
	}
	return ret;
}

Vector3 OpenGLFont::GapCenter(const std::string &txt) const {
	BoundingBox spaces;
	BoundingBox all;
	const auto cps = UTF8::GetCodepoints(txt);
	double x = 0.0;
	double y = 0.0;
	bool first = true;
	for (const auto &cp : cps) {
		if (!glyphs.count(cp))
			continue;
		const Glyph &glyph = glyphs.at(cp);
		BoundingBox bb = glyph.bb;
		bb.xmin += x;
		bb.xmax += x;
		bb.ymin += y;
		bb.ymax += y;
		all.Insert(bb);

		if (first)
			first = false;
		else {
			BoundingBox gap = bb;
			gap.xmin = bb.xmin - glyph_distance;
			gap.xmax = bb.xmin;
			spaces.Insert(gap);
		}
		x += bb.GetSizeX() + glyph_distance;
	}
	if (spaces.IsEmpty())
		return Vector3(0, 0, 0);
	Vector3 ret;
	ret.x = (spaces.xmin + spaces.xmax - all.xmin - all.xmax) / 2.0;
	return ret;
}
