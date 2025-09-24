///////////////////////////////////////////////////////////////////////////////
// Name               : Control3DAbstract.h
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

#ifndef CONTROL3DABSTRACT_H
#define CONTROL3DABSTRACT_H

#include "../SerialPort.h"
#include <string>
#include <stdint.h>

class Control3DAbstract {
public:
	Control3DAbstract();
	virtual ~Control3DAbstract() = default;

public:
	///\brief Opens the connection to the serial port.
	virtual bool Open();
	///\brief Closes the connection to the serial port.
	void Close();
	///\brief Sets a new port address
	void SetPort(std::string connection);
	///\brief Returns the port address
	std::string GetPort() const;
	///\brief Shows whether the connection is established.
	bool IsConnected() const {
		return port.IsOpen();
	}

	///\brief Returns the state of a given button.
	int GetButton(uint8_t idx) const;
	///\brief Returns the state of a given axis.
	int GetAxis(uint8_t idx) const;
	///\brief Indicates a change during the last pumping of data.
	bool HasChanged() const {
		return hasChanged;
	}

	///\brief Pumps data from the serial port to the packet analyzer.
	bool Pump();

	///\brief Returns an ID for the given 6-DOF driver.
	virtual uint8_t ReturnID() const = 0;

protected:
	virtual void InitDevice();
	virtual bool ProcessPacket() = 0;
	virtual void DataIn(uint8_t c) = 0;

public:
	static const uint8_t maxButtons = 10;

protected:
	SerialPort port; //< Serial port to 6-DOF controller
	std::string connection; //< Name of the connection /dev/ttyUSB, COM1, ...
	int Button[maxButtons]; //< Button states
	int Axis[6]; //< The six axes of the controller
	bool hasChanged; //< Indicates a change during the last pumping of data
};

#endif /* CONTROL3DABSTRACT_H */
