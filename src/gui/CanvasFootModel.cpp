///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasFootModel.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 14.06.2020
// Copyright          : (C) 2020 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "CanvasFootModel.h"

#include "../3D/OpenGLMaterial.h"
CanvasFootModel::CanvasFootModel(wxWindow* parent)
		: OpenGLCanvas(parent)
{
}

void CanvasFootModel::Render()
{
	OpenGLMaterial mat1(OpenGLMaterial::Preset::Emerald);
	mat1.UseMaterial();
	model.PaintBones();
	OpenGLMaterial mat2(OpenGLMaterial::Preset::YellowRubber);
	mat2.UseMaterial(); // 0.8
	model.PaintSkin();
}

void CanvasFootModel::RenderPick()
{
}
