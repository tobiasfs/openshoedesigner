///////////////////////////////////////////////////////////////////////////////
// Name               : FootModel.h
// Purpose            : Bone-based model of a foot
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 13.08.2015
// Copyright          : (C) 2015 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef FOOTMODEL_H
#define FOOTMODEL_H

/*!\class FootModel
 * \brief Bone-based model of a foot
 *
 * Stores the footbones and skin parameters.
 *
 * Upon calling Setup with a FootMeasurements object, the size and position of
 * the bones and the skin are calculated.
 *
 *\dot
 * digraph classes {
 *     rankdir = LR
 *
 *	 node [fontname=Helvetica, fontsize=10];
 *
 *   Tibia -> Talus
 *   Tibia -> Fibula
 *   Talus -> Cuboideum
 *   Talus -> Naviculare
 *   Talus -> Calcaneus
 *   Talus -> Talus2
 *   Naviculare -> Cuneiforme3
 *   Naviculare -> Cuneiforme2
 *   Naviculare -> Cuneiforme1
 *   Cuboideum -> Metatarsalis1
 *   Cuboideum -> Metatarsalis2
 *   Cuneiforme3 -> Metatarsalis3
 *   Cuneiforme2 -> Metatarsalis4
 *   Cuneiforme1 -> Metatarsalis5
 *   Metatarsalis1 -> PhalanxI1
 *   Metatarsalis2 -> PhalanxI2
 *   Metatarsalis3 -> PhalanxI3
 *   Metatarsalis4 -> PhalanxI4
 *   Metatarsalis5 -> PhalanxI5
 *   PhalanxI1 -> PhalanxII1
 *   PhalanxI2 -> PhalanxII2
 *   PhalanxI3 -> PhalanxII3
 *   PhalanxI4 -> PhalanxII4
 *   PhalanxI5 -> PhalanxII5
 *   PhalanxII1 -> PhalanxIII1
 *   PhalanxII2 -> PhalanxIII2
 *   PhalanxII3 -> PhalanxIII3
 *   PhalanxII4 -> PhalanxIII4
 * }
 * \enddot
 *
 * (Talus2 is an extra bone-element to get the form of the Talus right.)
 *
 * Translation:
 * | Latin        | German            |
 * |--------------|-------------------|
 * | Tibia        | Schienbein        |
 * | Fibula       | Wadenbein         |
 * | Talus        | Sprungbein        |
 * | Calcaneus    | Fersenbein        |
 * | Cuboideum    | Wuerfelbein       |
 * | Naviculare   | Kahnbein          |
 * | Cuneiforme   | Keilbein          |
 * | Metatarsalis | Mittelfussknochen |
 *
 *
 */

#include "Skeleton.h"

#include "../../3D/AffineTransformMatrix.h"
#include "../../3D/BoundingBox.h"
#include "../../3D/Volume.h"
#include "../../math/NelderMeadOptimizer.h"
#include "../object/Object.h"
//#include "../FootMeasurements.h"

#include <string>

class FootMeasurements;
class Shoe;
class Polygon3;

class FootModel: public Skeleton, public Object {
public:

	FootModel() = default;
	virtual ~FootModel() = default;

	void Mirror();

	bool IsModifiedForm() const;
	bool IsModifiedPosition() const;
	bool IsModifiedSkin() const;
	void ModifyForm(bool modified = true);
	void ModifyPosition(bool modified = true);
	void ModifySkin(bool modified = true);

	void PaintBones() const;
	void PaintSkin() const;

//	bool LoadModel(wxTextInputStream* stream);
//	bool SaveModel(wxTextOutputStream* stream);

	void UpdateForm(const FootMeasurements &measurements);

	void CalculateBones();
	void CalculateSkin();

	Polygon3 GetCenterline() const; //!< (Re-)move?

	AffineTransformMatrix origin; //!< Origin for drawing. The origin of the model is the ankle.

	BoundingBox bounds;
	Volume skin;

	std::string filename;

	double LRS() const;

private:
//	void InitBones();

	bool modifiedForm = true;
	bool modifiedPosition = true;
	bool modifiedSkin = true;

	// Model parameter, updated by optimizer
	double L = 1.0;
	double R = 1.0;
	double S = 1.0;

	NelderMeadOptimizer optiForm;
	NelderMeadOptimizer optiPos;
};

#endif /* FOOTMODEL_H */
