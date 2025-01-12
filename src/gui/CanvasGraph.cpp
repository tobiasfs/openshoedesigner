///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasGraph.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 30.10.2015
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
#include "CanvasGraph.h"

#include "../3D/Vector3.h"
#include <wx/dcclient.h>

CanvasGraph::CanvasGraph(wxWindow *parent, wxWindowID id, const wxPoint &pos,
		const wxSize &size, long style, const wxString &name) :
		wxPanel(parent, id, pos, size, style, name) {
//	this->SetBackgroundColour(wxColour(200, 200, 200));

// Connect Events
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(CanvasGraph::OnPaint));
	this->Connect(wxEVT_SIZE, wxSizeEventHandler(CanvasGraph::OnSize));
	this->Connect(wxEVT_MOTION, wxMouseEventHandler(CanvasGraph::OnMouseEvent),
			nullptr, this);
	this->Connect(wxEVT_MOUSEWHEEL,
			wxMouseEventHandler(CanvasGraph::OnMouseEvent), nullptr, this);
	this->Connect(wxEVT_RIGHT_DOWN,
			wxMouseEventHandler(CanvasGraph::OnMouseEvent), nullptr, this);
	this->Connect(wxEVT_MIDDLE_DOWN,
			wxMouseEventHandler(CanvasGraph::OnMouseEvent), nullptr, this);
	this->Connect(wxEVT_RIGHT_DCLICK,
			wxMouseEventHandler(CanvasGraph::OnMouseEvent), nullptr, this);
}

CanvasGraph::~CanvasGraph() {
	this->Disconnect(wxEVT_PAINT, wxPaintEventHandler(CanvasGraph::OnPaint));
	this->Disconnect(wxEVT_SIZE, wxSizeEventHandler(CanvasGraph::OnSize));
	this->Disconnect(wxEVT_MOTION,
			wxMouseEventHandler(CanvasGraph::OnMouseEvent));
	this->Disconnect(wxEVT_MOUSEWHEEL,
			wxMouseEventHandler(CanvasGraph::OnMouseEvent));
	this->Disconnect(wxEVT_RIGHT_DOWN,
			wxMouseEventHandler(CanvasGraph::OnMouseEvent));
	this->Disconnect(wxEVT_MIDDLE_DOWN,
			wxMouseEventHandler(CanvasGraph::OnMouseEvent));
	this->Disconnect(wxEVT_RIGHT_DCLICK,
			wxMouseEventHandler(CanvasGraph::OnMouseEvent));
}

void CanvasGraph::OnSize(wxSizeEvent &event) {
	this->Refresh();
}

void CanvasGraph::OnMouseEvent(wxMouseEvent &event) {
	if (event.ButtonDown(wxMOUSE_BTN_RIGHT)
			|| event.ButtonDown(wxMOUSE_BTN_MIDDLE)) {
		m_x = event.m_x;
		m_y = event.m_y;
	}
	if (event.ButtonDClick(wxMOUSE_BTN_RIGHT)) {
		view = AffineTransformMatrix::RotationAroundVector( { 0, 0, 1 },
		M_PI_2);
		m_x = event.m_x;
		m_y = event.m_y;
		this->Refresh();
	}

	if (event.Dragging() && event.MiddleIsDown()) {
		float movement = 107.0 / 0.0254;
		const float dx = (float) (event.m_x - m_x) / movement;
		const float dy = (float) (event.m_y - m_y) / movement;
		view.TranslateGlobal(dx, -dy, 0);
		m_x = event.m_x;
		m_y = event.m_y;
		this->Refresh();
	} else {
		const int x = event.GetWheelRotation();
		if (x != 0) {
			float sc = exp(-((float) x) / 1000.0);
			view.ScaleGlobal(sc, sc, sc);
			this->Refresh();
		}
	}
	if (event.Moving() || event.Dragging())
		event.Skip();
}

void CanvasGraph::OnPaint(wxPaintEvent &event) {

	wxPaintDC dc(this);
#ifdef DEBUG
	dc.DrawText(_T("CanvasGraph"), 10, 10);
#endif
	// Deactivate all shifts and scalings from wxDC.
	dc.SetMapMode(wxMM_TEXT); // Logical unit: pixel
	dc.SetUserScale(1.0, 1.0);
	dc.SetDeviceOrigin(0, 0);

	wxSize sd = dc.GetSize();

	const double dpi_x = 109.2;
	const double dpi_y = 109.2;

	projection.SetIdentity();
	projection[0] = dpi_x / 0.0254;
	projection[5] = -dpi_y / 0.0254;
	projection[12] = (double) sd.x / 2.0;
	projection[13] = (double) sd.y / 2.0;

	s = projection * view;

//	g = s;
//	g[0] = -g[1];
//	g[1] = 0.0;
//	g[5] = g[4];
//	g[4] = 0.0;
//	gRev = g.Inverse();
//
//	PaintGrid(dc);
//
//	dc.CrossHair(mx, my);

}
