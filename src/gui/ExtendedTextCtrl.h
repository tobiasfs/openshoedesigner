///////////////////////////////////////////////////////////////////////////////
// Name               : ExtendedTextCtrl.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 27.02.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef GUI_EXTENDEDTEXTCTRL_H
#define GUI_EXTENDEDTEXTCTRL_H

/*!\class ExtendedTextCtrl
 * \brief TODO: Not yet sure if needed
 *
 * ...
 */

#include <wx/textctrl.h>

class Project;

class ExtendedTextCtrl:public wxTextCtrl {
public:
	ExtendedTextCtrl(wxWindow* parent, wxWindowID id,
			const wxString& value = "", const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize, long style = 0,
			const wxValidator& validator = wxDefaultValidator,
			const wxString& name = wxTextCtrlNameStr);
	virtual ~ExtendedTextCtrl() = default;

	void SetProject(Project * project = nullptr);

protected:
	void OnContextMenu(wxContextMenuEvent& event);

private:
	static bool IsKnowID(int id);

	Project* project = nullptr;

DECLARE_EVENT_TABLE()
};

#endif /* GUI_EXTENDEDTEXTCTRL_H */
