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

void PaintLine(wxPaintDC &dc, const AffineTransformMatrix &sp,
		const Geometry::Vertex &v0, const Geometry::Vertex &v1, double shift) {

	wxPen *penBezier = wxThePenList->FindOrCreatePen(wxColour(0, 0, 0), 2);
	wxPen *penBezierSides = wxThePenList->FindOrCreatePen(wxColour(0, 0, 0), 1,
			wxPENSTYLE_DOT);

	if (fabs(v0.u + shift - M_PI) < FLT_EPSILON
			&& fabs(v1.u + shift - M_PI) < FLT_EPSILON) {
		Vector3 p0 = sp.Transform(v0.u + shift - 4.0 * M_PI, v0.v);
		Vector3 p1 = sp.Transform(v1.u + shift - 4.0 * M_PI, v1.v);
		Vector3 p2 = sp.Transform(v0.u + shift - 2.0 * M_PI, v0.v);
		Vector3 p3 = sp.Transform(v1.u + shift - 2.0 * M_PI, v1.v);
		Vector3 p4 = sp.Transform(v0.u + shift + 0.0 * M_PI, v0.v);
		Vector3 p5 = sp.Transform(v1.u + shift + 0.0 * M_PI, v1.v);
		Vector3 p6 = sp.Transform(v0.u + shift + 2.0 * M_PI, v0.v);
		Vector3 p7 = sp.Transform(v1.u + shift + 2.0 * M_PI, v1.v);
		dc.SetPen(*penBezier);
		dc.DrawLine(p2.x, p2.y, p3.x, p3.y);
		dc.DrawLine(p4.x, p4.y, p5.x, p5.y);
		dc.SetPen(*penBezierSides);
		dc.DrawLine(p0.x, p0.y, p1.x, p1.y);
		dc.DrawLine(p6.x, p6.y, p7.x, p7.y);
	} else if (fabs(v0.u + shift + M_PI) < FLT_EPSILON
			&& fabs(v1.u + shift + M_PI) < FLT_EPSILON) {
		Vector3 p0 = sp.Transform(v0.u + shift - 2.0 * M_PI, v0.v);
		Vector3 p1 = sp.Transform(v1.u + shift - 2.0 * M_PI, v1.v);
		Vector3 p2 = sp.Transform(v0.u + shift - 0.0 * M_PI, v0.v);
		Vector3 p3 = sp.Transform(v1.u + shift - 0.0 * M_PI, v1.v);
		Vector3 p4 = sp.Transform(v0.u + shift + 2.0 * M_PI, v0.v);
		Vector3 p5 = sp.Transform(v1.u + shift + 2.0 * M_PI, v1.v);
		Vector3 p6 = sp.Transform(v0.u + shift + 4.0 * M_PI, v0.v);
		Vector3 p7 = sp.Transform(v1.u + shift + 4.0 * M_PI, v1.v);
		dc.SetPen(*penBezier);
		dc.DrawLine(p2.x, p2.y, p3.x, p3.y);
		dc.DrawLine(p4.x, p4.y, p5.x, p5.y);
		dc.SetPen(*penBezierSides);
		dc.DrawLine(p0.x, p0.y, p1.x, p1.y);
		dc.DrawLine(p6.x, p6.y, p7.x, p7.y);
	} else {
		Vector3 p0 = sp.Transform(v0.u + shift - 2.0 * M_PI, v0.v);
		Vector3 p1 = sp.Transform(v1.u + shift - 2.0 * M_PI, v1.v);
		Vector3 p2 = sp.Transform(v0.u + shift + 0.0 * M_PI, v0.v);
		Vector3 p3 = sp.Transform(v1.u + shift + 0.0 * M_PI, v1.v);
		Vector3 p4 = sp.Transform(v0.u + shift + 2.0 * M_PI, v0.v);
		Vector3 p5 = sp.Transform(v1.u + shift + 2.0 * M_PI, v1.v);
		dc.SetPen(*penBezier);
		dc.DrawLine(p2.x, p2.y, p3.x, p3.y);
		dc.SetPen(*penBezierSides);
		dc.DrawLine(p0.x, p0.y, p1.x, p1.y);
		dc.DrawLine(p4.x, p4.y, p5.x, p5.y);
	}
}

void CanvasPattern::OnPaint(wxPaintEvent &event) {
	CanvasMeasurementGrid::OnPaint(event);
	wxPaintDC dc(this);
#ifdef DEBUG
	wxString text = wxString::Format(_T("CanvasPattern"));
	dc.DrawText(text, 10, 30);
#endif

	if (!design)
		return;

//	wxPen *penBezier = wxThePenList->FindOrCreatePen(wxColour(0, 0, 0), 2);
	wxPen *penEdge = wxThePenList->FindOrCreatePen(wxColour(60, 60, 60), 1,
			wxPENSTYLE_SHORT_DASH);
	wxPen *penVertex = wxThePenList->FindOrCreatePen(wxColour(60, 255, 60), 1);

	AffineTransformMatrix sp = camera
			* AffineTransformMatrix::Scaling(0.1 / M_PI, 0.1, 1);

	for (auto &e : design->edges) {

		double shift = 0.0;
		for (size_t idx = 1; idx < e.geo.CountVertices(); idx++) {
			const Geometry::Vertex &v0 = e.geo.GetVertex(idx - 1);
			const Geometry::Vertex &v1 = e.geo.GetVertex(idx);

			while ((v0.u + shift) > M_PI)
				shift -= 2.0 * M_PI;
			while ((v0.u + shift) < -M_PI)
				shift += 2.0 * M_PI;

			if ((v1.u + shift) > M_PI + FLT_EPSILON) {
				const double sep = (M_PI - v0.u - shift) / (v1.u - v0.u);
				Geometry::Vertex h = v0.Interp(v1, sep);
				PaintLine(dc, sp, v0, h, shift);
				PaintLine(dc, sp, h, v1, shift - 2.0 * M_PI);
			} else if ((v1.u + shift) < -M_PI - FLT_EPSILON) {
				const double sep = (-M_PI - v0.u - shift) / (v1.u - v0.u);
				Geometry::Vertex h = v0.Interp(v1, sep);
				PaintLine(dc, sp, v0, h, shift);
				PaintLine(dc, sp, h, v1, shift + 2.0 * M_PI);
			} else {
				PaintLine(dc, sp, v0, v1, shift);
			}
		}
	}

	for (auto &e : design->edges) {
		std::vector<wxPoint> pl;
		pl.clear();
		for (size_t idx : e.vidx) {
			const Design::PatchVertex &v = design->vertices[idx];
			Vector3 p = sp.Transform(v.u, v.v);
			pl.emplace_back(p.x, p.y);
		}
		dc.SetPen(*penEdge);
		dc.DrawLines(pl.size(), pl.data());
	}

	dc.SetPen(*penVertex);
	for (const Design::PatchVertex &v : design->vertices) {
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

