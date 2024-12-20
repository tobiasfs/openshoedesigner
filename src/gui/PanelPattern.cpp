///////////////////////////////////////////////////////////////////////////////
// Name               : PanelPattern.cpp
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
#include "PanelPattern.h"

#include <wx/dcclient.h>

PanelPattern::PanelPattern(wxWindow *parent, wxWindowID id, const wxPoint &pos,
		const wxSize &size, long style) :
		wxPanel(parent, id, pos, size, style) {

	this->SetBackgroundColour(wxColour(200, 200, 200));

	// Connect Events
	this->Connect(wxEVT_LEFT_DOWN,
			wxMouseEventHandler(PanelPattern::OnLeftDown));
	this->Connect(wxEVT_MOTION, wxMouseEventHandler(PanelPattern::OnMotion));
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(PanelPattern::OnPaint));
	this->Connect(wxEVT_SIZE, wxSizeEventHandler(PanelPattern::OnSize));
}

PanelPattern::~PanelPattern() {
	// Disconnect Events
	this->Disconnect(wxEVT_LEFT_DOWN,
			wxMouseEventHandler(PanelPattern::OnLeftDown));
	this->Disconnect(wxEVT_MOTION, wxMouseEventHandler(PanelPattern::OnMotion));
	this->Disconnect(wxEVT_PAINT, wxPaintEventHandler(PanelPattern::OnPaint));
	this->Disconnect(wxEVT_SIZE, wxSizeEventHandler(PanelPattern::OnSize));
}

void PanelPattern::OnSize(wxSizeEvent &event) {
	this->Refresh();
}

void PanelPattern::OnPaint(wxPaintEvent &event) {

	wxPoint temp;
	wxPaintDC dc(this);
	wxSize sz = GetClientSize();

	float height = 1.0; //TODO: <-Replace this
	float width = 1.0; //TODO: <-Replace this

	float scaleX = (float) sz.x / width;
	float scaleY = (float) sz.y / (height);

	float scaleFactor = (scaleX < scaleY) ? scaleX : scaleY;
	scaleFactor *= 0.9;

	int mx = sz.x / 2;
	int my = (int) (scaleY * height * 0.5);

	dc.CrossHair(mx, my);

	wxString text = wxString::Format(_T("Walkcycle"));
	dc.DrawText(text, 10, 10);
}

void PanelPattern::OnMotion(wxMouseEvent &event) {
}

void PanelPattern::OnLeftDown(wxMouseEvent &event) {
}

