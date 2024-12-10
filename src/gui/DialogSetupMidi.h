///////////////////////////////////////////////////////////////////////////////
// Name               : DialogSetupMidi.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 13.06.2020
// Copyright          : (C) 2020 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef DIALOGSETUPMIDI_H
#define DIALOGSETUPMIDI_H

/*!\class DialogSetupMidi
 * \brief ...
 *
 * ...
 */

#include "gui.h"
#include "../system/MidiPort.h"

class DialogSetupMidi:public GUIDialogMidiSetup {
public:
	DialogSetupMidi(wxWindow* parent, MidiPort * midi, wxWindowID id = wxID_ANY,
			const wxString& title = _("Midi Controller"), const wxPoint& pos =
					wxDefaultPosition, const wxSize& size = wxSize(321, 175),
			long style = wxDEFAULT_DIALOG_STYLE);

	virtual ~DialogSetupMidi() = default;
	void UpdateDevices();
	void OnConnectDisconnect(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);

	MidiPort * midi;

};

#endif /* DIALOGSETUPMIDI_H */
