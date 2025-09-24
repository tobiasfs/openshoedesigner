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

#include <wx/config.h>

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
	bool Open(std::string connection);
	bool Open();
	void Close();
	bool IsOpen() const;

	bool SetType(char id);
	char GetType();

	bool HasChanged();
	bool IsIdle();
	int GetButton(unsigned char i);
	int GetAxis(unsigned char i);
	std::string GetPort();
	bool SetPort(std::string port);
	bool Load(wxConfig *config);
	bool Save(wxConfig *config);

	bool Pump();

protected:
	Control3DAbstract *controller;
};

#endif /* CONTROL3D_H */
