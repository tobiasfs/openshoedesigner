///////////////////////////////////////////////////////////////////////////////
// Name               : ObjectLoad.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
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
#ifndef OPERATION_OBJECTLOAD_H
#define OPERATION_OBJECTLOAD_H

/** \class ObjectLoad
 * 	\code #include "ObjectLoad.h"\endcode
 * 	\ingroup ObjectOperations
 *  \brief Load the geometry from a file
 *
 * Used the importer derived from FileGeometry to import a geometry from disk.
 *
 * If the filename is changed or the modification time of the file is changed
 * the file is reloaded.
 *
 * Several file formats are supported: DXF, GTS, OBJ, PLY, SRL and some obscure
 * file format for sliced last scans. The files are identified by the file
 * extension.
 */

#include "Operation.h"

#include "../../3D/Geometry.h"
#include "../object/ObjectGeometry.h"
#include "../ParameterString.h"

#include <filesystem>
#include <memory>

class ObjectLoad: public Operation {
public:
	ObjectLoad();
	virtual ~ObjectLoad() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

	std::shared_ptr<ParameterString> filename;

	std::shared_ptr<ObjectGeometry> out;

private:
	std::filesystem::file_time_type lastModified;

};

#endif /* OPERATION_OBJECTLOAD_H */
