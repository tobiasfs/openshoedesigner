///////////////////////////////////////////////////////////////////////////////
// Name               : LastModel.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 02.03.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
// License            : GNU General Public License version 3.0 (GPLv3)
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

#ifndef OBJECT_LASTMODEL_H
#define OBJECT_LASTMODEL_H
/*!\class LastModel
 * \brief Last for shoemaking
 *
 * Alternatively this class takes over the data from the bone model and
 * provides the data to the above-mentioned steps. In this case the data
 * is not transformed and adapted. This step has already been done on the
 * foot-model.
 */

#include "ObjectGeometry.h"

#include <cmath>
#include <functional>
#include <stddef.h>
#include <string>
#include <vector>

#include "../../3D/AffineTransformMatrix.h"
#include "../../3D/Geometry.h"
#include "../../3D/Polygon3.h"
#include "../../3D/TestGrid.h"
#include "../../3D/Vector3.h"
#include "../../math/FormFinder.h"
#include "../../math/Polynomial.h"
#include "../../math/Symmetry.h"

class Insole;
class OpenGLText;
class FootMeasurements;
class Shoe;

class LastModel: public ObjectGeometry {
	friend class LastAnalyse;

public:
	LastModel();
	LastModel(const Geometry &geo);
	virtual ~LastModel() = default;

	void Transform(std::function<Vector3(Vector3)> func);
	void Mirror();

	void UpdateForm(const Insole &insole, const FootMeasurements &measurements);

	void Paint() const;
	void PaintMarker(const size_t idx, const OpenGLText &font,
			const std::string &text) const;

protected:



	Polygon3 planeXZ;
	DependentVector angleXZ;

public:

	Polygon3 bottomleft;
	Polygon3 bottomright;

	Polygon3 bottom;
	Polygon3 top;

	Polygon3 HeelGirth;
	Polygon3 WaistGirth;
	Polygon3 LittleToeGirth;
	Polygon3 BigToeGirth;

	std::vector<double> scalevalues;

	FormFinder formfinder;

	size_t idxZero = 0;
	size_t idxHeel = 0;
	size_t idxHeelPoint = 0;
	size_t idxHeelCenter = 0;
	size_t idxWaistBottom = 0;
	size_t idxLittleToeBottom = 0;
	size_t idxBigToeBottom = 0;
	size_t idxToeCenter = 0;
	size_t idxToePoint = 0;
	size_t idxToeTip = 0;
	size_t idxBigToeTop = 0;
	size_t idxLittleToeTop = 0;
	size_t idxWaistTop = 0;
	size_t idxTop = 0;

	double heela = 0.0;
	double toea = 0.0;

	DependentVector debug0;
	DependentVector debug1;
	DependentVector debug2;

	DependentVector debugA;
	DependentVector debugB;
	DependentVector debugC;

	TestGrid tg;

	double param_soleangle = 35 * M_PI / 180;

	Polynomial height08;

};

#endif /* OBJECT_LASTMODEL_H */
