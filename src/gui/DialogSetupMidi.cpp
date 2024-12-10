///////////////////////////////////////////////////////////////////////////////
// Name               : DialogSetupMidi.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 13.06.2020
// Copyright          : (C) 2020 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "DialogSetupMidi.h"

DialogSetupMidi::DialogSetupMidi(wxWindow* parent, MidiPort * midi,
		wxWindowID id, const wxString& title, const wxPoint& pos,
		const wxSize& size, long style)
		: GUIDialogMidiSetup(parent, id, title, pos, size, style), midi(midi)
{
}

void DialogSetupMidi::UpdateDevices()
{
	auto devicenames = midi->GetDeviceNames();
	m_choice->Clear();
	int n = 0;
	for(auto & name : devicenames)
		m_choice->Insert(name, n++);
}

void DialogSetupMidi::OnClose(wxCommandEvent& event)
{
	this->Close();
}

void DialogSetupMidi::OnConnectDisconnect(wxCommandEvent& event)
{
	if(m_buttonConnectDisconnect->GetLabel() == _T("Connect")){
		if(m_choice->GetSelection() < 0) return;
		wxString device = m_choice->GetStringSelection();
		auto d = midi->Open(device.ToStdString(),
				MidiPort::Direction::Bidirectional);

		d->Poll();
//		for(int i = 0; i < 100; ++i)
//			d->cc[i] = 64;
//		d->Poll();

		m_buttonConnectDisconnect->SetLabel(_T("Disconnect"));
		m_choice->Enable(false);
	}else{
		midi->CloseAll();
		m_buttonConnectDisconnect->SetLabel(_T("Connect"));
		m_choice->Enable(true);
	}
}

