///////////////////////////////////////////////////////////////////////////////
// Name               : Skeleton.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 18.02.2018
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

#include "Skeleton.h"

#include "../../3D/AffineTransformMatrix.h"
#include "../../3D/Vector3.h"
#include "../../math/MathParser.h"
#include "../../system/JSON.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>

Skeleton::Skeleton(const Skeleton &other) {
	if (&other == this)
		return;
	const size_t N = other.bones.size();
	this->bones.resize(N);
	for (size_t n = 0; n < N; ++n) {
		this->bones[n] = std::make_shared<Bone>(*(other.bones[n]));
		if (!other.bones[n]->parentTo.expired()) {
			const std::string parentname = other.bones[n]->parentTo.lock()->name;

			auto it = std::find(bones.begin(), bones.end(), parentname);
			if (it != bones.end()) {
				bones[n]->parentTo = *it;
			}
		}
	}
}

Skeleton::~Skeleton() {
	if (m_gllist != 0)
		glDeleteLists(m_gllist, 1);
}

bool operator==(const std::shared_ptr<Bone> &bone, const std::string &name) {
	return name.compare(bone->name) == 0;
}

Skeleton& Skeleton::operator =(const Skeleton &other) {
	// Support function to enable std::find with std::shared_ptr
	if (&other == this)
		return *this;
	const size_t N = other.bones.size();
	this->bones.resize(N);
	for (size_t n = 0; n < N; ++n) {
		this->bones[n] = std::make_shared<Bone>(*(other.bones[n]));
		if (!other.bones[n]->parentTo.expired()) {
			const std::string parentname = other.bones[n]->parentTo.lock()->name;

			auto it = std::find(bones.begin(), bones.end(), parentname);
			if (it != bones.end()) {
				bones[n]->parentTo = *it;
			}
		}
	}
	return *this;
}

void Skeleton::Clear() {
	bones.clear();
	update = true;
}

void Skeleton::AddBone(const std::string &name) {
	if (std::find(bones.begin(), bones.end(), name) != bones.end())
		return;
	bones.push_back(std::make_shared<Bone>(name));
}

std::shared_ptr<Bone> Skeleton::GetBone(const std::string &name) {
	auto it = std::find(bones.begin(), bones.end(), name);
	if (it == bones.end())
		throw std::runtime_error(
				"std::shared_ptr <Bone> Skeleton::GetBone - Bone not found.");
	return (*it);
}

size_t Skeleton::GetBoneCount() const {
	return bones.size();
}

bool Skeleton::Connect(const std::string &parent, const std::string &child) {
	auto b1 = std::find(bones.begin(), bones.end(), parent);
	auto b2 = std::find(bones.begin(), bones.end(), child);
	if (b1 == bones.end())
		return false;
	if (b2 == bones.end())
		return false;
	(*b2)->parentTo = (*b1);
	return true;
}

void Skeleton::Update() {
	for (auto &bone : bones)
		bone->Update();
	update = true;
}

void Skeleton::UpdateBonesFromFormula(MathParser &parser) {
	for (auto &bone : bones) {
		bone->length = parser.SIFromString(bone->formulaLength);
		bone->r1 = parser.SIFromString(bone->formulaR1);
		bone->r2 = parser.SIFromString(bone->formulaR2);
		bone->s1 = parser.SIFromString(bone->formulaS1);
		bone->s2 = parser.SIFromString(bone->formulaS2);
	}
	update = true;
}

void Skeleton::ResetRotation() {
	for (auto &bone : bones)
		bone->PushRotation();
}

void Skeleton::RestoreRotation() {
	for (auto &bone : bones)
		bone->PopRotation();
}

void Skeleton::LoadJSON(std::string filename) {
	JSON js = JSON::Load(filename);
	bones.clear();
	for (size_t n = 0; n < js.Size(); ++n) {
		const std::string name = js.GetKey(n);
		const JSON &bjs = js[name];
		const JSON &bjsi = bjs["init"];
		const JSON &bjsf = bjs["formula"];
		AddBone(name);
		auto bone = std::find(bones.begin(), bones.end(), name);
		(*bone)->initialized = false;
		(*bone)->r1init = bjsi["r1"].GetNumber(0.0);
		(*bone)->r2init = bjsi["r2"].GetNumber((*bone)->r1init);
		const JSON p1vec = bjsi["p1"];
		const JSON p2vec = bjsi["p2"];
		(*bone)->p1.x = p1vec[0].GetNumber();
		(*bone)->p1.y = p1vec[1].GetNumber();
		(*bone)->p1.z = p1vec[2].GetNumber();
		if (p2vec.IsArray()) {
			(*bone)->p2.x = p2vec[0].GetNumber();
			(*bone)->p2.y = p2vec[1].GetNumber();
			(*bone)->p2.z = p2vec[2].GetNumber();
		} else {
			(*bone)->p2 = (*bone)->p1;
		}
		(*bone)->formulaLength = bjsf["length"].GetString("0");
		(*bone)->formulaR1 = bjsf["r1"].GetString("0");
		(*bone)->formulaR2 = bjsf["r2"].GetString("0");
		(*bone)->formulaS1 = bjsf["s1"].GetString("0");
		(*bone)->formulaS2 = bjsf["s2"].GetString("0");
	}

	for (size_t n = 0; n < js.Size(); ++n) {
		const std::string name = js.GetKey(n);
		const JSON &bjs = js[name];
		std::string parent = bjs["parent"].GetString();
		Connect(parent, name);
	}
	for (auto &bone : bones)
		bone->UpdateHierarchy();
	std::sort(bones.begin(), bones.end(),
			[](const std::shared_ptr<Bone> &lhs,
					const std::shared_ptr<Bone> &rhs) {
				return lhs->hierarchyLevel < rhs->hierarchyLevel;
			});

	for (auto &bone : bones) {
//		const std::string name = bone->name;
		bone->lengthinit = (bone->p1 - bone->p2).Abs();
		if (bone->parentTo.expired()) {
			if (bone->lengthinit > 0.0) {
				bone->matrixinit.SetEx(bone->p2 - bone->p1);
				bone->matrixinit.SetEy(Vector3(0, 1, 0));
				bone->matrixinit.CalculateEz();
				bone->matrixinit.CalculateEy();
			}
		} else {
			std::shared_ptr<Bone> parent = bone->parentTo.lock();
			bone->matrixinit = parent->matrixinit;
			if (bone->lengthinit > 0.0) {
				bone->matrixinit.SetEx(bone->p2 - bone->p1);
				bone->matrixinit.SetEy(Vector3(0, 1, 0));
				bone->matrixinit.CalculateEz();
				bone->matrixinit.CalculateEy();
			}
		}
		bone->matrixinit.Normalize();
		bone->matrixinit.SetOrigin(bone->p1);
	}
}

bool Skeleton::SaveJSON(std::string filename) {
	MathParser parser;

	parser.vm.heap.Set("L", MathParser::Value(1.0));
	parser.vm.heap.Set("R", MathParser::Value(1.0));
	parser.vm.heap.Set("S", MathParser::Value(1.0));
	Skeleton::UpdateBonesFromFormula(parser);
	Skeleton::Update();

	try {
		JSON js;
		js.SetObject();
		for (auto &bone : bones) {
			JSON &jsb = js[bone->name];
			jsb.SetObject();
			JSON &jsi = jsb["init"];
			jsi.SetObject();
			JSON &jsf = jsb["formula"];
			jsf.SetObject();

			if (bone->parentTo.expired()) {
				jsb["parent"].SetNull();
			} else {
				std::shared_ptr<Bone> parent = bone->parentTo.lock();
				jsb["parent"].SetString(parent->name);
			}

			jsi["r1"].SetNumber(bone->r1init);
			jsi["r2"].SetNumber(bone->r2init);

			Vector3 p1 = bone->matrixinit.GetOrigin();
			Vector3 p2 = bone->matrixinit.Transform(bone->lengthinit, 0, 0);

			JSON &jsp1 = jsi["p1"];
			jsp1.SetArray(3);
			JSON &jsp2 = jsi["p2"];
			jsp2.SetArray(3);

			jsp1[0].SetNumber(p1.x);
			jsp1[1].SetNumber(p1.y);
			jsp1[2].SetNumber(p1.z);
			jsp2[0].SetNumber(p2.x);
			jsp2[1].SetNumber(p2.y);
			jsp2[2].SetNumber(p2.z);

			jsf["length"].SetString(bone->formulaLength);
			jsf["r1"].SetString(bone->formulaR1);
			jsf["r2"].SetString(bone->formulaR2);
			jsf["s1"].SetString(bone->formulaS1);
			jsf["s2"].SetString(bone->formulaS2);
		}
		js.Save(filename);
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;
}

void Skeleton::Render() const {
	if (m_gllist == 0) {
		m_gllist = glGenLists(1);
		update = true;
	}
	if (update) {
		glNewList(m_gllist, GL_COMPILE_AND_EXECUTE);

		size_t n = 0;
		for (auto &bone : bones) {
			::glPushName(n++);
			bone->Paint();
			::glPopName();
		}
		glEndList();
		update = false;
	} else {
		glCallList(m_gllist);
	}
}

