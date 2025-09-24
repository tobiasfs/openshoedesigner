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

#include <sstream>

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
	MathParser::VM &vm = parser.vm;
	try {
		parser.ParseExpression(text.ToStdString());
		vm.Reset();
		vm.Run();
	} catch (std::exception &ex) {
		error = std::string(ex.what());
	}
	m_textCtrlError->SetValue(error);
	if (error.empty() && !vm.stack.empty()) {
		const auto &value = vm.stack.front();
		const auto &unit = value.GetUnit();

		m_textCtrlNumber->SetValue(
				wxString::Format(_T("%g"), value.ToDouble()));
		m_textCtrlUnit->SetValue(unit.ToString());

		std::ostringstream sib;
		for (uint8_t n = 0; n < Unit::Size(); n++) {
			Unit::Base b = (Unit::Base) n;
			if (unit[b] != 0)
				sib << unit.GetName(b) << "^" << unit[b] << " ";
		}
		m_textCtrlSIBase->SetValue(wxString(sib.str()));
	} else {
		m_textCtrlNumber->SetValue(_T(""));
		m_textCtrlUnit->SetValue(_T(""));
	}
}
