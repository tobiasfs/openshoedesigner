///////////////////////////////////////////////////////////////////////////////
// Name               : Control3DAbstract.cpp
// Purpose            : Abstract class for 6DOF controller classes.
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

#include "Control3DAbstract.h"

Control3DAbstract::Control3DAbstract() {
	for (uint_fast8_t i = 0; i < 6; i++)
		Axis[i] = 0;
	for (uint_fast8_t i = 0; i < maxButtons; i++)
		Button[i] = false;
	hasChanged = false;
}

void Control3DAbstract::InitDevice() {
}

void Control3DAbstract::SetPort(std::string connection) {
	this->connection = connection;
}

std::string Control3DAbstract::GetPort() const {
	return connection;
}

bool Control3DAbstract::Open() {
	return port.Open(connection, 9600);
}

void Control3DAbstract::Close() {
	port.Close();
}

int Control3DAbstract::GetButton(uint8_t idx) const {
	if (idx > maxButtons)
		return false;
	return Button[idx];
}

int Control3DAbstract::GetAxis(uint8_t idx) const {
	if (idx > 5)
		return 0;
	return Axis[idx];
}

bool Control3DAbstract::Pump() {
	hasChanged = false;
	if (!port.IsOpen())
		return false;
	char temp[128];
	uint_fast8_t i = port.ReadData(temp, 128);
	while (i != 0) {
		for (uint_fast8_t j = 0; j < i; j++)
			DataIn((uint8_t) temp[j]);
		i = port.ReadData(temp, 128);
	}
	return true;
}
