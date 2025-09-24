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

#include "FrameParent.h"

#include "../Config.h"
#include "../system/MidiPort.h"

#include <iostream>
#include <memory>
#include <set>
#include <string>

DialogSetupMidi::DialogSetupMidi(wxWindow *parent) :
		GUIDialogMidiSetup(parent) {
}

void DialogSetupMidi::UpdateDevices() {
	FrameParent *parentframe = wxStaticCast(GetParent(), FrameParent);
	std::shared_ptr<MidiPort> &midiport = parentframe->midiport;

	if (!midiport)
		return;

	auto devicenames = midiport->GetDeviceNames();
	m_choice->Clear();
	int n = 0;
	for (auto &name : devicenames)
		m_choice->Insert(name, n++);
}

void DialogSetupMidi::OnClose(wxCommandEvent &event) {
	this->Close();
}

void DialogSetupMidi::OnConnectDisconnect(wxCommandEvent &event) {
	FrameParent *parentframe = wxStaticCast(GetParent(), FrameParent);
	std::shared_ptr<MidiPort> &midiport = parentframe->midiport;
	std::shared_ptr<MidiDevice> &mididevice = parentframe->mididevice;

	if (!midiport)
		return;

	if (m_buttonConnectDisconnect->GetLabel() == _T("Connect")) {
		if (m_choice->GetSelection() < 0)
			return;
		wxString device = m_choice->GetStringSelection();

		mididevice = midiport->Open(device.ToStdString(),
				MidiPort::Direction::Bidirectional);
		mididevice->Poll();
//		for(int i = 0; i < 100; i++)
//			d->cc[i] = 64;
//		d->Poll();

		m_buttonConnectDisconnect->SetLabel(_T("Disconnect"));
		m_choice->Enable(false);
	} else {
		mididevice.reset();
		midiport->CloseAll();
		m_buttonConnectDisconnect->SetLabel(_T("Connect"));
		m_choice->Enable(true);
	}
}

void DialogSetupMidi::Load(wxConfig *config) {
	if (config == nullptr)
		return;
	FrameParent *parentframe = wxStaticCast(GetParent(), FrameParent);
	std::shared_ptr<MidiPort> &midiport = parentframe->midiport;
	std::shared_ptr<MidiDevice> &mididevice = parentframe->mididevice;
	if (!midiport)
		return;

	const bool connect = config->ReadBool("MidiConnected", false);
	const std::string device = config->Read("MidiName", "").ToStdString();

	if (connect) {
		mididevice = midiport->Open(device, MidiPort::Direction::Bidirectional);
		mididevice->Poll();
		m_buttonConnectDisconnect->SetLabel(_T("Disconnect"));
		m_choice->Enable(false);
	} else {
		mididevice.reset();
		midiport->CloseAll();
		m_buttonConnectDisconnect->SetLabel(_T("Connect"));
		m_choice->Enable(true);
	}
}

void DialogSetupMidi::Save(wxConfig *config) {
	if (config == nullptr)
		return;

	FrameParent *parentframe = wxStaticCast(GetParent(), FrameParent);
	std::shared_ptr<MidiPort> &midiport = parentframe->midiport;
	std::shared_ptr<MidiDevice> &mididevice = parentframe->mididevice;
	if (!midiport) {
		config->Write(_T("MidiConnected"), (bool) false);
		return;
	}
	if (mididevice) {
		config->Write(_T("MidiConnected"), (bool) true);
		config->Write(_T("MidiName"), wxString(mididevice->GetName()));
	} else {
		config->Write(_T("MidiConnected"), (bool) false);
		config->Write(_T("MidiName"), m_choice->GetStringSelection());
	}
}
