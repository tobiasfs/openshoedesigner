///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasFlattening.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 11.01.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "CanvasFlattening.h"

#include <wx/dcclient.h>
CanvasFlattening::CanvasFlattening(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		CanvasMeasurementGrid(parent, id, pos, size, style, name) {

	Connect(wxEVT_PAINT, wxPaintEventHandler(CanvasFlattening::OnPaint));

}

CanvasFlattening::~CanvasFlattening() {
	Disconnect(wxEVT_PAINT, wxPaintEventHandler(CanvasFlattening::OnPaint));

}

void CanvasFlattening::OnPaint(wxPaintEvent &event) {
	CanvasMeasurementGrid::OnPaint(event);
	wxPaintDC dc(this);
#ifdef DEBUG
	dc.DrawText(_T("CanvasFlattening"), 10, 30);
#endif
}
