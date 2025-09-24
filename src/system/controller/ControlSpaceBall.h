///////////////////////////////////////////////////////////////////////////////
// Name               : ControlSpaceBall.h
// Purpose            : Class to get data from a Spaceball controller
// Thread Safe        : Yes
// Platform dependent : Yes
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 21.07.2009
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

#ifndef CONTROLSPACEBALL_H
#define CONTROLSPACEBALL_H

#include "Control3DAbstract.h"

#define CONTROLSPACEBALL_ID	1

class ControlSpaceBall: public Control3DAbstract {
public:
	ControlSpaceBall();
	virtual ~ControlSpaceBall() = default;

	uint8_t ReturnID() const {
		return CONTROLSPACEBALL_ID;
	}

protected:
	void InitDevice();
	bool ProcessPacket();
	void DataIn(uint8_t c);

private:
	static const size_t maxDataLength = 256;
	unsigned char InBuffer[maxDataLength];
	int idx;
};

#endif // CONTROLSPACEBALL_H
