///////////////////////////////////////////////////////////////////////////////
// Name               : Builder.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 17.11.2024
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
#ifndef SRC_PROJECT_BUILDER_H_
#define SRC_PROJECT_BUILDER_H_
/** \class Builder
 * 	\code #include "Builder.h"\endcode
 * 	\ingroup project
 *  \brief Setup and run the process-chain for the shoe generation.
 *
 * Reads the configuration and other relevant data from the project and
 * generates a chain of operations to produce all necessary output objects.
 *
 * The generated operations are sorted and executed in the correct sequence
 * and only if necessary (by tracking all the modified-flags).
 *
 * # Implementation needed
 *
 * ## Debugging leftovers
 *
 ~~~~~{.cpp}
 Volume vol;
 OrientedMatrix xray;
 OrientedMatrix heightfield;
 Polygon3 bow;
 ~~~~~
 *
 *
 ~~~~~{.cpp}
 vol.SetSize(4, 4, 4, 0.1);
 vol.SetOrigin(Vector3(-0.15, -0.15, -0.15));

 vol.SetSize(2, 2, 2, 0.1);
 vol.SetOrigin(Vector3(-0.05, -0.05, -0.05));

 for(size_t n = 0; n < vol.Numel(); n++)
 vol[n] = 0.0;
 vol.CalcSurface();
 ~~~~~
 *
 *
 ~~~~~{.cpp}
 heightfield = footL.skin.SurfaceField();
 OrientedMatrix temp = heightfield.XRay(Volume::Method::MinValue);
 bow.Clear();
 for (unsigned int i = 0; i < temp.Numel(); i++) {
 if (temp[i] < DBL_MAX) {
 bow.InsertPoint(i * temp.dx + temp.origin.x, 0,
 temp[i] + temp.origin.z);
 }
 ~~~~~
 *
 *
 ~~~~~{.cpp}
 xray = footL.skin.XRay(Volume::Method::MeanValue);
 bow = footL.GetCenterline();
 //	bow.elements[0] = lastvol.GetSurface(bow.elements[1],
 //			(bow.elements[0] - bow.elements[1]) * 2);
 //	size_t M = bow.elements.GetCount();
 //	bow.elements[M - 1] = lastvol.GetSurface(bow.elements[M - 2],
 //			(bow.elements[M - 1] - bow.elements[M - 2]) * 2);
 //
 bow.Resample(50);
 bow.Filter(20);
 ~~~~~
 *
 * ## Designing the shoe
 *
 ~~~~~{.cpp}
 if (lastModelL.IsModified()) {
 lastModelL.Modify(false);
 insoleL.Mirror(false);
 lastModelL.UpdateForm(insoleL, measL);
 lastModelL.Mirror();
 insoleL.Mirror(true);
 //		csL.Update(measL, insoleL, lastModelL);
 return true;
 }
 std::cout << "Update L - done\n";
 return false;
 ~~~~~
 *
 *
 ~~~~~{.cpp}
 bool Project::UpdateRight() {
 wxCriticalSectionLocker locker(CSRight);
 if (insoleR.IsModified()) {
 insoleR.Modify(false);
 insoleR.Construct(shoe, measR);
 insoleR.Shape(shoe, fmax(-legLengthDifference->ToDouble(), 0));
 lastModelR.Modify(true);
 footR.ModifyPosition(true);
 return true;
 }
 if (footR.IsModifiedForm()) {
 footR.ModifyForm(false);
 //		footR.UpdateForm(measR);
 footR.ModifyPosition(true);
 return true;
 }
 if (footR.IsModifiedPosition()) {
 footR.ModifyPosition(false);
 footR.UpdatePosition(shoe, fmax(-legLengthDifference.ToDouble(), 0),
 measR.angleMixing.ToDouble());
 footR.ModifySkin(true);
 return true;
 }
 if (footR.IsModifiedSkin()) {
 footR.ModifySkin(false);
 footR.CalculateSkin();
 return true;
 }
 if (lastModelR.IsModified()) {
 lastModelR.Modify(false);
 lastModelR.UpdateForm(insoleR, measR);
 //		csR.Update(measR, insoleR, lastModelR);
 return true;
 }

 std::cout << "Update R - done\n";
 return false;
 }
 ~~~~~
 *
 *
 */

#include "operation/CoordinateSystemConstruct.h"
#include "operation/FootModelLoad.h"
#include "operation/FootModelUpdate.h"
#include "operation/FootScanLoad.h"
#include "operation/HeelCenter.h"
#include "operation/HeelConstruct.h"
#include "operation/HeelExtractInsole.h"
#include "operation/HeelNormalize.h"
#include "operation/InsoleAnalyze.h"
#include "operation/InsoleConstruct.h"
#include "operation/InsoleFlatten.h"
#include "operation/InsoleTransform.h"
#include "operation/LastAnalyse.h"
#include "operation/LastConstruct.h"
#include "operation/LastNormalize.h"
#include "operation/LastUpdate.h"
#include "operation/ObjectLoad.h"

#include "operation/Operation.h"

#include <memory>
#include <vector>

class Project;

class Builder {
public:
	Builder() = default;
	virtual ~Builder() = default;

	bool IsSetup() const;
	void Setup(Project &project);
	void Update(Project &project);

	void Paint() const;

#ifdef DEBUG
	void ToDot(std::ostream &out, const Project &project) const;
	void ToCSV(std::ostream &out) const;
#endif

	std::shared_ptr<const Matrix> GetDebugMatrix() const {
		return opHeelExtractInsole->debug;
	}

private:
	void Connect(Project &project);
	void ResetState();

public:
	std::string error;

private:
	std::vector<std::shared_ptr<Operation>> operations;

	std::shared_ptr<CoordinateSystemConstruct> opCoordinateSystemConstruct;
	std::shared_ptr<FootModelLoad> opFootModelLoad;
	std::shared_ptr<FootModelUpdate> opFootModelUpdate;
	std::shared_ptr<FootScanLoad> opFootScanLoad;
	std::shared_ptr<HeelCenter> opHeelCenter;
	std::shared_ptr<HeelConstruct> opHeelConstruct;
	std::shared_ptr<HeelExtractInsole> opHeelExtractInsole;
	std::shared_ptr<ObjectLoad> opHeelLoad;
	std::shared_ptr<HeelNormalize> opHeelNormalize;
	std::shared_ptr<InsoleAnalyze> opInsoleAnalyze;
	std::shared_ptr<InsoleConstruct> opInsoleConstruct;
	std::shared_ptr<InsoleFlatten> opInsoleFlatten;
	std::shared_ptr<InsoleTransform> opInsoleTransform;
	std::shared_ptr<LastAnalyse> opLastAnalyse;
	std::shared_ptr<LastConstruct> opLastConstruct;
	std::shared_ptr<ObjectLoad> opLastLoad;
	std::shared_ptr<LastNormalize> opLastNormalize;
	std::shared_ptr<LastUpdate> opLastUpdate;

#ifdef DEBUG
	size_t debug_count = 0;
#endif
};

#endif /* SRC_PROJECT_BUILDER_H_ */
