///////////////////////////////////////////////////////////////////////////////
// Name               : DialogEditorFootModel.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 14.06.2020
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

#include "DialogEditorFootModel.h"

#include <sstream>

DialogEditorFootModel::DialogEditorFootModel(wxWindow *parent, wxWindowID id,
		const wxString &title, const wxPoint &pos, const wxSize &size,
		long style) :
		GUIFrameEditorFootModel(parent, id, title, pos, size, style) {
	timer.SetOwner(this);

	timer.Start(100); // ms

	this->Connect(wxEVT_TIMER,
			wxTimerEventHandler(DialogEditorFootModel::OnTimer), NULL, this);
}

void DialogEditorFootModel::OnLoad(wxCommandEvent &event) {

	wxFileDialog dialog(this, _("Open Foot Model..."), _T(""), _T(""),
			_("Foot Model (*.json)|*.json|All Files|*.*"),
			wxFD_OPEN | wxFD_FILE_MUST_EXIST);

//		if(wxDir::Exists(filepaths.lastFootDirectory)){
//			dialog.SetDirectory(filepaths.lastFootDirectory);
//		}

	if (dialog.ShowModal() == wxID_OK) {
		wxFileName fileName(dialog.GetPath());
		m_canvasFootModel->model.LoadJSON(fileName.GetFullPath().ToStdString());

		m_canvasFootModel->model.GetBone("Tibia")->matrixinit.SetOrigin(
				Vector3(0, 0, 0));
		m_canvasFootModel->model.CalculateBones();
		m_canvasFootModel->model.CalculateSkin();
		m_canvasFootModel->model.Update();
//		Talus = m_canvasFootModel->model.GetBone("Talus");
//		Calcaneus = m_canvasFootModel->model.GetBone("Calcaneus");
//		Cuboideum = m_canvasFootModel->model.GetBone("Cuboideum");
//		Cuneiforme1 = m_canvasFootModel->model.GetBone("Cuneiforme1");
//		Cuneiforme2 = m_canvasFootModel->model.GetBone("Cuneiforme2");
//		Cuneiforme3 = m_canvasFootModel->model.GetBone("Cuneiforme3");
//
//		PhalanxI1 = m_canvasFootModel->model.GetBone("PhalanxI1");
//		PhalanxI2 = m_canvasFootModel->model.GetBone("PhalanxI2");
//		PhalanxI3 = m_canvasFootModel->model.GetBone("PhalanxI3");
//		PhalanxI4 = m_canvasFootModel->model.GetBone("PhalanxI4");
//		PhalanxI5 = m_canvasFootModel->model.GetBone("PhalanxI5");

		mode = 0;
		m_canvasFootModel->Refresh();
	}
}

void DialogEditorFootModel::OnSave(wxCommandEvent &event) {
//	FrameParent* parent = wxStaticCast(GetParent(), FrameParent);

	wxFileDialog dialog(this, _("Save Foot Model As..."), _T(""), _T(""),
			_("Foot Model (*.json)|*.json|All Files|*.*"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

//		if(wxDir::Exists(filepaths.lastFootDirectory)){
//			dialog.SetDirectory(filepaths.lastFootDirectory);
//		}

	//	if(project.fileName.IsOk()) dialog.SetFilename(
	//			project.fileName.GetFullPath());

	if (dialog.ShowModal() == wxID_OK) {
		wxFileName fileName(dialog.GetPath());
//			Project* project = wxStaticCast(GetDocument(), Project);
		m_canvasFootModel->model.SaveJSON(fileName.GetFullPath().ToStdString());
	}
}

void DialogEditorFootModel::OnClose(wxCommandEvent &event) {
	this->Close();
}

DialogEditorFootModel::~DialogEditorFootModel() {

	this->Disconnect(wxEVT_TIMER,
			wxTimerEventHandler(DialogEditorFootModel::OnTimer),
			NULL, this);
}

void DialogEditorFootModel::SetMidi(MidiPort *midi) {
	device = midi->GetFirstOpenDevice(MidiPort::Direction::Input);

	for (size_t n = 0; n < 32; ++n)
		device->cc[n] = 0;
	device->cc[32] = 127;
	device->Send(0, 56);
}

void DialogEditorFootModel::OnTimer(wxTimerEvent &event) {
	if (device->Poll()) {

		uint8_t temp = mode;
		for (size_t n = 32; n < 48; ++n) {
			if (n == mode)
				continue;
			if (device->cc[n])
				temp = n;
		}
		if (temp != mode) {
			for (size_t n = 32; n < 48; ++n) {
				if (n != temp)
					device->cc[n] = 0;
			}
			mode = temp;
			for (uint8_t n = 48; n <= 55; ++n)
				device->cc[n] = 0;

			switch (mode) {
			case 32:
				SetSlider(48, "Tibia");
				SetSlider(50, "Fibula");
				SetSlider(52, "Talus");
				SetSlider(54, "Talus2");
				break;
			case 33:
				SetSlider(52, "Calcaneus");
				SetSlider(48, "Cuboideum");
				SetSlider(50, "Naviculare");
				break;
			case 34:
				SetSlider(48, "Cuneiforme1");
				SetSlider(50, "Cuneiforme2");
				SetSlider(52, "Cuneiforme3");
				break;
			case 35:
				SetSlider(48, "Metatarsalis1");
				SetSlider(50, "PhalanxI1");
				SetSlider(52, "PhalanxII1");
				SetSlider(54, "PhalanxIII1");
				break;
			case 36:
				SetSlider(48, "Metatarsalis2");
				SetSlider(50, "PhalanxI2");
				SetSlider(52, "PhalanxII2");
				SetSlider(54, "PhalanxIII2");
				break;
			case 37:
				SetSlider(48, "Metatarsalis3");
				SetSlider(50, "PhalanxI3");
				SetSlider(52, "PhalanxII3");
				SetSlider(54, "PhalanxIII3");
				break;
			case 38:
				SetSlider(48, "Metatarsalis4");
				SetSlider(50, "PhalanxI4");
				SetSlider(52, "PhalanxII4");
				SetSlider(54, "PhalanxIII4");
				break;
			case 39:
				SetSlider(48, "Metatarsalis5");
				SetSlider(50, "PhalanxI5");
				SetSlider(52, "PhalanxII5");
				break;
			}
		}

		switch (mode) {
		case 32:
			Update(48, "Tibia");
			Update(50, "Fibula");
			Update(52, "Talus");
			Update(54, "Talus2");
			break;
		case 33:
			Update(52, "Calcaneus");
			Update(48, "Cuboideum");
			Update(50, "Naviculare");
			break;
		case 34:
			Update(48, "Cuneiforme1");
			Update(50, "Cuneiforme2");
			Update(52, "Cuneiforme3");
			break;
		case 35:
			Update(48, "Metatarsalis1");
			Update(50, "PhalanxI1");
			Update(52, "PhalanxII1");
			Update(54, "PhalanxIII1");
			break;
		case 36:
			Update(48, "Metatarsalis2");
			Update(50, "PhalanxI2");
			Update(52, "PhalanxII2");
			Update(54, "PhalanxIII2");
			break;
		case 37:
			Update(48, "Metatarsalis3");
			Update(50, "PhalanxI3");
			Update(52, "PhalanxII3");
			Update(54, "PhalanxIII3");
			break;
		case 38:
			Update(48, "Metatarsalis4");
			Update(50, "PhalanxI4");
			Update(52, "PhalanxII4");
			Update(54, "PhalanxIII4");
			break;
		case 39:
			Update(48, "Metatarsalis5");
			Update(50, "PhalanxI5");
			Update(52, "PhalanxII5");
			break;
		}

		m_canvasFootModel->model.CalculateBones();
		m_canvasFootModel->model.CalculateSkin();
		m_canvasFootModel->Refresh();
	}
}

void DialogEditorFootModel::Update(uint8_t idx, const std::string &name) {
	std::shared_ptr<Bone> bone = m_canvasFootModel->model.GetBone(name);
	if (bone == nullptr)
		return;
	bone->formulaS1 = MakeString((double) (device->cc[idx]) / 1000.0);
	bone->formulaS2 = MakeString((double) (device->cc[idx + 1]) / 1000.0);
}

void DialogEditorFootModel::SetSlider(uint8_t idx, const std::string &name) {
	std::shared_ptr<Bone> bone = m_canvasFootModel->model.GetBone(name);
	if (bone == nullptr)
		return;
	const double s = m_canvasFootModel->model.LRS();

	device->cc[idx] = (uint8_t) (bone->s1 * 1000.0 / s);
	device->cc[idx + 1] = (uint8_t) (bone->s2 * 1000.0 / s);
}

std::string DialogEditorFootModel::MakeString(double value) {
	std::ostringstream str;
	str << "L*R*S*" << value;
	return str.str();
}
