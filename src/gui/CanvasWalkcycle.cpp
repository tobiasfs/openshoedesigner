///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasWalkcycle.cpp
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
#include "CanvasWalkcycle.h"

#include "../3D/Vector3.h"
#include <wx/dcclient.h>

CanvasWalkcycle::CanvasWalkcycle(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		wxPanel(parent, id, pos, size, style, name) {

//	this->SetBackgroundColour(wxColour(200, 200, 200));

	speed = std::make_shared<ParameterFormula>("Speed", "0.83 m/s",
			"Speed of the walkcycle.");

	unit.Set(Unit::Base::m, 1);
	view = AffineTransformMatrix::RotationAroundVector( { 0, 0, 1 }, M_PI_2);
	view.TranslateGlobal(0.1, 0, 0);

// Connect Events
	this->Connect(wxEVT_LEFT_DOWN,
			wxMouseEventHandler(CanvasWalkcycle::OnLeftDown));
	this->Connect(wxEVT_MOTION, wxMouseEventHandler(CanvasWalkcycle::OnMotion));
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(CanvasWalkcycle::OnPaint));
	this->Connect(wxEVT_SIZE, wxSizeEventHandler(CanvasWalkcycle::OnSize));

	timer.SetOwner(this);
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(CanvasWalkcycle::OnTimer),
			nullptr, this);
	timer.Start(dt * 1000.0);
}

CanvasWalkcycle::~CanvasWalkcycle() {
	// Disconnect Events
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(CanvasWalkcycle::OnTimer),
			nullptr, this);
	this->Disconnect(wxEVT_LEFT_DOWN,
			wxMouseEventHandler(CanvasWalkcycle::OnLeftDown));
	this->Disconnect(wxEVT_MOTION,
			wxMouseEventHandler(CanvasWalkcycle::OnMotion));
	this->Disconnect(wxEVT_PAINT,
			wxPaintEventHandler(CanvasWalkcycle::OnPaint));
	this->Disconnect(wxEVT_SIZE, wxSizeEventHandler(CanvasWalkcycle::OnSize));
}

void CanvasWalkcycle::OnSize(wxSizeEvent &event) {
	this->Refresh();
}

void CanvasWalkcycle::OnPaint(wxPaintEvent &event) {
	if (!cyclelock.try_lock())
		return;
	std::lock_guard<std::mutex> lock(cyclelock, std::adopt_lock);

	wxPaintDC dc(this);
#ifdef DEBUG
	dc.DrawText(_T("CanvasWalkcycle"), 10, 10);
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
	sRev = s.Inverse();

	g = s;
	g[0] = -g[1];
	g[1] = 0.0;
	g[5] = g[4];
	g[4] = 0.0;
	gRev = g.Inverse();

	PaintGrid(dc);
}

void CanvasWalkcycle::PaintGrid(wxDC &dc) {
	wxSize sd = dc.GetSize();

	wxPen *c0 = wxThePenList->FindOrCreatePen(wxColour(200, 200, 200), 1);
	wxPen *c1 = wxThePenList->FindOrCreatePen(wxColour(150, 150, 150), 1);
	wxPen *c2 = wxThePenList->FindOrCreatePen(wxColour(100, 100, 100), 1);

	Vector3 topleft = gRev( { 0, 0, 0 });
	Vector3 bottomright = gRev(Vector3(sd.x, sd.y, 0));

	double minX = topleft.x;
	double maxX = bottomright.x;
	double minY = bottomright.y;
	double maxY = topleft.y;

	int x0 = (int) ceil(unit.FromSI(minX));
	int x1 = (int) floor(unit.FromSI(maxX));
	int y0 = (int) ceil(unit.FromSI(minY));
	int y1 = (int) floor(unit.FromSI(maxY));

	dc.SetPen(*c0);
	for (int x = x0; x <= x1; x++) {
		double xx = unit.ToSI((double) x);
		if (x % multiplier == 0) {
			if (x == 0)
				dc.SetPen(*c2);
			else
				dc.SetPen(*c1);
		}

		Vector3 p0 = g.Transform(xx, minY);
		Vector3 p1 = g.Transform(xx, maxY);

		dc.DrawLine(p0.x, p0.y, p1.x, p1.y);
		if (x % multiplier == 0)
			dc.SetPen(*c0);

	}
	dc.SetPen(*c0);
	for (int y = y0; y <= y1; y++) {
		double yy = unit.ToSI((double) y);
		if (y % multiplier == 0) {
			if (y == 0)
				dc.SetPen(*c2);
			else
				dc.SetPen(*c1);
		}
		Vector3 p0 = g.Transform(minX, yy);
		Vector3 p1 = g.Transform(maxX, yy);
		dc.DrawLine(p0.x, p0.y, p1.x, p1.y);
		if (y % multiplier == 0)
			dc.SetPen(*c0);
	}
	dc.SetPen(*wxBLACK_PEN);

	{
		Vector3 p0 = s.Transform(0, 0);
		Vector3 p1 = s.Transform(cos(cycle * 2 * M_PI), sin(cycle * 2 * M_PI));
		dc.DrawLine(p0.x, p0.y, p1.x, p1.y);
	}

}

void CanvasWalkcycle::OnMotion(wxMouseEvent &event) {

}

void CanvasWalkcycle::OnLeftDown(wxMouseEvent &event) {

}

void CanvasWalkcycle::OnTimer(wxTimerEvent &event) {
	t += dt;
	cycle += dt * speed->ToDouble();
	this->Refresh();
}
