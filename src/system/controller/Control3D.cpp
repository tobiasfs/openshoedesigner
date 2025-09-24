///////////////////////////////////////////////////////////////////////////////
// Name               : Control3D.cpp
// Purpose            : Main class for 6DOF controller
// Thread Safe        : Yes
// Platform dependent : Yes
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.12.2009
// Copyright          : (C) 2009 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "Control3D.h"

Control3D::Control3D() {
	controller = nullptr;
	SetType(1);
}

Control3D::~Control3D() {
	Close();
	if (controller != nullptr)
		delete controller;
}

bool Control3D::SetType(char id) {
	if (controller != nullptr) {
		if (id == controller->ReturnID())
			return true;
		delete controller;
		controller = nullptr;
	}
	switch (id) {
	case CONTROLSPACEBALL_ID:
		controller = new ControlSpaceBall;
		break;
	case CONTROLSPACEORB_ID:
		controller = new ControlSpaceOrb;
		break;
	case CONTROLSPACEMOUSE_ID:
		controller = new ControlSpaceMouse;
		break;
	default:
		return false;
	}
	return true;
}

char Control3D::GetType() {
	if (controller == nullptr)
		return 0;
	return controller->ReturnID();
}

bool Control3D::Open(std::string connection) {
	if (controller == nullptr)
		return false;
	Close();
	controller->SetPort(connection);
	return controller->Open();
}

bool Control3D::Open() {
	if (controller == nullptr)
		return false;
	Close();
	return controller->Open();
}

void Control3D::Close() {
	controller->Close();
}

bool Control3D::IsOpen() const {
	if (controller == nullptr)
		return false;
	return controller->IsConnected();
}

bool Control3D::HasChanged() {
	if (controller == nullptr)
		return 0;
	return controller->HasChanged();
}

bool Control3D::IsIdle() {
	if (controller == nullptr)
		return true;
	unsigned char i;
	for (i = 0; i < 6; i++)
		if (controller->GetAxis(i) != 0)
			return false;
	for (i = 0; i < controller->maxButtons; i++)
		if (controller->GetButton(i) != 0)
			return false;
	return true;
}

int Control3D::GetButton(unsigned char i) {
	if (controller == nullptr)
		return 0;
	return controller->GetButton(i);
}

int Control3D::GetAxis(unsigned char i) {
	if (controller == nullptr)
		return 0;
	return controller->GetAxis(i);
}

std::string Control3D::GetPort() {
	if (controller == nullptr)
		return std::string("");
	return controller->GetPort();
}

bool Control3D::SetPort(std::string port) {
	if (controller == nullptr)
		return false;
	controller->SetPort(port);
	return true;
}

bool Control3D::Pump() {
	if (controller == nullptr)
		return false;
	return controller->Pump();
}

bool Control3D::Load(wxConfig *config) {
	wxASSERT(config!=nullptr);

	wxString str;

	if (config->Read(_T("Control_Type"), &str)) {
		if (str == _T("Spaceball")) {
			SetType(CONTROLSPACEBALL_ID);
		}
		if (str == _T("Spaceorb")) {
			SetType(CONTROLSPACEORB_ID);
		}
		if (str == _T("Spacemouse")) {
			SetType(CONTROLSPACEMOUSE_ID);
		}

	}

	bool found = config->Read(_T("Control_Port"), &str);

	if (!found || str.IsEmpty()) {
#ifdef __LINUX
		str = _T("/dev/ttyS0");
#endif
#ifdef __WIN
		str = _T("COM1");
#endif
	}
	SetPort(str.ToStdString());
	if (config->Read(_T("Control_ActivateOnStartUp"), &str)) {
		if (str == _T("Yes")) {
			this->Open();
		}
	}
	return true;
}

bool Control3D::Save(wxConfig *config) {
	wxASSERT(config!=nullptr);

	if (controller == nullptr)
		return false;

	switch (GetType()) {
	case CONTROLSPACEBALL_ID:
		config->Write(_T("Control_Type"), _T("Spaceball"));
		break;
	case CONTROLSPACEORB_ID:
		config->Write(_T("Control_Type"), _T("Spaceorb"));
		break;
	case CONTROLSPACEMOUSE_ID:
		config->Write(_T("Control_Type"), _T("Spacemouse"));
		break;
	}

	config->Write(_T("Control_Port"), wxString(GetPort()));

	if (IsOpen()) {
		config->Write(_T("Control_ActivateOnStartUp"), _T("Yes"));
	} else {
		config->Write(_T("Control_ActivateOnStartUp"), _T("No"));
	}

	return true;
}
