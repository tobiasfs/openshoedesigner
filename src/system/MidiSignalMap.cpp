///////////////////////////////////////////////////////////////////////////////
// Name               : MidiSignalMap.cpp
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

#include "MidiSignalMap.h"

#include <cmath>

MidiSignalMap::MidiSignalMap(size_t channel_, double vmin_, double vmax_,
		double v0_) :
		channel(channel_), vmin(vmin_), vmax(vmax_), v0(v0_) {
	initialized = false;
}

MidiSignalMap::MidiSignalMap(std::shared_ptr<MidiDevice> md_, size_t channel_,
		double vmin_, double vmax_, double v0_) :
		md(md_), channel(channel_), vmin(vmin_), vmax(vmax_), v0(v0_) {
	initialized = false;
	Set(v0);
}
double MidiSignalMap::operator ()() {
	if (md.use_count() == 0)
		return v0;
	if (!initialized)
		Set(v0);
	return ((double) md->cc[channel] / 127.0) * (vmax - vmin) + vmin;
}

void MidiSignalMap::Set(double v) {
	if (md.use_count() == 0)
		return;
	md->cc[channel] = (unsigned char) std::round(
			127.0 * (v - vmin) / (vmax - vmin));
	initialized = true;
}

