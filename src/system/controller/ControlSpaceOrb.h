///////////////////////////////////////////////////////////////////////////////
// Name               : ControlSpaceOrb.h
// Purpose            : Class to get data from a Spaceorb controller.
// Thread Safe        : Yes
// Platform dependent : Yes
// Compiler Options   :
// Author             : Vojtech Pavlik
// Modified for C++   : Tobias Schaefer
// Adapted            : 21.07.2009
// Copyright          : (C) 1999-2001 Vojtech Pavlik <vojtech@ucw.cz>
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

#ifndef CONTROLSPACEORB_H
#define CONTROLSPACEORB_H

#include "Control3DAbstract.h"

#define CONTROLSPACEORB_ID 2

class ControlSpaceOrb: public Control3DAbstract {
public:
	ControlSpaceOrb();
	virtual ~ControlSpaceOrb() = default;

	uint8_t ReturnID() const {
		return CONTROLSPACEORB_ID;
	}

protected:
	void InitDevice();
	bool ProcessPacket();
	void DataIn(uint8_t c);

private:
	static const size_t maxDataLength = 64;
	unsigned char Data[maxDataLength];
	int idx;
};

#endif // CONTROLSPACEORB_H
