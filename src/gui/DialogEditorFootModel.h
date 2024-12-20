///////////////////////////////////////////////////////////////////////////////
// Name               : DialogEditorFootModel.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 14.06.2020
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

#ifndef GUI_DIALOGEDITORFOOTMODEL_H
#define GUI_DIALOGEDITORFOOTMODEL_H

/*!\class DialogEditorFootModel
 * \brief ...
 *
 * ...
 */

#include <wx/timer.h>
#include <memory>
#include "../system/MidiPort.h"
#include "gui.h"

class DialogEditorFootModel: public GUIFrameEditorFootModel {
public:
	DialogEditorFootModel(wxWindow *parent, wxWindowID id = wxID_ANY,
			const wxString &title = _("Foot model editor"), const wxPoint &pos =
					wxDefaultPosition, const wxSize &size = wxSize(715, 351),
			long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	virtual ~DialogEditorFootModel();

	void OnLoad(wxCommandEvent &event) override;
	void OnSave(wxCommandEvent &event) override;
	void OnClose(wxCommandEvent &event) override;

	void SetMidi(MidiPort *midi);

	wxTimer timer;

	void OnTimer(wxTimerEvent &event);

private:
	uint8_t mode = 0;
	void SetSlider(uint8_t idx, const std::string &name);
	void Update_(uint8_t idx, const std::string &name);
	static std::string MakeString(double value);

	std::shared_ptr<MidiDevice> device;
};

#endif /* GUI_DIALOGEDITORFOOTMODEL_H */
