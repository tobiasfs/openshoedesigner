///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasSupport.cpp
// Purpose            : Panel for setting the walkcycle supports
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 27.10.2015
// Copyright          : (C) 2015 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "CanvasSupport.h"

#include <wx/dcclient.h>
#include <wx/log.h>

CanvasSupport::CanvasSupport(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		CanvasMeasurementGrid(parent, id, pos, size, style, name) {

	// Connect Events
//	this->Connect(wxEVT_LEFT_DOWN,
//			wxMouseEventHandler(CanvasSupport::OnLeftDown));
//	this->Connect(wxEVT_MOTION, wxMouseEventHandler(CanvasSupport::OnMotion));
//	this->Connect(wxEVT_SIZE, wxSizeEventHandler(CanvasSupport::OnSize));
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(CanvasSupport::OnPaint));
}

CanvasSupport::~CanvasSupport() {
	// Disconnect Events
//	this->Disconnect(wxEVT_LEFT_DOWN,
//			wxMouseEventHandler(CanvasSupport::OnLeftDown));
//	this->Disconnect(wxEVT_MOTION,
//			wxMouseEventHandler(CanvasSupport::OnMotion));
	this->Disconnect(wxEVT_PAINT, wxPaintEventHandler(CanvasSupport::OnPaint));
//	this->Disconnect(wxEVT_SIZE, wxSizeEventHandler(CanvasSupport::OnSize));
}

//void CanvasSupport::OnSize(wxSizeEvent &event) {
//	this->Refresh();
//}

void CanvasSupport::OnPaint(wxPaintEvent &event) {
	CanvasMeasurementGrid::OnPaint(event);
	wxPaintDC dc(this);
#ifdef DEBUG
	wxString text = wxString::Format(_T("CanvasSupport"));
	dc.DrawText(text, 10, 30);
#endif

}

