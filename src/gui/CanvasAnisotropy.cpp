///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasAnisotropy.cpp
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
#include "CanvasAnisotropy.h"

#include <wx/dcclient.h>

CanvasAnisotropy::CanvasAnisotropy(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		wxPanel(parent, id, pos, size, style, name) {
	this->SetBackgroundColour(wxColour(200, 200, 200));

	// Connect Events
	this->Connect(wxEVT_LEFT_DOWN,
			wxMouseEventHandler(CanvasAnisotropy::OnLeftDown));
	this->Connect(wxEVT_MOTION,
			wxMouseEventHandler(CanvasAnisotropy::OnMotion));
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(CanvasAnisotropy::OnPaint));
	this->Connect(wxEVT_SIZE, wxSizeEventHandler(CanvasAnisotropy::OnSize));

}

CanvasAnisotropy::~CanvasAnisotropy() {
	// Disconnect Events
	this->Disconnect(wxEVT_LEFT_DOWN,
			wxMouseEventHandler(CanvasAnisotropy::OnLeftDown));
	this->Disconnect(wxEVT_MOTION,
			wxMouseEventHandler(CanvasAnisotropy::OnMotion));
	this->Disconnect(wxEVT_PAINT,
			wxPaintEventHandler(CanvasAnisotropy::OnPaint));
	this->Disconnect(wxEVT_SIZE, wxSizeEventHandler(CanvasAnisotropy::OnSize));
}

void CanvasAnisotropy::OnSize(wxSizeEvent &event) {
	this->Refresh();
}

void CanvasAnisotropy::OnPaint(wxPaintEvent &event) {

	wxPoint temp;
	wxPaintDC dc(this);
	wxSize sz = GetClientSize();

	wxString text = wxString::Format(_T("CanvasAnisotropy"));
	dc.DrawText(text, 10, 10);

	float height = 1.0; //TODO: <-Replace this
	float width = 1.0; //TODO: <-Replace this

	float scaleX = (float) sz.x / width;
	float scaleY = (float) sz.y / (height);

	float scaleFactor = (scaleX < scaleY) ? scaleX : scaleY;
	scaleFactor *= 0.9;

	int mx = sz.x / 2;
	int my = (int) (scaleY * height * 0.5);

	dc.CrossHair(mx, my);

}

void CanvasAnisotropy::OnMotion(wxMouseEvent &event) {

}

void CanvasAnisotropy::OnLeftDown(wxMouseEvent &event) {

}

