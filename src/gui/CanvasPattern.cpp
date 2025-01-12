///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasPattern.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 01.11.2017
// Copyright          : (C) 2017 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "CanvasPattern.h"

#include <wx/dcclient.h>

CanvasPattern::CanvasPattern(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		CanvasMeasurementGrid(parent, id, pos, size, style, name) {

	// Connect Events
//	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CanvasPattern::OnLeftDown));
//	Connect(wxEVT_MOTION, wxMouseEventHandler(CanvasPattern::OnMotion));
	Connect(wxEVT_PAINT, wxPaintEventHandler(CanvasPattern::OnPaint));
//	Connect(wxEVT_SIZE, wxSizeEventHandler(CanvasPattern::OnSize));
}

CanvasPattern::~CanvasPattern() {
	// Disconnect Events
//	Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CanvasPattern::OnLeftDown));
//	Disconnect(wxEVT_MOTION, wxMouseEventHandler(CanvasPattern::OnMotion));
	Disconnect(wxEVT_PAINT, wxPaintEventHandler(CanvasPattern::OnPaint));
//	Disconnect(wxEVT_SIZE, wxSizeEventHandler(CanvasPattern::OnSize));
}

//void CanvasPattern::OnSize(wxSizeEvent &event) {
//	Refresh();
//}

void CanvasPattern::OnPaint(wxPaintEvent &event) {
	CanvasMeasurementGrid::OnPaint(event);
	wxPaintDC dc(this);
#ifdef DEBUG
	wxString text = wxString::Format(_T("CanvasPattern"));
	dc.DrawText(text, 10, 30);
#endif


}

//void CanvasPattern::OnMotion(wxMouseEvent &event) {
//}
//
//void CanvasPattern::OnLeftDown(wxMouseEvent &event) {
//}

