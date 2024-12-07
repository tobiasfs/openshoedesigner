///////////////////////////////////////////////////////////////////////////////
// Name               : Exporter.h
// Purpose            : Exports objects to .mat files
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 26.06.2022
// Copyright          : (C) 2022 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef MATH_EXPORTER_H
#define MATH_EXPORTER_H

/*!\class Exporter
 * \brief Adapter To export objects to a MatlabFile
 *
 * Exports objects to a Matlab .mat file for testing and processing
 * in Octave/Matlab.
 *
 * This class was written to not pollute the namespaces of MatlabFile or of the
 * respective objects.
 */

#include "MatlabFile.h"

#include <initializer_list>
#include <string>

class DependentVector;
class Polygon3;
class Geometry;

class Exporter: public MatlabFile {
public:
	explicit Exporter(const std::string &filename_, const std::string &prefix_ =
			"");

	void Add(const DependentVector &vector, const std::string &name = "");
	void Add(const Polygon3 &polygon, const std::string &name = "");
	void Add(const Geometry &geo, const std::string &name = "");

	void Add(const std::initializer_list<double> &values,
			const std::string &name);
	void Add(const std::initializer_list<double> &values0,
			const std::initializer_list<double> &values1,
			const std::string &name);

protected:

	std::string GenName(const std::string &name_);

	std::string prefix;
	int count = 0;
};

#endif /* MATH_EXPORTER_H */
