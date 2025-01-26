///////////////////////////////////////////////////////////////////////////////
// Name               : FootModel.cpp
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

#include "FootModel.h"

#include "Bone.h"

#include "../../3D/Vector3.h"
#include "../../math/MathParser.h"
#include "../Configuration.h"
#include "../ParameterFormula.h"
#include "../../3D/Polygon3.h"

#include <cmath>
#include <memory>
#include <vector>

#include "../../3D/OpenGL.h"

//const unsigned int FootModel::NBones = 29;

//FootModel::FootModel()
//{
//	bones.reserve(NBones); //TODO Move away from using pointers.
//
//	Tibia = Skeleton::AddBone(_T("Tibia"));
//	Fibula = Skeleton::AddBone(_T("Fibula"));
//	Talus = Skeleton::AddBone(_T("Talus"));
//	Talus2 = Skeleton::AddBone(_T("Talus2"));
//	Calcaneus = Skeleton::AddBone(_T("Calcaneus"));
//	Cuboideum = Skeleton::AddBone(_T("Cuboideum"));
//	Naviculare = Skeleton::AddBone(_T("Naviculare"));
//	Cuneiforme1 = Skeleton::AddBone(_T("Cuneiforme1"));
//	Cuneiforme2 = Skeleton::AddBone(_T("Cuneiforme2"));
//	Cuneiforme3 = Skeleton::AddBone(_T("Cuneiforme3"));
//	Metatarsalis1 = Skeleton::AddBone(_T("Metatarsalis1"));
//	Metatarsalis2 = Skeleton::AddBone(_T("Metatarsalis2"));
//	Metatarsalis3 = Skeleton::AddBone(_T("Metatarsalis3"));
//	Metatarsalis4 = Skeleton::AddBone(_T("Metatarsalis4"));
//	Metatarsalis5 = Skeleton::AddBone(_T("Metatarsalis5"));
//
//	PhalanxI1 = Skeleton::AddBone(_T("PhalanxI1"));
//	PhalanxI2 = Skeleton::AddBone(_T("PhalanxI2"));
//	PhalanxI3 = Skeleton::AddBone(_T("PhalanxI3"));
//	PhalanxI4 = Skeleton::AddBone(_T("PhalanxI4"));
//	PhalanxI5 = Skeleton::AddBone(_T("PhalanxI5"));
//
//	PhalanxII1 = Skeleton::AddBone(_T("PhalanxII1"));
//	PhalanxII2 = Skeleton::AddBone(_T("PhalanxII2"));
//	PhalanxII3 = Skeleton::AddBone(_T("PhalanxII3"));
//	PhalanxII4 = Skeleton::AddBone(_T("PhalanxII4"));
//	PhalanxII5 = Skeleton::AddBone(_T("PhalanxII5"));
//
//	PhalanxIII1 = Skeleton::AddBone(_T("PhalanxIII1"));
//	PhalanxIII2 = Skeleton::AddBone(_T("PhalanxIII2"));
//	PhalanxIII3 = Skeleton::AddBone(_T("PhalanxIII3"));
//	PhalanxIII4 = Skeleton::AddBone(_T("PhalanxIII4"));
//
//	assert(bones.size() == NBones);
//
//	Skeleton::Connect(_T("Tibia"), _T("Fibula"));
//
//	Skeleton::Connect(_T("Tibia"), _T("Talus"));
//	Skeleton::Connect(_T("Talus"), _T("Talus2"));
//	Skeleton::Connect(_T("Talus"), _T("Calcaneus"));
//
//	Skeleton::Connect(_T("Talus"), _T("Cuboideum"));
//	Skeleton::Connect(_T("Talus"), _T("Naviculare"));
//	Skeleton::Connect(_T("Naviculare"), _T("Cuneiforme1"));
//	Skeleton::Connect(_T("Naviculare"), _T("Cuneiforme2"));
//	Skeleton::Connect(_T("Naviculare"), _T("Cuneiforme3"));
//
//	Skeleton::Connect(_T("Cuboideum"), _T("Metatarsalis1"));
//	Skeleton::Connect(_T("Cuboideum"), _T("Metatarsalis2"));
//
//	// Yes, this is correct: 3->3, 4->2, 5->1
//	Skeleton::Connect(_T("Cuneiforme3"), _T("Metatarsalis3"));
//	Skeleton::Connect(_T("Cuneiforme2"), _T("Metatarsalis4"));
//	Skeleton::Connect(_T("Cuneiforme1"), _T("Metatarsalis5"));
//
//	Skeleton::Connect(_T("Metatarsalis1"), _T("PhalanxI1"));
//	Skeleton::Connect(_T("Metatarsalis2"), _T("PhalanxI2"));
//	Skeleton::Connect(_T("Metatarsalis3"), _T("PhalanxI3"));
//	Skeleton::Connect(_T("Metatarsalis4"), _T("PhalanxI4"));
//	Skeleton::Connect(_T("Metatarsalis5"), _T("PhalanxI5"));
//
//	Skeleton::Connect(_T("PhalanxI1"), _T("PhalanxII1"));
//	Skeleton::Connect(_T("PhalanxI2"), _T("PhalanxII2"));
//	Skeleton::Connect(_T("PhalanxI3"), _T("PhalanxII3"));
//	Skeleton::Connect(_T("PhalanxI4"), _T("PhalanxII4"));
//	Skeleton::Connect(_T("PhalanxI5"), _T("PhalanxII5"));
//
//	Skeleton::Connect(_T("PhalanxII1"), _T("PhalanxIII1"));
//	Skeleton::Connect(_T("PhalanxII2"), _T("PhalanxIII2"));
//	Skeleton::Connect(_T("PhalanxII3"), _T("PhalanxIII3"));
//	Skeleton::Connect(_T("PhalanxII4"), _T("PhalanxIII4"));
//
//
//	modifiedForm = true;
//	modifiedPosition = true;
//	modifiedSkin = true;
//
//}

void FootModel::PaintBones() const {
	glPushMatrix();
	origin.GLMultMatrix();
	Skeleton::Render();
	glPopMatrix();
}

void FootModel::PaintSkin() const {
	glPushMatrix();
	origin.GLMultMatrix();
	skin.PaintSurface();
	glPopMatrix();
}

//void FootModel::InitBones()
//{
//	Tibia->anchorD = 0.0143493;
//	Tibia->anchorN.Set(0.00240981, 0.00711795, 0);
//	Tibia->link.Set(0, 0, 0);
//	Tibia->normal.Set(-0.00398669, 0.0946922, 0.995499);
//	Tibia->length = 0.0778566;
//	Tibia->r1 = 0.0252857;
//	Tibia->r2 = 0.0143908;
//	Tibia->s1 = 0.01;
//	Tibia->s2 = 0.01;
//	Fibula->anchorD = 1;
//	Fibula->anchorN.Set(0, 0, 0);
//	Fibula->link.Set(-0.358608, 0.556751, -2.92391);
//	Fibula->normal.Set(-0.0740036, 0.0567033, 0.995645);
//	Fibula->length = 0.112671;
//	Fibula->r1 = 0.0139018;
//	Fibula->r2 = 0.00654067;
//	Fibula->s1 = 0.01;
//	Fibula->s2 = 0.01;
//	Talus->anchorD = -0.0175868;
//	Talus->anchorN.Set(-0.167854, 0.144765, -0.0144423);
//	Talus->link.Set(0.102541, -0.109632, -0.525473);
//	Talus->normal.Set(-0.364068, 0.371839, -0.853926);
//	Talus->length = 0.0290728;
//	Talus->r1 = 0.0229317;
//	Talus->r2 = 0.0292152;
//	Talus->s1 = 0.01;
//	Talus->s2 = 0.01;
//	Talus2->anchorD = 1;
//	Talus2->anchorN.Set(-7.50496e-17, 0, 0);
//	Talus2->link.Set(0.793716, -0.331365, 0.254379);
//	Talus2->normal.Set(-0.552159, 0.39296, -0.735325);
//	Talus2->length = 0.0267382;
//	Talus2->r1 = 0.019513;
//	Talus2->r2 = 0.019083;
//	Talus2->s1 = 0.01;
//	Talus2->s2 = 0.01;
//	Calcaneus->anchorD = 1;
//	Calcaneus->anchorN.Set(-7.50496e-17, 0, 0);
//	Calcaneus->link.Set(-0.447956, 0.177577, -0.346443);
//	Calcaneus->normal.Set(0.0523509, -0.557595, -0.828461);
//	Calcaneus->length = 0.0175793;
//	Calcaneus->r1 = 0.0271975;
//	Calcaneus->r2 = 0.0165503;
//	Calcaneus->s1 = 0.01;
//	Calcaneus->s2 = 0.01;
//	Cuboideum->anchorD = 0.284221;
//	Cuboideum->anchorN.Set(0.690085, 0.170781, -0.219849);
//	Cuboideum->link.Set(0.747911, 0.51343, -0.201065);
//	Cuboideum->normal.Set(-0.916225, -0.104622, -0.386763);
//	Cuboideum->length = 0.0258761;
//	Cuboideum->r1 = 0.0183236;
//	Cuboideum->r2 = 0.0104453;
//	Cuboideum->s1 = 0.01;
//	Cuboideum->s2 = 0.01;
//	Naviculare->anchorD = 0.021979;
//	Naviculare->anchorN.Set(1.51782, -0.265032, -0.762522);
//	Naviculare->link.Set(0.124651, -0.0215898, -0.0781941);
//	Naviculare->normal.Set(0, 0, 1);
//	Naviculare->length = 1e-06;
//	Naviculare->r1 = 0.0217206;
//	Naviculare->r2 = 0.0217206;
//	Naviculare->s1 = 0.01;
//	Naviculare->s2 = 0.01;
//	Cuneiforme1->anchorD = 4903.23;
//	Cuneiforme1->anchorN.Set(-0.0808979, -0.351837, 0);
//	Cuneiforme1->link.Set(0.939791, 0.163393, -0.252929);
//	Cuneiforme1->normal.Set(0, 0, 1);
//	Cuneiforme1->length = 1e-06;
//	Cuneiforme1->r1 = 0.0167164;
//	Cuneiforme1->r2 = 0.0167164;
//	Cuneiforme1->s1 = 0.01;
//	Cuneiforme1->s2 = 0.01;
//	Cuneiforme2->anchorD = 3492.37;
//	Cuneiforme2->anchorN.Set(0.110719, 0.0411397, 0);
//	Cuneiforme2->link.Set(0.579158, 0.140203, -0.150428);
//	Cuneiforme2->normal.Set(0, 0, 1);
//	Cuneiforme2->length = 1e-06;
//	Cuneiforme2->r1 = 0.018744;
//	Cuneiforme2->r2 = 0.018744;
//	Cuneiforme2->s1 = 0.01;
//	Cuneiforme2->s2 = 0.01;
//	Cuneiforme3->anchorD = 1093.4;
//	Cuneiforme3->anchorN.Set(-0.0159425, 0.401639, 0);
//	Cuneiforme3->link.Set(0.575879, 0.379473, -0.0282063);
//	Cuneiforme3->normal.Set(0, 0, 1);
//	Cuneiforme3->length = 1e-06;
//	Cuneiforme3->r1 = 0.0173153;
//	Cuneiforme3->r2 = 0.0173153;
//	Cuneiforme3->s1 = 0.01;
//	Cuneiforme3->s2 = 0.01;
//	Metatarsalis1->anchorD = -0.0221462;
//	Metatarsalis1->anchorN.Set(-0.150224, 0.393776, 0.249356);
//	Metatarsalis1->link.Set(0.380148, 0.427998, -0.0852047);
//	Metatarsalis1->normal.Set(0.880389, 0.0436524, -0.472239);
//	Metatarsalis1->length = 0.0648639;
//	Metatarsalis1->r1 = 0.00865907;
//	Metatarsalis1->r2 = 0.00556117;
//	Metatarsalis1->s1 = 0.01;
//	Metatarsalis1->s2 = 0.01;
//	Metatarsalis2->anchorD = -0.293424;
//	Metatarsalis2->anchorN.Set(0.0110624, 0.111926, -0.0564829);
//	Metatarsalis2->link.Set(0.569394, 0.137734, -0.16996);
//	Metatarsalis2->normal.Set(0.904266, 0.061991, -0.422445);
//	Metatarsalis2->length = 0.0654811;
//	Metatarsalis2->r1 = 0.00698005;
//	Metatarsalis2->r2 = 0.00613493;
//	Metatarsalis2->s1 = 0.01;
//	Metatarsalis2->s2 = 0.01;
//	Metatarsalis3->anchorD = -1065.96;
//	Metatarsalis3->anchorN.Set(0.263858, 0.0529254, 0);
//	Metatarsalis3->link.Set(0.469229, 0.141451, -0.195523);
//	Metatarsalis3->normal.Set(0.919196, 0.0653173, -0.388347);
//	Metatarsalis3->length = 0.0632905;
//	Metatarsalis3->r1 = 0.00782647;
//	Metatarsalis3->r2 = 0.00572615;
//	Metatarsalis3->s1 = 0.01;
//	Metatarsalis3->s2 = 0.01;
//	Metatarsalis4->anchorD = 2940.78;
//	Metatarsalis4->anchorN.Set(0.151374, 0.0284331, 0);
//	Metatarsalis4->link.Set(0.532047, 0.138633, -0.250448);
//	Metatarsalis4->normal.Set(0.913255, 0.0666341, -0.401903);
//	Metatarsalis4->length = 0.0603107;
//	Metatarsalis4->r1 = 0.00936035;
//	Metatarsalis4->r2 = 0.00654676;
//	Metatarsalis4->s1 = 0.01;
//	Metatarsalis4->s2 = 0.01;
//	Metatarsalis5->anchorD = 2207.58;
//	Metatarsalis5->anchorN.Set(-0.171461, -0.160574, 0);
//	Metatarsalis5->link.Set(0.587347, 0.00514523, -0.227436);
//	Metatarsalis5->normal.Set(0.88909, -0.169428, -0.425221);
//	Metatarsalis5->length = 0.0518372;
//	Metatarsalis5->r1 = 0.0125943;
//	Metatarsalis5->r2 = 0.0134721;
//	Metatarsalis5->s1 = 0.01;
//	Metatarsalis5->s2 = 0.01;
//	PhalanxI1->anchorD = 1;
//	PhalanxI1->anchorN.Set(0, 0, 0);
//	PhalanxI1->link.Set(0.639832, 0.119222, -0.188153);
//	PhalanxI1->normal.Set(0.997245, 0.0634209, -0.0384615);
//	PhalanxI1->length = 0.0198607;
//	PhalanxI1->r1 = 0.00389884;
//	PhalanxI1->r2 = 0.00351277;
//	PhalanxI1->s1 = 0.01;
//	PhalanxI1->s2 = 0.01;
//	PhalanxI2->anchorD = 1;
//	PhalanxI2->anchorN.Set(0, 0, -2.0041e-15);
//	PhalanxI2->link.Set(0.710841, 0.0517571, -0.346671);
//	PhalanxI2->normal.Set(0.99332, -0.0219117, -0.113297);
//	PhalanxI2->length = 0.020395;
//	PhalanxI2->r1 = 0.00386744;
//	PhalanxI2->r2 = 0.00347469;
//	PhalanxI2->s1 = 0.01;
//	PhalanxI2->s2 = 0.01;
//	PhalanxI3->anchorD = 1;
//	PhalanxI3->anchorN.Set(0, 0, 0);
//	PhalanxI3->link.Set(0.613531, 0.0633435, -0.153156);
//	PhalanxI3->normal.Set(0.985379, -0.0910715, -0.143991);
//	PhalanxI3->length = 0.0237361;
//	PhalanxI3->r1 = 0.00467427;
//	PhalanxI3->r2 = 0.00360121;
//	PhalanxI3->s1 = 0.01;
//	PhalanxI3->s2 = 0.01;
//	PhalanxI4->anchorD = 1;
//	PhalanxI4->anchorN.Set(0, 0, 0);
//	PhalanxI4->link.Set(0.589528, 0.00348011, -0.0698836);
//	PhalanxI4->normal.Set(0.999602, 0, 0.0282182);
//	PhalanxI4->length = 0.0304066;
//	PhalanxI4->r1 = 0.00615657;
//	PhalanxI4->r2 = 0.00460484;
//	PhalanxI4->s1 = 0.01;
//	PhalanxI4->s2 = 0.01;
//	PhalanxI5->anchorD = 1;
//	PhalanxI5->anchorN.Set(0, -1.37243e-16, 0);
//	PhalanxI5->link.Set(0.503154, -0.0540011, -0.0848098);
//	PhalanxI5->normal.Set(0.993706, 0.102251, 0.0457563);
//	PhalanxI5->length = 0.0372071;
//	PhalanxI5->r1 = 0.011416;
//	PhalanxI5->r2 = 0.00972755;
//	PhalanxI5->s1 = 0.01;
//	PhalanxI5->s2 = 0.01;
//	PhalanxII1->anchorD = 1;
//	PhalanxII1->anchorN.Set(0, 0, 0);
//	PhalanxII1->link.Set(0.80599, -0.0732716, -0.219815);
//	PhalanxII1->normal.Set(0.952523, -0.09218, -0.290177);
//	PhalanxII1->length = 0.00418865;
//	PhalanxII1->r1 = 0.00313488;
//	PhalanxII1->r2 = 0.00300183;
//	PhalanxII1->s1 = 0.01;
//	PhalanxII1->s2 = 0.01;
//	PhalanxII2->anchorD = 1;
//	PhalanxII2->anchorN.Set(0, 0, 0);
//	PhalanxII2->link.Set(0.724642, -0.0813872, -0.0761605);
//	PhalanxII2->normal.Set(0.990229, -0.107434, -0.0889097);
//	PhalanxII2->length = 0.00868533;
//	PhalanxII2->r1 = 0.0029785;
//	PhalanxII2->r2 = 0.00249033;
//	PhalanxII2->s1 = 0.01;
//	PhalanxII2->s2 = 0.01;
//	PhalanxII3->anchorD = 1;
//	PhalanxII3->anchorN.Set(0, 0, 0);
//	PhalanxII3->link.Set(0.709316, -0.0226388, -0.149371);
//	PhalanxII3->normal.Set(0.995856, -0.048071, -0.0772028);
//	PhalanxII3->length = 0.011577;
//	PhalanxII3->r1 = 0.00366832;
//	PhalanxII3->r2 = 0.00339932;
//	PhalanxII3->s1 = 0.01;
//	PhalanxII3->s2 = 0.01;
//	PhalanxII4->anchorD = 1;
//	PhalanxII4->anchorN.Set(0, 0, 0);
//	PhalanxII4->link.Set(0.648185, -0.0829682, 0.0215058);
//	PhalanxII4->normal.Set(0.965172, -0.0876323, 0.246502);
//	PhalanxII4->length = 0.0141649;
//	PhalanxII4->r1 = 0.00386467;
//	PhalanxII4->r2 = 0.00362472;
//	PhalanxII4->s1 = 0.01;
//	PhalanxII4->s2 = 0.01;
//	PhalanxII5->anchorD = 1;
//	PhalanxII5->anchorN.Set(0, 0, 0);
//	PhalanxII5->link.Set(0.381906, 0.0813048, 0.0877222);
//	PhalanxII5->normal.Set(0.932687, 0.183921, 0.31027);
//	PhalanxII5->length = 0.0229436;
//	PhalanxII5->r1 = 0.00740043;
//	PhalanxII5->r2 = 0.00520677;
//	PhalanxII5->s1 = 0.01;
//	PhalanxII5->s2 = 0.01;
//	PhalanxIII1->anchorD = 1;
//	PhalanxIII1->anchorN.Set(0, 0, 0);
//	PhalanxIII1->link.Set(0.708613, -0.150312, 0);
//	PhalanxIII1->normal.Set(0.985525, -0.166794, 0.0303394);
//	PhalanxIII1->length = 0.00848434;
//	PhalanxIII1->r1 = 0.00285936;
//	PhalanxIII1->r2 = 0.00198959;
//	PhalanxIII1->s1 = 0.01;
//	PhalanxIII1->s2 = 0.01;
//	PhalanxIII2->anchorD = 1;
//	PhalanxIII2->anchorN.Set(0, 0, 0);
//	PhalanxIII2->link.Set(0.727318, -0.0233147, -0.0693038);
//	PhalanxIII2->normal.Set(0.976722, -0.149084, -0.154231);
//	PhalanxIII2->length = 0.00752188;
//	PhalanxIII2->r1 = 0.00257118;
//	PhalanxIII2->r2 = 0.00191079;
//	PhalanxIII2->s1 = 0.01;
//	PhalanxIII2->s2 = 0.01;
//	PhalanxIII3->anchorD = 1;
//	PhalanxIII3->anchorN.Set(0, 0, 0);
//	PhalanxIII3->link.Set(0.660875, -0.0799867, -0.107049);
//	PhalanxIII3->normal.Set(0.97882, -0.0362503, -0.201488);
//	PhalanxIII3->length = 0.00930337;
//	PhalanxIII3->r1 = 0.00329245;
//	PhalanxIII3->r2 = 0.00223042;
//	PhalanxIII3->s1 = 0.01;
//	PhalanxIII3->s2 = 0.01;
//	PhalanxIII4->anchorD = 1;
//	PhalanxIII4->anchorN.Set(0, 0, 0);
//	PhalanxIII4->link.Set(0.628021, -0.0702913, 0.139392);
//	PhalanxIII4->normal.Set(0.968221, -0.0525498, 0.244512);
//	PhalanxIII4->length = 0.00852905;
//	PhalanxIII4->r1 = 0.00361931;
//	PhalanxIII4->r2 = 0.00288448;
//	PhalanxIII4->s1 = 0.01;
//	PhalanxIII4->s2 = 0.01;
//}

bool FootModel::LoadModel(std::istream &stream) {

//	if (!Tibia->Set(stream->ReadLine()))
//		return false;
//	if (!Fibula->Set(stream->ReadLine()))
//		return false;
//	if (!Talus->Set(stream->ReadLine()))
//		return false;
//	if (!Talus2->Set(stream->ReadLine()))
//		return false;
//	if (!Calcaneus->Set(stream->ReadLine()))
//		return false;
//	if (!Cuboideum->Set(stream->ReadLine()))
//		return false;
//	if (!Naviculare->Set(stream->ReadLine()))
//		return false;
//	if (!Cuneiforme1->Set(stream->ReadLine()))
//		return false;
//	if (!Cuneiforme2->Set(stream->ReadLine()))
//		return false;
//	if (!Cuneiforme3->Set(stream->ReadLine()))
//		return false;
//	if (!Metatarsalis1->Set(stream->ReadLine()))
//		return false;
//	if (!Metatarsalis2->Set(stream->ReadLine()))
//		return false;
//	if (!Metatarsalis3->Set(stream->ReadLine()))
//		return false;
//	if (!Metatarsalis4->Set(stream->ReadLine()))
//		return false;
//	if (!Metatarsalis5->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxI1->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxI2->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxI3->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxI4->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxI5->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxII1->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxII2->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxII3->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxII4->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxII5->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxIII1->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxIII2->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxIII3->Set(stream->ReadLine()))
//		return false;
//	if (!PhalanxIII4->Set(stream->ReadLine()))
//		return false;

	CalculateBones();

	return true;
}

bool FootModel::SaveModel(std::ostream &stream) {
//	stream->WriteString(Tibia->Get());
//	stream->WriteString(Fibula->Get());
//	stream->WriteString(Talus->Get());
//	stream->WriteString(Talus2->Get());
//	stream->WriteString(Calcaneus->Get());
//	stream->WriteString(Cuboideum->Get());
//	stream->WriteString(Naviculare->Get());
//	stream->WriteString(Cuneiforme1->Get());
//	stream->WriteString(Cuneiforme2->Get());
//	stream->WriteString(Cuneiforme3->Get());
//	stream->WriteString(Metatarsalis1->Get());
//	stream->WriteString(Metatarsalis2->Get());
//	stream->WriteString(Metatarsalis3->Get());
//	stream->WriteString(Metatarsalis4->Get());
//	stream->WriteString(Metatarsalis5->Get());
//	stream->WriteString(PhalanxI1->Get());
//	stream->WriteString(PhalanxI2->Get());
//	stream->WriteString(PhalanxI3->Get());
//	stream->WriteString(PhalanxI4->Get());
//	stream->WriteString(PhalanxI5->Get());
//	stream->WriteString(PhalanxII1->Get());
//	stream->WriteString(PhalanxII2->Get());
//	stream->WriteString(PhalanxII3->Get());
//	stream->WriteString(PhalanxII4->Get());
//	stream->WriteString(PhalanxII5->Get());
//	stream->WriteString(PhalanxIII1->Get());
//	stream->WriteString(PhalanxIII2->Get());
//	stream->WriteString(PhalanxIII3->Get());
//	stream->WriteString(PhalanxIII4->Get());
	return true;
}

void FootModel::Mirror() {
	for (auto &bone : bones) {
		bone->p1.y = -bone->p1.y;
		bone->p2.y = -bone->p2.y;
		bone->matrixinit.ScaleGlobal(1, -1, 1);
		bone->matrixinit.UpdateOrientation();
	}
}

//TODO: Remove if not used: FootModel::UpdateForm
void FootModel::UpdateFormx(const FootMeasurements &measurements) {
	std::shared_ptr<Bone> PhalanxIII4 = GetBone("PhalanxIII4");
	std::shared_ptr<Bone> Calcaneus = GetBone("Calcaneus");
	ResetRotation();
	optiForm.reevalBest = true;
	optiForm.keepSimplex = false;
	optiForm.simplexSpread = 0.01;
	optiForm.evalLimit = 30;
	optiForm.errorLimit = 0.0005;
	optiForm.param.resize(1, 0.0);
	optiForm.param[0] = L;
	optiForm.Start();
	while (optiForm.IsRunning()) {
		L = optiForm.param[0];
		CalculateBones();
		optiForm.SetError(
				std::fabs(
						(PhalanxIII4->GetXMax() - Calcaneus->GetXMin())
								- measurements.footLength->ToDouble()));
	}
	RestoreRotation();
	ModifyForm(true);
}

void FootModel::CalculateBones() {
	MathParser parser;

	parser.vm.heap.Set("L", MathParser::Value(L));
	parser.vm.heap.Set("R", MathParser::Value(R));
	parser.vm.heap.Set("S", MathParser::Value(S));
	Skeleton::UpdateBonesFromFormula(parser);
	Skeleton::Update();
}

void FootModel::CalculateSkin() {
	BoundingBox bb;
	for (auto &bone : bones) {
		bb.Insert(
				Vector3(bone->p1.x - bone->r1 - 3 * bone->s1,
						bone->p1.y - bone->r1 - 3 * bone->s1,
						bone->p1.z - bone->r1 - 3 * bone->s1));
		bb.Insert(
				Vector3(bone->p2.x - bone->r2 - 3 * bone->s2,
						bone->p2.y - bone->r2 - 3 * bone->s2,
						bone->p2.z - bone->r2 - 3 * bone->s2));
		bb.Insert(
				Vector3(bone->p1.x + bone->r1 + 3 * bone->s1,
						bone->p1.y + bone->r1 + 3 * bone->s1,
						bone->p1.z + bone->r1 + 0 * bone->s1));
		bb.Insert(
				Vector3(bone->p2.x + bone->r2 + 3 * bone->s2,
						bone->p2.y + bone->r2 + 3 * bone->s2,
						bone->p2.z + bone->r2 + 0 * bone->s2));
	}

	const double cellvol = bb.GetVolume() / 50000.0;
	const double res = cbrt(cellvol);
	skin.SetExtent(bb.GetSizeX(), bb.GetSizeY(), bb.GetSizeZ(), res);
	skin.SetOrigin(Vector3(bb.xmin, bb.ymin, bb.zmin));
	//	volume.AddHalfplane(Vector3(0, 0, 1), -0.10, 0.01);
	//	volume.AddSphere(Vector3(0, 0.1, 0), 0.15, 0.1);
	//	volume.AddSphere(Vector3(0.13, 0, 0.0), 0.19, 0.01);
	//	volume.AddCylinder(Vector3(0.0, 0, 0.0), Vector3(0.05 * 1, 0, -0.00), 0.04,
	//			0.04, 0.02, 0.04);
	skin.Clear();

	for (auto &bone : bones) {
		skin.AddCylinder(bone->p1, bone->p2, bone->r1 + bone->s1,
				bone->r2 + bone->s2, (bone->s1 + bone->s2) / 2.0);
		//	volume->AddCylinder(footL.bones[n].p1,footL.bones[n].p2,footL.bones[n].r1,footL.bones[n].r2,footL.bones[n].s1,footL.bones[n].s2);
	}

//	footL.AddToVolume(&volume);
//	volume.Rotate(Volume::Z, 2);
//	volume.Rotate(Volume::Z, -1);
//	volume.Rotate(Volume::Z, -1);
	skin.CalcSurface();
	ModifySkin(true);
}

Polygon3 FootModel::GetCenterline() const {
	Polygon3 temp;

//	temp.InsertPoint(
//			Calcaneus->p1.x
//					+ (-Calcaneus->r1 - Calcaneus->s1) * cos(Talus->roty - 0.4),
//			Calcaneus->p1.y,
//			Calcaneus->p1.z
//					- (-Calcaneus->r1 - Calcaneus->s1)
//							* sin(Talus->roty - 0.4));
//
//	temp.InsertPoint(Calcaneus->p1);
//	temp.InsertPoint(Talus->p2);
//	temp.InsertPoint(Naviculare->p1);
//	temp.InsertPoint(Metatarsalis4->p1);
//	temp.InsertPoint(PhalanxI4->p1);
//	temp.InsertPoint(PhalanxIII4->p2);
//
//	temp.InsertPoint(
//			PhalanxIII4->p1
//					+ (PhalanxIII4->p2 - PhalanxIII4->p1)
//							* (1
//									+ (PhalanxIII4->r2 + PhalanxIII4->s2)
//											/ (PhalanxIII4->p2 - PhalanxIII4->p1).Abs()));

	return temp;
}

bool FootModel::IsModifiedForm() const {
	return modifiedForm;
}

bool FootModel::IsModifiedPosition() const {
	return modifiedPosition;
}

bool FootModel::IsModifiedSkin() const {
	return modifiedSkin;
}

void FootModel::ModifyForm(bool modified) {
	modifiedForm = modified;
}

void FootModel::ModifyPosition(bool modified) {
	modifiedPosition = modified;
}

void FootModel::ModifySkin(bool modified) {
	modifiedSkin = modified;
}

double FootModel::LRS() const {
	return L * R * S;
}
