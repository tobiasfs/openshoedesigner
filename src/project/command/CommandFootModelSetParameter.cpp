///////////////////////////////////////////////////////////////////////////////
// Name               : CommandFootModelSetParameter.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 19.02.2018
// Copyright          : (C) 2018 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "CommandFootModelSetParameter.h"

//#include <wx/event.h>
//#include "../../gui/IDs.h"

CommandFootModelSetParameter::CommandFootModelSetParameter(const wxString &name,
		Project *project, size_t boneNr, Type field, std::string newFormula) :
		wxCommand(true, name) {
	this->project = project;
	this->activeSide = ProjectView::Side::Both;
//	if(project->symmetry == Project::Full
//			|| project->symmetry == Project::OnlyModel) this->activeSide =
//			Project::Both;
	this->boneNr = boneNr;
	this->field = field;
	this->newFormula = newFormula;
}

bool CommandFootModelSetParameter::Do() {
	if (project == NULL)
		return false;
	return false;
//	if(activeSide == Project::Left || activeSide == Project::Both){
//		if(boneNr >= project->footL.bones.size()) return false;
//		switch(field){
//
//		// BoneLength
//		case Bone::stringIdentifier::stringLength:
//			oldFormulaL = project->footL.bones[boneNr].lengthv;
//			project->footL.bones[boneNr].lengthv = newFormula;
//			break;
//		case Bone::stringIdentifier::stringR1:
//			oldFormulaL = project->footL.bones[boneNr].r1v;
//			project->footL.bones[boneNr].r1v = newFormula;
//			break;
//		case Bone::stringIdentifier::stringR2:
//			oldFormulaL = project->footL.bones[boneNr].r2v;
//			project->footL.bones[boneNr].r2v = newFormula;
//			break;
//		case Bone::stringIdentifier::stringS1:
//			oldFormulaL = project->footL.bones[boneNr].s1v;
//			project->footL.bones[boneNr].s1v = newFormula;
//			break;
//		case Bone::stringIdentifier::stringS2:
//			oldFormulaL = project->footL.bones[boneNr].s2v;
//			project->footL.bones[boneNr].s2v = newFormula;
//			break;
////	case 3:{
////		// Leg
////
////		break;}
//		default:
//			return false;
//		}
//	}
//	if(activeSide == Project::Right || activeSide == Project::Both){
//		if(boneNr >= project->footR.bones.size()) return false;
//		switch(field){
//
//		// BoneLength
//		case Bone::stringIdentifier::stringLength:
//			oldFormulaR = project->footR.bones[boneNr].lengthv;
//			project->footR.bones[boneNr].lengthv = newFormula;
//			break;
//		case Bone::stringIdentifier::stringR1:
//			oldFormulaR = project->footR.bones[boneNr].r1v;
//			project->footR.bones[boneNr].r1v = newFormula;
//			break;
//		case Bone::stringIdentifier::stringR2:
//			oldFormulaR = project->footR.bones[boneNr].r2v;
//			project->footR.bones[boneNr].r2v = newFormula;
//			break;
//		case Bone::stringIdentifier::stringS1:
//			oldFormulaR = project->footR.bones[boneNr].s1v;
//			project->footR.bones[boneNr].s1v = newFormula;
//			break;
//		case Bone::stringIdentifier::stringS2:
//			oldFormulaR = project->footR.bones[boneNr].s2v;
//			project->footR.bones[boneNr].s2v = newFormula;
//			break;
//			//	case 3:{
//			//		// Leg
//			//
//			//		break;}
//		default:
//			return false;
//		}
//	}
//
//	project->Update(Project::UpdateFoot, activeSide);
	return true;
}

bool CommandFootModelSetParameter::Undo() {
	if (project == NULL)
		return false;
	return false;
//	if(activeSide == Project::Left || activeSide == Project::Both){
//
//		if(boneNr >= project->footL.bones.size()) return false;
//		switch(field){
//		case Bone::stringIdentifier::stringLength:
//			project->footL.bones[boneNr].lengthv = oldFormulaL;
//			break;
//		case Bone::stringIdentifier::stringR1:
//			project->footL.bones[boneNr].r1v = oldFormulaL;
//			break;
//		case Bone::stringIdentifier::stringR2:
//			project->footL.bones[boneNr].r2v = oldFormulaL;
//			break;
//		case Bone::stringIdentifier::stringS1:
//			project->footL.bones[boneNr].s1v = oldFormulaL;
//			break;
//		case Bone::stringIdentifier::stringS2:
//			project->footL.bones[boneNr].s2v = oldFormulaL;
//			break;
//
////	case 3:{
////		// Leg
////
////		break;}
//
//		default:
//			return false;
//		}
//	}
//
//	if(activeSide == Project::Right || activeSide == Project::Both){
//
//		if(boneNr >= project->footR.bones.size()) return false;
//		switch(field){
//		case Bone::stringIdentifier::stringLength:
//			project->footR.bones[boneNr].lengthv = oldFormulaR;
//			break;
//		case Bone::stringIdentifier::stringR1:
//			project->footR.bones[boneNr].r1v = oldFormulaR;
//			break;
//		case Bone::stringIdentifier::stringR2:
//			project->footR.bones[boneNr].r2v = oldFormulaR;
//			break;
//		case Bone::stringIdentifier::stringS1:
//			project->footR.bones[boneNr].s1v = oldFormulaR;
//			break;
//		case Bone::stringIdentifier::stringS2:
//			project->footR.bones[boneNr].s2v = oldFormulaR;
//			break;
//
////	case 3:{
////		// Leg
////
////		break;}
//
//		default:
//			return false;
//		}
//	}
//
//	project->Update(Project::UpdateFoot, activeSide);
//	return true;
}
