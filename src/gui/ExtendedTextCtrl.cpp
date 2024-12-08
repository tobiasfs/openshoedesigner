///////////////////////////////////////////////////////////////////////////////
// Name               : ExtendedTextCtrl.cpp
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

#include "ExtendedTextCtrl.h"

#include <wx/menu.h>

#include "gui.h"

BEGIN_EVENT_TABLE(ExtendedTextCtrl, wxTextCtrl) EVT_CONTEXT_MENU(ExtendedTextCtrl::OnContextMenu)
END_EVENT_TABLE()

ExtendedTextCtrl::ExtendedTextCtrl(wxWindow* parent, wxWindowID id,
		const wxString& value, const wxPoint& pos, const wxSize& size,
		long style, const wxValidator& validator, const wxString& name)
		: wxTextCtrl(parent, id, value, pos, size, style, validator, name)
{
}

void ExtendedTextCtrl::SetProject(Project* project)
{
	this->project = project;
}

void ExtendedTextCtrl::OnContextMenu(wxContextMenuEvent& event)
{
	if(project == nullptr || !IsKnowID(event.GetId())){
		event.Skip();
		return;
	}

	wxMenu* menu = new wxMenu;
	menu->Append(wxID_CUT);
	menu->Append(wxID_COPY);
	menu->Append(wxID_PASTE);
	menu->Append(wxID_CLEAR);
	menu->AppendSeparator();
	menu->Append(wxID_SELECTALL);

	// Add any custom items here

	PopupMenu(menu);
}

bool ExtendedTextCtrl::IsKnowID(int id)
{
	switch(id){
	case ID_MEASUREMENT_FOOTLENGTH:
	case ID_MEASUREMENT_BALLWIDTH:
	case ID_MEASUREMENT_BIGTOEGIRTH:
	case ID_MEASUREMENT_LITTLETOEGIRTH:
	case ID_MEASUREMENT_WAISTGIRTH:
	case ID_MEASUREMENT_HEELGIRTH:
	case ID_MEASUREMENT_HEELWIDTH:
	case ID_MEASUREMENT_ANGLEMIXING:
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
	case ID_MEASUREMENT_BELOWCRUTCHGIRTH:
	case ID_MEASUREMENT_BELOWCRUTCHLEVEL:
	case ID_MEASUREMENT_MIDDLEOFCALFGIRTH:
	case ID_MEASUREMENT_MIDDLEOFCALFLEVEL:
	case ID_MEASUREMENT_ABOVEKNEEGIRTH:
	case ID_MEASUREMENT_ABOVEKNEELEVEL:
	case ID_MEASUREMENT_OVERKNEECAPGIRTH:
	case ID_MEASUREMENT_OVERKNEECAPLEVEL:
	case ID_MEASUREMENT_BELOWKNEEGIRTH:
	case ID_MEASUREMENT_BELOWKNEELEVEL:
	case ID_MEASUREMENT_MIDDLEOFSHANKGIRTH:
	case ID_MEASUREMENT_MIDDLEOFSHANKLEVEL:
	case ID_MEASUREMENT_ABOVEANKLEGIRTH:
	case ID_MEASUREMENT_ABOVEANKLELEVEL:
	case ID_MEASUREMENT_OVERANKLEBONEGIRTH:
	case ID_MEASUREMENT_OVERANKLEBONELEVEL:
	case ID_HEELHEIGHT:
	case ID_BALLHEIGHT:
	case ID_HEELPITCH:
	case ID_TOESPRING:
	case ID_UPPERLEVEL:
	case ID_EXTRALENGTH:
	case ID_FOOTCOMPRESSION:
		return true;
	}
	return false;
}
