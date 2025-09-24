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
#include <cfloat>
#include <wx/dcclient.h>

CanvasGraph::Axis::Axis(const Unit &unit_) :
		unit(unit_) {
}

void CanvasGraph::Axis::Set(double vmin, double vmax, size_t tickCount) {
	// Sanitize range
	if (tickCount < 3)
		tickCount = 3;
	{
		const double h = std::fmin(vmin, vmax);
		vmax = std::fmax(vmin, vmax);
		vmin = h;
	}
	if (vmin > vmax - DBL_EPSILON) {
		vmin = (vmin + vmax - 1.0) / 2.0;
		vmax = vmin + 1.0;
	}

	dv = std::pow(10.0,
			std::floor(std::log10((vmax - vmin) / (double) tickCount)));
	n0 = std::floor(vmin / dv);
	n1 = std::ceil(vmax / dv);
	int divcount = 0;
	while (divcount < 10 && (n1 - n0) > tickCount) {
		dv *= (divcount % 4 == 1) ? (5.0 / 4.0) : 2.0;
		divcount++;
		n0 = std::floor(vmin / dv);
		n1 = std::ceil(vmax / dv);
	}
	switch (divcount % 4) {
	default:
		divs = 10;
		break;
	case 1:
		divs = 5;
		break;
	case 2:
		divs = 4;
		break;
	case 3:
		divs = 2;
		break;
	}
}

void CanvasGraph::Axis::Paint(wxDC &dc, const AffineTransformMatrix &s) const {
	if (n0 >= n1)
		return;
	const bool isXAxis = fabs(s[1] * s[4]) < fabs(s[0] * s[5]);

	wxPen *c0 = wxThePenList->FindOrCreatePen(wxColour(200, 200, 200), 1);
	wxPen *c1 = wxThePenList->FindOrCreatePen(wxColour(150, 150, 150), 1);
	wxPen *c2 = wxThePenList->FindOrCreatePen(wxColour(100, 100, 100), 1);

	wxSize sd = dc.GetSize();

	int m0 = n0;
	int m1 = n1;

	Vector3 p0 = s.Transform((double) m0 * dv, 0);
	Vector3 p1 = s.Transform((double) m1 * dv, 0);

	if (isXAxis) {
		// X-direction
		const double Nunit = (p1.x - p0.x) / (double) (n1 - n0);
		if (p0.x < 0)
			m0 = m0 + ceil(-p0.x / Nunit);
		if (p1.x > sd.x)
			m1 = m1 - ceil((p1.x - (double) sd.x) / Nunit);
	} else {
		// Y-direction
		const double Nunit = (p0.y - p1.y) / (double) (n1 - n0);
		if (p1.y < 0)
			m1 = m1 - ceil(-p1.y / Nunit);
		if (p0.y > sd.y)
			m0 = m0 + ceil((p0.y - (double) sd.y) / Nunit);
	}
	dc.SetPen(*c2);
	p0 = s.Transform((double) m0 * dv, 0);
	p1 = s.Transform((double) m1 * dv, 0);
	dc.DrawLine(p0.x, p0.y, p1.x, p1.y);

//	Vector3 p2 = s.Transform(((double) n1 + 0.5) * dv, dv * 0.2);
//	Vector3 p3 = s.Transform(((double) n1 + 0.5) * dv, -dv * 0.2);
//	dc.DrawLine(p1.x, p1.y, p2.x, p2.y);
//	dc.DrawLine(p1.x, p1.y, p3.x, p3.y);

	for (int n = m0; n <= m1; n++) {
		Vector3 t0 = s.Transform((double) n * dv, 0);
		Vector3 t1 = t0;
		if (isXAxis)
			t1.y += 5;
		else
			t1.x -= 5;
		Vector3 dir = (t1 - t0).Normal();
		wxString num = unit.TextFromSI((double) n * dv);
		const wxSize txts = dc.GetTextExtent(num);
		double dx =
				fabs(dir.x) > FLT_EPSILON ?
						((double) txts.x / fabs(dir.x)) : DBL_MAX;
		double dy =
				fabs(dir.y) > FLT_EPSILON ?
						((double) txts.y / fabs(dir.y)) : DBL_MAX;
		Vector3 offs = dir * fmin(dx, dy) * 0.75;

		if (n == 0) {
			dc.SetPen(*c2);
		} else if (abs(n % divs) == 0) {
			dc.SetPen(*c1);
		} else {
			dc.SetPen(*c0);
		}
		if (isXAxis) {
			dc.DrawLine(t0.x, 0, t0.x, sd.y);
		} else {
			dc.DrawLine(0, t0.y, sd.x, t0.y);
		}

		dc.SetPen(*wxBLACK_PEN);
		dc.DrawLine(t0.x, t0.y, t1.x, t1.y);

		dc.DrawText(num, t0.x - txts.x / 2 + offs.x,
				t0.y - txts.y / 2 + offs.y);

	}
	dc.SetPen(wxNullPen);
}

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
			const float dx = 1.0 + (float) (event.m_x - m_x) / movement;
			const float dy = 1.0 + (float) (m_y - event.m_y) / movement;
			view.ScaleGlobal(dx, dy, 1.00);
			m_x = event.m_x;
			m_y = event.m_y;
			this->Refresh();
		}

	} else {
		const int x = event.GetWheelRotation();
		if (x != 0) {
			float sc = exp(-((float) x) / 1000.0);
			if (event.ShiftDown())
				view.ScaleGlobal(1.0, sc, 1.0);
			else
				view.ScaleGlobal(sc, sc, 1.0);
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

	const double dpi_x = 109.2;
	const double dpi_y = 109.2;

	wxSize sd = dc.GetSize();
	projection.SetIdentity();
	projection[0] = dpi_x / 0.0254;
	projection[5] = -dpi_y / 0.0254;
	projection[12] = (double) sd.x / 2.0;
	projection[13] = (double) sd.y / 2.0;

	s = projection * view;
	sRev = s.Inverse();

	if (!values || values->empty() || values->Size(1) < 1)
		return;

	const size_t rows = values->Size(0);
	const size_t plots = values->Size(1) - 1;

	auto mMin = values->Min(0);
	auto mMax = values->Max(0);

	double xMin = mMin(0, 0);
	double xMax = mMax(0, 0);
	double yMin = -1.0;
	double yMax = 1.0;
	if (plots > 0) {
		yMin = mMin(0, 1);
		yMax = mMax(0, 1);
	}
	for (size_t n = 2; n <= plots; n++) {
		yMin = std::fmin(yMin, mMin(0, n));
		yMax = std::fmax(yMax, mMax(0, n));
	}
	Vector3 p0 = s.Transform(xMin, yMin);
	Vector3 p1 = s.Transform(xMax, yMax);

	size_t rx = (p1.x - p0.x) / 75;
	size_t ry = (p0.y - p1.y) / 75;

	axisX.Set(xMin, xMax, rx);
	axisY.Set(yMin, yMax, ry);

	axisX.Paint(dc, s);

	AffineTransformMatrix sY = s;
	for (uint8_t n = 0; n < 4; n++)
		std::swap(sY[n], sY[n + 4]); // @suppress("Invalid arguments")

	axisY.Paint(dc, sY);

	std::vector<wxPoint> points(rows);
	for (size_t m = 0; m < plots; m++) {
		// m to HSV-color

		double f = (double) m * 2.0;
		if ((m / 3) % 2 == 1)
			f += 1.0;
		if ((m / 3) % 4 > 1)
			f += 0.5;

		int h = (int) floor(f);
		f -= (double) h;
		h = h % 6;
		double t = 255.0 * f;
		double q = 255.0 - t;
		int r;
		int g;
		int b;
		switch (h) {
		default:
			r = 255;
			g = (int) t;
			b = 0;
			break;
		case 1:
			r = (int) q;
			g = 255;
			b = 0;
			break;
		case 2:
			r = 0;
			g = 255;
			b = (int) t;
			break;
		case 3:
			r = 0;
			g = (int) q;
			b = 255;
			break;
		case 4:
			r = (int) t;
			g = 0;
			b = 255;
			break;
		case 5:
			r = 255;
			g = 0;
			b = (int) q;
			break;
		}
		wxPen *c0 = wxThePenList->FindOrCreatePen(wxColour(r, g, b), 2);
		dc.SetPen(*c0);
		for (size_t n = 0; n < rows; n++) {
			double x = values->operator()(n, 0);
			double y = values->operator()(n, m + 1);
			Vector3 p = s.Transform(x, y);
			points[n].x = p.x;
			points[n].y = p.y;
		}
		dc.DrawLines(points.size(), points.data());
	}
	dc.SetPen(wxNullPen);
}

