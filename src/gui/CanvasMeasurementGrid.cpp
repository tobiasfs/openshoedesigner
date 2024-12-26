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

#include <wx/dcclient.h>

CanvasMeasurementGrid::CanvasMeasurementGrid(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style) :
		wxPanel(parent, id, pos, size, style) {

	unit = Unit("cm");

	this->Connect(wxEVT_PAINT,
			wxPaintEventHandler(CanvasMeasurementGrid::OnPaint));
	this->Connect(wxEVT_SIZE,
			wxSizeEventHandler(CanvasMeasurementGrid::OnSize));

}

CanvasMeasurementGrid::~CanvasMeasurementGrid() {
	this->Disconnect(wxEVT_PAINT,
			wxPaintEventHandler(CanvasMeasurementGrid::OnPaint));
	this->Disconnect(wxEVT_SIZE,
			wxSizeEventHandler(CanvasMeasurementGrid::OnSize));
}

void CanvasMeasurementGrid::OnSize(wxSizeEvent &event) {
	this->Refresh();
}

void CanvasMeasurementGrid::OnPaint(wxPaintEvent &event) {
	wxPoint temp;
	wxPaintDC dc(this);
	dc.DrawText(_T("CanvasMeasurementGrid"), 10, 10);
	dc.SetMapMode(wxMM_METRIC);
	wxSize sz = dc.GetSize();
	dc.SetDeviceOrigin(sz.x / 2, sz.y / 2);
	dc.SetUserScale(0.65,0.65);
	PaintGrid(dc);
}

void CanvasMeasurementGrid::PaintGrid(wxDC &dc) {

	dc.SetPen(*wxGREY_PEN);

	int x0 = (int) ceil(unit.FromSI(minX));
	int x1 = (int) floor(unit.FromSI(maxX));
	int y0 = (int) ceil(unit.FromSI(minY));
	int y1 = (int) floor(unit.FromSI(maxY));
	for (int x = x0; x <= x1; ++x) {
		double xx = unit.ToSI((double) x);
		dc.DrawLine(xx * 1000, minY * 1000, xx * 1000, maxY * 1000);
	}
	for (int y = y0; y <= y1; ++y) {
		double yy = unit.ToSI((double) y);
		dc.DrawLine(minX * 1000, yy * 1000, maxX * 1000, yy * 1000);
	}
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawLine(0 * 1000, minY * 1000, 0 * 1000, maxY * 1000);
	dc.DrawLine(minX * 1000, 0 * 1000, maxX * 1000, 0 * 1000);
}

