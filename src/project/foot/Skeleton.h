///////////////////////////////////////////////////////////////////////////////
// Name               : Skeleton.h
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

#ifndef PROJECT_FOOT_SKELETON_H
#define PROJECT_FOOT_SKELETON_H

/*!\class Skeleton
 * \brief Collection of bones.
 *
 * Bones are enumerated for easier handling. A std::map<std::string, Bone> would be cleaner, but
 * in the end more difficult to use.
 */

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Bone.h"
#include "../../3D/OpenGL.h"

class MathParser;
class Skeleton {
public:
	Skeleton() = default;
	virtual ~Skeleton();
	Skeleton(const Skeleton &other);
	Skeleton& operator=(const Skeleton &other);

	void Clear();
	void AddBone(const std::string &name);
	std::shared_ptr<Bone> GetBone(const std::string &name);
	size_t GetBoneCount() const;

	bool Connect(const std::string &parent, const std::string &child);
	void Update();
	void UpdateBonesFromFormula(MathParser &parser);
	void ResetRotation();
	void RestoreRotation();

	void LoadJSON(std::string filename);
	bool SaveJSON(std::string filename);

	void Render() const;

protected:
	std::vector<std::shared_ptr<Bone>> bones;

private:
	mutable GLuint m_gllist = 0;
	mutable bool update = true;
};

#endif /* PROJECT_FOOT_SKELETON_H */
