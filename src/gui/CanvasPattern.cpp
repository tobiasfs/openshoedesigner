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

#include <cfloat>
#include <wx/dcclient.h>

CanvasPattern::CanvasPattern(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		CanvasMeasurementGrid(parent, id, pos, size, style, name) {

	// Connect Events
//	Connect(wxEVT_SIZE, wxSizeEventHandler(CanvasPattern::OnSize));
//	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CanvasPattern::OnLeftDown));
//	Connect(wxEVT_MOTION, wxMouseEventHandler(CanvasPattern::OnMotion));
	Connect(wxEVT_PAINT, wxPaintEventHandler(CanvasPattern::OnPaint));
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

	if (!design)
		return;

	wxPen *penBezier = wxThePenList->FindOrCreatePen(wxColour(0, 0, 0), 2);
	wxPen *penEdge = wxThePenList->FindOrCreatePen(wxColour(60, 60, 60), 1,
			wxPENSTYLE_SHORT_DASH);
	wxPen *penVertex = wxThePenList->FindOrCreatePen(wxColour(60, 260, 60), 1);

	AffineTransformMatrix sp = camera
			* AffineTransformMatrix::Scaling(0.1 / M_PI, 0.1, 1);

	std::vector<wxPoint> pl;
	for (auto &e : design->edges) {
		pl.clear();
		for (size_t idx = 0; idx < e.geo.CountVertices(); idx++) {
			const Geometry::Vertex &v = e.geo.GetVertex(idx);
			Vector3 p = sp.Transform(v.u, v.v);
			pl.emplace_back(p.x, p.y);
		}
		dc.SetPen(*penBezier);
		dc.DrawLines(pl.size(), pl.data());

		pl.clear();
		for (size_t idx : e.vidx) {
			const Design::Vertex &v = design->vertices[idx];
			Vector3 p = sp.Transform(v.u, v.v);
			pl.emplace_back(p.x, p.y);
		}
		dc.SetPen(*penEdge);
		dc.DrawLines(pl.size(), pl.data());
	}
	dc.SetPen(*penVertex);
	for (const Design::Vertex &v : design->vertices) {
		Vector3 p = sp.Transform(v.u, v.v);
		dc.DrawCircle(p.x, p.y, 4);
		if (v.u < -M_PI + FLT_EPSILON) {
			Vector3 p = sp.Transform(v.u + 2.0 * M_PI, v.v);
			dc.DrawCircle(p.x, p.y, 4);
		}
		if (v.u > M_PI - FLT_EPSILON) {
			Vector3 p = sp.Transform(v.u - 2.0 * M_PI, v.v);
			dc.DrawCircle(p.x, p.y, 4);
		}
	}
	dc.SetPen(*wxBLACK);
}

//void CanvasPattern::OnMotion(wxMouseEvent &event) {
//}
//
//void CanvasPattern::OnLeftDown(wxMouseEvent &event) {
//}

