///////////////////////////////////////////////////////////////////////////////
// Name               : FrameDebugParser.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 16.11.2015
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

#include "FrameDebugParser.h"

FrameDebugParser::FrameDebugParser(wxWindow *parent) :
		GUIFrameDebugParser(parent) {
//	parser.autoEvaluate = true;
//	parser.AddAllowedUnit("mm", 1e-3);
//	parser.AddAllowedUnit("cm", 1e-2);
//	parser.AddAllowedUnit("m", 1);
//	parser.AddAllowedUnit("in", 2.54e-2);
//	parser.AddAllowedUnit("ft", 0.3048);
}

void FrameDebugParser::OnCloseX(wxCloseEvent &event) {
	this->Hide();
}

void FrameDebugParser::OnText(wxCommandEvent &event) {
	wxString text;
	text = m_textCtrlExpression->GetValue();
	std::string error;
	try {
		parser.ParseExpression(text.ToStdString());
		parser.InitMachine();
		parser.Run();
	} catch (std::exception &ex) {
		error = std::string(ex.what());
	}
	m_textCtrlError->SetValue(error);
	if (error.empty() && parser.StackSize() >= 1) {
		m_textCtrlNumber->SetValue(
				wxString::Format(_T("%g"), parser.GetStack(0).ToDouble()));
		m_textCtrlUnit->SetValue(parser.GetStack(0).GetUnit().ToString());
	} else {
		m_textCtrlNumber->SetValue(_T(""));
		m_textCtrlUnit->SetValue(_T(""));
	}
}
