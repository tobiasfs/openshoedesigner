///////////////////////////////////////////////////////////////////////////////
// Name               : DialogQuickInitFoot.cpp
// Purpose            : Dialog to initialize foot and last settings
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 01.11.2015
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

#include "DialogQuickInitFoot.h"

DialogQuickInitFoot::DialogQuickInitFoot(wxWindow *parent) :
		GUIDialogQuickInitFoot(parent) {
//	parser.ResetVariables(false);
//	parser.autoEvaluate = true;
	length = 0;
	width = 0;
}

void DialogQuickInitFoot::OnTextEnter(wxCommandEvent &event) {
	ParseText(event.GetString().ToStdString());

	//	m_textCtrlShoeSize->Navigate();
	// m_buttonInit->SetFocus();
}

void DialogQuickInitFoot::OnClose(wxCommandEvent &event) {
	ParseText(m_textCtrlShoeSize->GetValue().ToStdString());
	event.Skip();
}

void DialogQuickInitFoot::ParseText(std::string text) {

	const auto idx = text.find_last_not_of("EUSCNKJPA");
	if (idx == std::string::npos) {
		//Error: No Number found.
		return;
	}
	std::string unit;
	if ((idx + 1) < text.size()) {
		unit = text.substr(idx + 1, std::string::npos);
		text = text.substr(0, idx + 1);
	}

	if (!unit.empty()) {
		if (unit.compare("EU") == 0)
			m_choiceUnit->SetSelection(0);
		if (unit.compare("US") == 0)
			m_choiceUnit->SetSelection(1);
		if (unit.compare("CN") == 0)
			m_choiceUnit->SetSelection(2);
		if (unit.compare("UK") == 0)
			m_choiceUnit->SetSelection(3);
		if (unit.compare("JP") == 0)
			m_choiceUnit->SetSelection(4);
		if (unit.compare("AU") == 0)
			m_choiceUnit->SetSelection(5);
		if (unit.compare("mm") == 0)
			m_choiceUnit->SetSelection(6);
		if (unit.compare("cm") == 0)
			m_choiceUnit->SetSelection(7);
		if (unit.compare("in") == 0)
			m_choiceUnit->SetSelection(8);
		if (unit.compare("ft") == 0)
			m_choiceUnit->SetSelection(9);
	}

	width = 1.0;
	if (m_radioBtnSmall->GetValue())
		width = 0.9;
	if (m_radioBtnWide->GetValue())
		width = 1.1;

	MathParser parser;
	parser.ParseExpression(text);
	MathParser::VM &vm = parser.vm;
	vm.Reset();
	vm.Run();
	if (!vm.stack.empty()) {
		const double num = vm.stack.front().ToDouble();
		const Unit unit_2 = vm.stack.front().GetUnit();
		m_textCtrlShoeSize->SetValue(wxString::Format(_T("%g"), num));
		switch (m_choiceUnit->GetSelection()) {
		case 0:
			length = num / 150 - 1.5e-2;
			break;
		case 1:
			length = (num + 21.5) / 3 * 2.54e-2;
			break;
		case 2:
			length = num / 100;
			break;
		case 3:
			length = ((num + 25) * 8.46e-3) - 1.5e-2;
			break;
		case 4:
			length = num / 1000;
			break;
		case 5:
			length = ((num + 25) * 8.46e-3) - 1.5e-2;
			break;
		case 6:
			length = num * 1e-3;
			break;
		case 7:
			length = num * 1e-2;
			break;
		case 8:
			length = num * 2.54e-2;
			break;
		case 9:
			length = num * 0.3048;
			break;
		}
	}
}
