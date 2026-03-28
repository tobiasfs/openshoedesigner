///////////////////////////////////////////////////////////////////////////////
// Name               : Control3D.h
// Purpose            : Main class for 6DOF controller
// Thread Safe        : Yes
// Platform dependent : Yes
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.12.2009
// Copyright          : (C) 2009 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef CONTROL3D_H
#define CONTROL3D_H

#include "Control3DAbstract.h"
#include "ControlSpaceBall.h"
#include "ControlSpaceOrb.h"
#include "ControlSpaceMouse.h"

//#include <wx/config.h>

#include <string>

class Control3D {
public:
	Control3D();
	// Delete copy and copy assignment constructor to prevent copying
	// of this class, because this class has pointers with objects allocated
	// to them.
	Control3D(const Control3D&) = delete;
	Control3D& operator=(const Control3D&) = delete;
	virtual ~Control3D();

public:
	void Init(const std::string &type, const std::string &port, bool activate);

	bool SetType(uint8_t id);
	bool SetTypeString(const std::string &type);
	bool SetPort(const std::string &port);

	uint8_t GetType() const;
	std::string GetTypeString() const;
	std::string GetPort() const;

	bool Open(const std::string &connection);
	bool Open();
	void Close();

	bool IsOpen() const;
	bool IsActive() const;
	bool IsIdle() const;
	bool HasChanged() const;
	int GetButton(unsigned char i) const;
	int GetAxis(unsigned char i) const;

//	bool Load(wxConfig *config);
//	bool Save(wxConfig *config);

	bool Pump();

protected:
	Control3DAbstract *controller = nullptr;
};

#endif /* CONTROL3D_H */
