///////////////////////////////////////////////////////////////////////////////
// Name               : MidiSignalMap.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 08.12.2023
// Copyright          : (C) 2023 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef SYSTEM_MIDISIGNALMAP_H
#define SYSTEM_MIDISIGNALMAP_H

#include "MidiPort.h"

/**\class MidiSignalMap
 * \brief Maps a Midi Signal to a double value and back.
 *
 * Used for assigning buttons and sliders on midi input devices to double
 * value ranges.
 *
 * Can be used even if there is no Midi Devce available, but the signals passed
 * in and out of the MidiSignalMap are heavily quantised (to 128 steps).
 */
class MidiSignalMap {
public:
	MidiSignalMap(size_t channel_, double vmin_, double vmax_,
			double v0_ = 0.0);
	MidiSignalMap(std::shared_ptr<MidiDevice> md_, size_t channel_,
			double vmin_, double vmax_, double v0_ = 0.0);

	double operator()();
	void Set(double v);
	void Reset();

	std::shared_ptr<MidiDevice> md;
private:
	size_t channel;
	double vmin;
	double vmax;
	double v0;
	bool initialized;

};

#endif /* SYSTEM_MIDISIGNALMAP_H */
