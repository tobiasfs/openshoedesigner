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

	wxPen *c = wxThePenList->FindOrCreatePen(wxColour(240, 240, 240), 1);
	wxPen *a = wxThePenList->FindOrCreatePen(wxColour(80, 80, 80), 1);

	if (flatteningL) {
		for (Geometry &p : flatteningL->patches) {
			const AffineTransformMatrix &m = p.matrix * camera;
			auto mapUV = [&m](const Geometry::Vertex &p) {
				return m.Transform(p.x, p.y);
			};

			for (size_t eidx = 0; eidx < p.CountEdges(); eidx++) {
				const Geometry::Edge &ed = p.GetEdge(eidx);
				if (ed.trianglecount <= 1) {
					const Geometry::Vertex &v0 = p.GetVertex(ed.va);
					const Geometry::Vertex &v1 = p.GetVertex(ed.vb);
					Vector3 p2 = mapUV(v0);
					Vector3 p3 = mapUV(v1);
					dc.SetPen(*a);
					dc.DrawLine(p2.x, p2.y, p3.x, p3.y);
				}
			}
		}
	}
	if (flatteningR) {
		for (Geometry &p : flatteningR->patches) {
			const AffineTransformMatrix &m = p.matrix * camera;
			auto mapUV = [&m](const Geometry::Vertex &p) {
				return m.Transform(p.x, p.y);
			};

			for (size_t eidx = 0; eidx < p.CountEdges(); eidx++) {
				const Geometry::Edge &ed = p.GetEdge(eidx);
				if (ed.trianglecount <= 1) {
					const Geometry::Vertex &v0 = p.GetVertex(ed.va);
					const Geometry::Vertex &v1 = p.GetVertex(ed.vb);
					Vector3 p2 = mapUV(v0);
					Vector3 p3 = mapUV(v1);
					dc.SetPen(*c);
					dc.DrawLine(p2.x, p2.y, p3.x, p3.y);
				}
			}
		}
	}
	dc.SetPen(*wxBLACK_PEN);
}
