///////////////////////////////////////////////////////////////////////////////
// Name               : Upper.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 23.06.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "Upper.h"
#include "../../3D/BoundingBox.h"
#include "../../3D/Polygon3.h"

#include <iostream>
#include <fstream>

void Upper::SaveSVG(const std::string &filename) const {
	std::vector<Polygon3> outlines;
	outlines.reserve(patches.size());

	BoundingBox global;
	global.SetSize(0.0, 0.0, 0.0);

	for (const Geometry &patch : patches) {
		Polygon3 outline;
		outline.ExtractOutline(patch);

		BoundingBox bb;
		for (size_t vidx = 0; vidx < outline.CountVertices(); vidx++)
			bb.Insert(outline[vidx]);

		double padding = 0.01;
		bb.xmin -= padding;
		bb.xmax += padding;
		bb.ymin -= padding;
		bb.ymax += padding;

		AffineTransformMatrix m;
		m.TranslateGlobal(-bb.xmin, -bb.ymin + global.ymax, -bb.zmin);
		outline.Transform(m);
		bb.Transform(m);
		global.Insert(bb);
		outlines.push_back(outline);
	}

	std::ofstream svg(filename);
	if (!svg.is_open()) {
		throw std::runtime_error("Could not open svg file for writing.");
	}

	svg << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
	svg << "<svg\n";
	svg << "   version=\"1.1\"\n";
	svg << "   xmlns=\"http://www.w3.org/2000/svg\"\n";
	svg << "   viewBox=\"";
	svg << global.xmin * 1000.0 << " ";
	svg << global.ymin * 1000.0 << " ";
	svg << global.GetSizeX() * 1000.0 << " ";
	svg << global.GetSizeY() * 1000.0 << "\"\n";
	svg << "   width=\"" << global.GetSizeX() * 1000.0 << "mm\"\n";
	svg << "   height=\"" << global.GetSizeY() * 1000.0 << "mm\"\n";
	svg << ">\n";

	for (const Polygon3 &outline : outlines) {
		if (outline.IsEmpty())
			continue;
		svg << "<g fill=\"none\" stroke=\"black\" stroke-width=\"0.5\">\n";
		svg << "<path d=\"M ";
		svg << outline[0].x * 1000.0 << " " << outline[0].y * 1000 << " ";
		for (size_t vidx = 1; vidx < outline.CountVertices(); vidx++) {
			svg << "L " << outline[vidx].x * 1000.0 << " "
					<< outline[vidx].y * 1000.0 << " ";
		}
		svg << "Z\" />\n";
		svg << "</g>\n";
	}
	svg << "</svg>\n";

	std::cout << "File written.\n";

}
