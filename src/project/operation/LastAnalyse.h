///////////////////////////////////////////////////////////////////////////////
// Name               : LastAnalyse.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 10.11.2024
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
#ifndef SRC_PROJECT_OPERATION_LASTANALYSE_H_
#define SRC_PROJECT_OPERATION_LASTANALYSE_H_

/**\class LastAnalyse
 * \brief Analyse a last loaded from a file.
 * \ingroup ObjectOperations
 * \code #include "LastAnalyse.h"\endcode
 *
 * The operation analysis and normalizes the form of the last.
 */

#include "../../3D/Polygon3.h"
#include "../../math/MEstimator.h"
#include "../object/LastModel.h"
#include "../ParameterEnum.h"
#include "Operation.h"

#include <memory>

class LastAnalyse: public Operation {
protected:
	struct Cut {
		Polygon3 p;
		AffineTransformMatrix m;
	};
public:
	LastAnalyse();
	virtual ~LastAnalyse() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

#ifdef DEBUG
	virtual void Paint() const override;
#endif

private:
	void AnalyseForm();

	void FindAndReorientCenterplane();
	bool FindMarker();
	void MarkMeasurements();
	void FindOutline();
	size_t FindTopPoint(size_t idx, size_t idxStart, size_t idxEnd) const;
	DependentVector OrthogonalPoint(const Vector3 &p) const;

	static double RelValAt(const DependentVector &est, double x);

	static Cut CalculateCut(const Geometry &hull, const Vector3 &p0,
			const Vector3 &p1);
	static BoundingBox CalculateBoundingBox(const Polygon3 &polygon);
	static AffineTransformMatrix RotatedScale(const Cut &c, double target);
	static bool Vector3XLess(const Vector3 a, const Vector3 b);

public:
	std::shared_ptr<ParameterEnum> lastReorient;
	std::shared_ptr<ObjectGeometry> in;
	std::shared_ptr<LastModel> out;

private:
	Polygon3 debug;
	KernelDensityEstimator kde;
	MEstimator me;

//	LastModel debug0;
//	LastModel debug1;
};

#endif /* SRC_PROJECT_OPERATION_LASTANALYSE_H_ */
