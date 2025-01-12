///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasInsole.cpp
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
#include "CanvasInsole.h"

#include <wx/dcclient.h>

CanvasInsole::CanvasInsole(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		CanvasMeasurementGrid(parent, id, pos, size, style, name) {

	this->Connect(wxEVT_PAINT, wxPaintEventHandler(CanvasInsole::OnPaint));
	this->Connect(wxEVT_SIZE, wxSizeEventHandler(CanvasInsole::OnSize));
}

CanvasInsole::~CanvasInsole() {
	this->Disconnect(wxEVT_SIZE, wxSizeEventHandler(CanvasInsole::OnSize));
	this->Disconnect(wxEVT_PAINT, wxPaintEventHandler(CanvasInsole::OnPaint));
}

void CanvasInsole::OnSize(wxSizeEvent &event) {
	Refresh();
}

void CanvasInsole::PaintInsole(wxDC &dc, const AffineTransformMatrix &m,
		const std::shared_ptr<Insole> &insole) {

	wxPen *c = wxThePenList->FindOrCreatePen(wxColour(100, 200, 100), 3);
	wxPen *a = wxThePenList->FindOrCreatePen(wxColour(50, 50, 50), 1);

	wxPen *h = wxThePenList->FindOrCreatePen(wxColour(100, 100, 100), 1);
	wxPen *d = wxThePenList->FindOrCreatePen(wxColour(100, 100, 100), 1,
			wxPENSTYLE_DOT);

//	double off = 0.0;

	dc.SetPen(*c);
	for (size_t n = 0; n < insole->EdgeCount(); ++n) {
		if (insole->GetEdge(n).trianglecount >= 2)
			continue;
		const auto &v0 = insole->GetEdgeVertex(n, 0);
		const auto &v1 = insole->GetEdgeVertex(n, 1);
//		Geometry::Vertex j(off, 0, 0);
		Vector3 p0 = m.Transform(v0);
		Vector3 p1 = m.Transform(v1);
		dc.DrawLine(p0.x, p0.y, p1.x, p1.y);
//		off += 0.0001;
	}

	Vector3 A = m.Transform(insole->A);
	Vector3 B = m.Transform(insole->B);
	Vector3 C = m.Transform(insole->C);
	Vector3 D = m.Transform(insole->D);
	Vector3 E = m.Transform(insole->E);
	Vector3 F = m.Transform(insole->F);
	Vector3 G = m.Transform(insole->G);
	Vector3 H = m.Transform(insole->H);
	Vector3 J = m.Transform(insole->J);
	Vector3 K = m.Transform(insole->K);
	Vector3 L = m.Transform(insole->L);
	Vector3 N = m.Transform(insole->N);
	Vector3 Z = m.Transform(insole->Z);

	dc.SetPen(*a);
	dc.DrawCircle(A.x, A.y, 2);
	dc.DrawText("A", A.x + 5, A.y - 20);
	dc.DrawCircle(B.x, B.y, 2);
	dc.DrawText("B", B.x + 5, B.y - 20);
	dc.DrawCircle(C.x, C.y, 2);
	dc.DrawText("C", C.x + 5, C.y - 20);
	dc.DrawCircle(D.x, D.y, 2);
	dc.DrawText("D", D.x + 5, D.y - 20);
	dc.DrawCircle(E.x, E.y, 2);
	dc.DrawText("E", E.x + 5, E.y - 20);
	dc.DrawCircle(F.x, F.y, 2);
	dc.DrawText("F", F.x + 5, F.y - 20);
	dc.DrawCircle(G.x, G.y, 2);
	dc.DrawText("G", G.x + 5, G.y - 20);
	dc.DrawCircle(H.x, H.y, 2);
	dc.DrawText("H", H.x + 5, H.y - 20);
	dc.DrawCircle(J.x, J.y, 2);
	dc.DrawText("J", J.x + 5, J.y - 20);
	dc.DrawCircle(K.x, K.y, 2);
	dc.DrawText("K", K.x + 5, K.y - 20);
	dc.DrawCircle(L.x, L.y, 2);
	dc.DrawText("L", L.x + 5, L.y - 20);
	dc.DrawCircle(N.x, N.y, 2);
	dc.DrawText("N", N.x + 5, N.y - 20);
	dc.DrawCircle(Z.x, Z.y, 2);
	dc.DrawText("Z", Z.x + 5, Z.y - 20);

	dc.SetPen(*h);
	dc.DrawLine(A.x, A.y, B.x, B.y);
	dc.DrawLine(E.x, E.y, F.x, F.y);
	dc.DrawLine(E.x, E.y, G.x, G.y);
	dc.DrawLine(G.x, G.y, H.x, H.y);
	dc.DrawLine(H.x, H.y, F.x, F.y);
	dc.DrawLine(K.x, K.y, L.x, L.y);

	dc.SetPen(*d);
	dc.DrawLine(J.x, J.y, N.x, N.y);

	dc.SetPen(*wxBLACK_PEN);
}

void CanvasInsole::OnPaint(wxPaintEvent &event) {
	CanvasMeasurementGrid::OnPaint(event);

	wxPaintDC dc(this);
#ifdef DEBUG
	dc.DrawText(_T("CanvasInsole"), 10, 30);
#endif
	if (insoleL) {
		const double d = (insoleL && insoleR) ? dL : 0.0;
		AffineTransformMatrix m = s
				* AffineTransformMatrix::Translation(0, d, 0);
		PaintInsole(dc, m, insoleL);
	}
	if (insoleR) {
		const double d = (insoleL && insoleR) ? dR : 0.0;
		AffineTransformMatrix m = s * AffineTransformMatrix::Scaling(1, -1, 1)
				* AffineTransformMatrix::Translation(0, d, 0);
		PaintInsole(dc, m, insoleR);
	}
}

