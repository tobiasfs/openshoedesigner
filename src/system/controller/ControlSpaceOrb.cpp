///////////////////////////////////////////////////////////////////////////////
// Name               : ControlSpaceOrb.cpp
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

#include "ControlSpaceOrb.h"

ControlSpaceOrb::ControlSpaceOrb() {
	InitDevice();
}

void ControlSpaceOrb::InitDevice() {
	idx = 0;
}

bool ControlSpaceOrb::ProcessPacket() {
	const static unsigned char spaceorb_xor[] = "SpaceWare";
	//static unsigned char spaceorb_errors[7][256] = { "EEPROM storing 0 failed", "Receive queue overflow", "Transmit queue timeout","Bad packet", "Power brown-out", "EEPROM checksum error", "Hardware fault" };
	unsigned char c = 0;
	int i;
	if (idx < 2)
		return false;
	for (i = 0; i < idx; i++)
		c ^= data[i];
	if (c)
		return false;
	switch (data[0]) {
	case 'R': /* Reset packet */
		data[idx - 1] = 0;
		for (i = 1; i < idx && data[i] == ' '; i++) {
		}
		//printk(KERN_INFO "input%d: %s [%s] on serio%d\n",spaceorb->dev.number, spaceorb_name, spaceorb->data + i, spaceorb->serio->number);
		break;
	case 'D': /* Ball + button data */
		if (idx != 12)
			return false;
		for (i = 0; i < 9; i++)
			data[i + 2] ^= spaceorb_xor[i];
		axis[0] = (data[2] << 3) | (data[3] >> 4);
		axis[2] = ((data[3] & 0x0f) << 6) | (data[4] >> 1);
		axis[1] = ((data[4] & 0x01) << 9) | (data[5] << 2) | (data[4] >> 5);
		axis[3] = ((data[6] & 0x1f) << 5) | (data[7] >> 2);
		axis[4] = ((data[7] & 0x03) << 8) | (data[8] << 1) | (data[7] >> 6);
		axis[5] = ((data[9] & 0x3f) << 4) | (data[10] >> 3);
		for (i = 0; i < 6; i++)
			axis[i] -= ((axis[i] & 0x200) ? 1024 : 0);
		for (i = 0; i < 7; i++)
			button[i] = (data[1] >> i) & 1;
		hasChanged = true;
		break;
	case 'K': /* Button data */
		if (idx != 5)
			return false;
		for (i = 0; i < 7; i++)
			button[i] = (data[2] >> i) & 1;
		hasChanged = true;
		break;
	case 'E': /* Error packet */
		if (idx != 4)
			return false;
		//printk(KERN_ERR "joy-spaceorb: Device error. [ ");
		//for (i = 0; i < 7; i++) if (data[1] & (1 << i)) printk("%s ", spaceorb_errors[i]);
		//printk("]\n");
		break;
	}
	return true;
}

void ControlSpaceOrb::DataIn(uint8_t c) {
	//TODO Check spec, if xor is correct here.
	if (~c & 0x80) {
		if (idx)
			ProcessPacket();
		idx = 0;
	}
	if (idx < maxDataLength) {
		data[idx++] = c & 0x7f;
	}
}

