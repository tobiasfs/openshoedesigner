///////////////////////////////////////////////////////////////////////////////
// Name               : PanelMeasurementGrid.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 25.12.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "CanvasMeasurementGrid.h"

#include "../3D/Vector3.h"
#include <wx/dcclient.h>

CanvasMeasurementGrid::CanvasMeasurementGrid(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		wxPanel(parent, id, pos, size, style, name) {

	unit = Unit("cm");

	view.SetIdentity();
//	view = AffineTransformMatrix::RotationAroundVector( { 0, 0, 1 }, M_PI_2);
//	view.TranslateGlobal(0, 0, 0);

	this->Connect(wxEVT_PAINT,
			wxPaintEventHandler(CanvasMeasurementGrid::OnPaint));
	this->Connect(wxEVT_SIZE,
			wxSizeEventHandler(CanvasMeasurementGrid::OnSize));
	this->Connect(wxEVT_MOTION,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent), nullptr,
			this);
	this->Connect(wxEVT_MOUSEWHEEL,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent), nullptr,
			this);
	this->Connect(wxEVT_RIGHT_DOWN,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent), nullptr,
			this);
	this->Connect(wxEVT_MIDDLE_DOWN,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent), nullptr,
			this);
	this->Connect(wxEVT_RIGHT_DCLICK,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent), nullptr,
			this);

}

CanvasMeasurementGrid::~CanvasMeasurementGrid() {
	this->Disconnect(wxEVT_PAINT,
			wxPaintEventHandler(CanvasMeasurementGrid::OnPaint));
	this->Disconnect(wxEVT_SIZE,
			wxSizeEventHandler(CanvasMeasurementGrid::OnSize));
	this->Disconnect(wxEVT_MOTION,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent));
	this->Disconnect(wxEVT_MOUSEWHEEL,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent));
	this->Disconnect(wxEVT_RIGHT_DOWN,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent));
	this->Disconnect(wxEVT_MIDDLE_DOWN,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent));
	this->Disconnect(wxEVT_RIGHT_DCLICK,
			wxMouseEventHandler(CanvasMeasurementGrid::OnMouseEvent));
}

void CanvasMeasurementGrid::OnSize(wxSizeEvent &event) {

	this->Refresh();
}

void CanvasMeasurementGrid::OnMouseEvent(wxMouseEvent &event) {
	if (event.ButtonDown(wxMOUSE_BTN_RIGHT)
			|| event.ButtonDown(wxMOUSE_BTN_MIDDLE)) {
		m_x = event.m_x;
		m_y = event.m_y;
	}
	if (event.ButtonDClick(wxMOUSE_BTN_RIGHT)) {
//		view = AffineTransformMatrix::RotationAroundVector( { 0, 0, 1 },
//		M_PI_2);
		view.SetIdentity();
		m_x = event.m_x;
		m_y = event.m_y;
		this->Refresh();
	}

	if (event.Dragging()) {
		if (event.MiddleIsDown()) {
			float movement = 107.0 / 0.0254;
			const float dx = (float) (event.m_x - m_x) / movement;
			const float dy = (float) (m_y - event.m_y) / movement;
			view.TranslateGlobal(dx, dy, 0);
			m_x = event.m_x;
			m_y = event.m_y;
			this->Refresh();
		}
		if (event.RightIsDown()) {
			float movement = 500.0;
			const float dx = 1.0
					+ (float) ((event.m_x - m_x) + (m_y - event.m_y))
							/ movement;
			view.ScaleGlobal(dx, dx, 1.00);
			m_x = event.m_x;
			m_y = event.m_y;
			this->Refresh();
		}

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

void CanvasMeasurementGrid::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);
#ifdef DEBUG
	dc.DrawText(_T("CanvasMeasurementGrid"), 10, 10);
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


	camera = projection * view;
	gridInv = camera.Inverse();

// 	Coordinate system
//
//	const double sc = 0.05;
//	Vector3 p0 = camera.Transform(0, 0, 0);
//	Vector3 p1 = camera.Transform(sc, 0, 0);
//	Vector3 p2 = camera.Transform(0.9 * sc, -0.1 * sc, 0);
//	Vector3 p3 = camera.Transform(0.9 * sc, 0.1 * sc, 0);
//	Vector3 p4 = camera.Transform(0, sc, 0);
//	Vector3 p5 = camera.Transform(-0.1 * sc, 0.9 * sc, 0);
//	Vector3 p6 = camera.Transform(0.1 * sc, 0.9 * sc, 0);
//
//	wxPen *c0 = wxThePenList->FindOrCreatePen(wxColour(255,100, 100), 3);
//	wxPen *c1 = wxThePenList->FindOrCreatePen(wxColour(100, 255, 100), 3);
//
//	dc.SetPen(*c0);
//	dc.DrawLine(p0.x, p0.y, p1.x, p1.y);
//	dc.DrawLine(p2.x, p2.y, p1.x, p1.y);
//	dc.DrawLine(p3.x, p3.y, p1.x, p1.y);
//	dc.SetPen(*c1);
//	dc.DrawLine(p0.x, p0.y, p4.x, p4.y);
//	dc.DrawLine(p5.x, p5.y, p4.x, p4.y);
//	dc.DrawLine(p6.x, p6.y, p4.x, p4.y);
//	dc.SetPen(*wxBLACK_PEN);

	PaintGrid(dc);
}

void CanvasMeasurementGrid::PaintGrid(wxDC &dc) {
	wxSize sd = dc.GetSize();

	wxPen *c0 = wxThePenList->FindOrCreatePen(wxColour(200, 200, 200), 1);
	wxPen *c1 = wxThePenList->FindOrCreatePen(wxColour(150, 150, 150), 1);
	wxPen *c2 = wxThePenList->FindOrCreatePen(wxColour(100, 100, 100), 1);

	Vector3 topleft = gridInv( { 0, 0, 0 });
	Vector3 bottomright = gridInv(Vector3(sd.x, sd.y, 0));

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

		Vector3 p0 = camera.Transform(xx, minY);
		Vector3 p1 = camera.Transform(xx, maxY);

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
		Vector3 p0 = camera.Transform(minX, yy);
		Vector3 p1 = camera.Transform(maxX, yy);
		dc.DrawLine(p0.x, p0.y, p1.x, p1.y);
		if (y % multiplier == 0)
			dc.SetPen(*c0);
	}
	dc.SetPen(*wxBLACK_PEN);
}

