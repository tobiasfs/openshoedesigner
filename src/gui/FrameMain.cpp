///////////////////////////////////////////////////////////////////////////////
// Name               : FrameMain.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 11.08.2015
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

#include "FrameMain.h"

#include "../languages.h"
#include "DialogEditorFootModel.h"
#include "DialogQuickInitFoot.h"
#include "FrameCalculator.h"
#include "FrameParent.h"
#include "FrameSetupBackgroundImages.h"
#include "IDs.h"
#include "SemaphoreTryLocker.h"

#include "../project/command/CommandConfigSetEnum.h"
#include "../project/command/CommandConfigSetParameter.h"
#include "../project/command/CommandConfigSetString.h"
#include "../project/command/CommandFootMeasurementsCopy.h"
#include "../project/command/CommandFootMeasurementSet.h"
#include "../project/command/CommandFootModelSetParameter.h"

#include "icons/FootMeasurements_small.xpm"
#include "icons/FootMeasurements.xpm"
#include "icons/Logo.xpm"

#include <wx/cmdproc.h>
#include <wx/dir.h>
#include <wx/filedlg.h>
#include <wx/filename.h>

#include <cfloat>
#include <cmath>

FrameMain::FrameMain(wxDocument *doc, wxView *view, wxConfig *config,
		wxDocParentFrame *parent) :
		GUIFrameMain(doc, view, parent), bm0(FootMeasurements_small_xpm), bm1(
				FootMeasurements_xpm), loopGuard(1, 1) {

	DEBUGOUT << "FrameMain: Constructor called.\n";

	wxIconBundle logo;
	logo.AddIcon(wxIcon(Logo_xpm));
	SetIcons(logo);

	this->config = config;

	presets = JSON::Load("data/Presets.json");

	m_menuFile->Append(wxID_NEW);
	m_menuFile->Append(wxID_OPEN);
	m_menuFile->Append(wxID_REVERT_TO_SAVED);
	wxMenu *m_menuRecent = new wxMenu();
	m_menuFile->Append(
			new wxMenuItem(m_menuFile, wxID_ANY, _("&Recently opened"),
					wxEmptyString, wxITEM_NORMAL, m_menuRecent));
	doc->GetDocumentManager()->FileHistoryAddFilesToMenu(m_menuRecent);
	m_menuFile->Append(wxID_SAVE);
	m_menuFile->Append(wxID_SAVEAS);
	m_menuFile->AppendSeparator();
	m_menuFile->Append(wxID_PRINT);
	m_menuFile->Append(wxID_PREVIEW);
	m_menuFile->Append(wxID_PRINT_SETUP, _("Print Setup..."));
	m_menuFile->AppendSeparator();
	m_menuFile->Append(wxID_CLOSE);
	m_menuFile->Append(wxID_EXIT);

	m_menuEdit->AppendSeparator();
	m_menuEdit->Insert(0, wxID_UNDO);
	m_menuEdit->Insert(1, wxID_REDO);
	m_menuEdit->Append(ID_UPDATEPROJECT, _("&Update") + wxT("\tF5"),
			_T("Recalculated the shoe."));

	m_menuPreferences->Append(ID_SETUPPATHS, _("Setup &Paths"));
#ifdef USE_6DOFCONTROLLER
	m_menuPreferences->Append(ID_SETUPCONTROLLER, _("Setup 6DOF &Controller"));
#endif
	m_menuPreferences->Append(ID_SETUPSTEREO3D, _("Setup &Stereo 3D"));
#ifdef USE_PORTMIDI
	m_menuPreferences->Append(ID_SETUPMIDI, _("Setup &MIDI"));
#endif
	m_menuPreferences->Append(ID_SETUPLANGUAGE, _T("Change Language"));
	m_menuPreferences->Append(ID_SETUPUNITS,
	_("Setup &Units") + wxT("\tCtrl+U"));

	m_menuHelp->AppendSeparator();
	m_menuHelp->Append(wxID_HELP, _("&Help") + wxT("\tF1"));
	m_menuHelp->Append(wxID_ABOUT, _("&About") + wxT("\tF2"));

	doc->GetCommandProcessor()->SetEditMenu(m_menuEdit);
	doc->GetCommandProcessor()->Initialize();

	FrameParent *parentframe = wxStaticCast(parent, FrameParent);
	SettingsStereo3D *settings = &(parentframe->settingsStereo3D);

#ifdef USE_6DOFCONTROLLER
	m_canvas3D->SetController(parentframe->control);
#endif
	m_canvas3D->SetProjectView(wxStaticCast(view, ProjectView));
	settings->WriteToCanvas(m_canvas3D);
	m_canvas3D->Refresh();

	filepaths.Load(config);

	m_editorCode->SetLexer(wxSTC_LEX_CPP);
	m_editorCode->SetWrapMode(wxSTC_WRAP_WORD);
	m_editorCode->SetWrapVisualFlags(wxSTC_WRAPVISUALFLAG_END);
	m_editorCode->StyleSetForeground(wxSTC_C_NUMBER, wxColour("BLUE"));

	TransferDataToWindow();

	this->Connect(ID_REFRESHVIEW3D, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::RefreshCanvas));
	this->Connect(ID_REFRESHVIEW, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::RefreshView));
	this->Connect(ID_UPDATEPROJECT, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::UpdateProject));

	timer.SetOwner(this);
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(FrameMain::OnTimer), nullptr,
			this);
	timer.Start(100);

	this->Connect(wxEVT_DESTROY,
			wxWindowDestroyEventHandler(FrameMain::OnDestroy), nullptr, this);

//	Project *project = wxStaticCast(doc, Project);
//	project->Update();
}

FrameMain::~FrameMain() {

	timer.Stop();

	DEBUGOUT << "FrameMain: Destructor called.\n";

	this->Disconnect(wxEVT_DESTROY,
			wxWindowDestroyEventHandler(FrameMain::OnDestroy), nullptr, this);

	this->Disconnect(ID_UPDATEPROJECT, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::UpdateProject));
	this->Disconnect(ID_REFRESHVIEW, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::RefreshView));
	this->Disconnect(ID_REFRESHVIEW3D, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::RefreshCanvas));

	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(FrameMain::OnTimer),
			nullptr, this);

	filepaths.Save(config);
}

void FrameMain::OnDestroy(wxWindowDestroyEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) called.\n";

	timer.Stop();
}

bool FrameMain::TransferDataFromWindow() {
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	projectview->showBones = m_menuView->IsChecked(ID_BONES);
	projectview->showSkin = m_menuView->IsChecked(ID_SKIN);
	projectview->showLeg = m_menuView->IsChecked(ID_LEG);
	projectview->showLast = m_menuView->IsChecked(ID_LAST);
	projectview->showInsole = m_menuView->IsChecked(ID_INSOLE);
	projectview->showUpper = m_menuView->IsChecked(ID_UPPER);
	projectview->showHeel = m_menuView->IsChecked(ID_HEEL);
	projectview->showCutaway = m_menuView->IsChecked(ID_CUTAWAY);
	projectview->showFloor = m_menuView->IsChecked(ID_FLOOR);
	projectview->showCoordinateSystem = m_menuView->IsChecked(
	ID_COORDINATESYSTEM);
	projectview->showBackground = m_menuView->IsChecked(ID_BACKGROUND);
	return true;
}

bool FrameMain::TransferDataToWindow() {
//	return true;
	SemaphoreTryLocker locker(loopGuard);
	if (!locker.IsOK())
		return false;

	const ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	const Project *project = wxStaticCast(GetDocument(), Project);
	const FootMeasurements *meas = projectview->GetActiveFootMeasurements();
	const auto &config = project->config;

	m_canvasInsole->insoleL =
			(projectview->showLeft) ? project->insoleFlatL : nullptr;
	m_canvasInsole->insoleR =
			(projectview->showRight) ? project->insoleFlatR : nullptr;
	m_canvasInsole->dL = project->footL.ballWidth->ToDouble() * 0.75;
	m_canvasInsole->dR = project->footR.ballWidth->ToDouble() * 0.75;

	m_canvasPattern->design = project->design;

	// Set checkboxes and selections in main menu

	m_menuFootParameters->Check(ID_MEASUREMENTS,
			config.measurementSource->IsSelection("measurementBased"));
	m_menuFootParameters->Check(ID_SCAN,
			config.measurementSource->IsSelection("scanBased"));

	m_menuLastModel->Check(ID_CONSTRUCTION,
			config.lastConstructionType->IsSelection("construct"));
	m_menuLastModel->Check(ID_BONES,
			config.lastConstructionType->IsSelection("boneBased"));
	m_menuLastModel->Check(ID_LAST,
			config.lastConstructionType->IsSelection("loadFromFile"));

	m_menuConstruction->Check(
	ID_CONSTRUCTION_EXPERIMENTAL,
			config.generator->IsSelection("experimental"));
	m_menuConstruction->Check(
	ID_CONSTRUCTION_WELDED, config.generator->IsSelection("welted"));
	m_menuConstruction->Check(
	ID_CONSTRUCTION_CEMENTED, config.generator->IsSelection("cemented"));
	m_menuConstruction->Check(
	ID_CONSTRUCTION_MOLDED, config.generator->IsSelection("molded"));
	m_menuConstruction->Check(
	ID_CONSTRUCTION_DUTCH, config.generator->IsSelection("dutch"));
	m_menuConstruction->Check(
	ID_CONSTRUCTION_GETA, config.generator->IsSelection("geta"));

	m_menuView->Check(ID_STEREO3D,
			m_canvas3D->stereoMode != OpenGLCanvas::Stereo3D::Off);
	m_menuView->Check(ID_LEFT, projectview->showLeft);
	m_menuView->Check(ID_RIGHT, projectview->showRight);
	m_menuView->Check(ID_BONES, projectview->showBones);
	m_menuView->Check(ID_SKIN, projectview->showSkin);
	m_menuView->Check(ID_LEG, projectview->showLeg);
	m_menuView->Check(ID_LAST, projectview->showLast);
	m_menuView->Check(ID_INSOLE, projectview->showInsole);
	m_menuView->Check(ID_HEEL, projectview->showHeel);
	m_menuView->Check(ID_UPPER, projectview->showUpper);
	m_menuView->Check(ID_CUTAWAY, projectview->showCutaway);
	m_menuView->Check(ID_FLOOR, projectview->showFloor);
	m_menuView->Check(ID_COORDINATESYSTEM, projectview->showCoordinateSystem);
	m_menuView->Check(ID_BACKGROUND, projectview->showBackground);

	// On Page "Foot":

	switch (projectview->active) {
	case ProjectView::Side::Left:
		m_radioBtnEditLeft->SetValue(true);
		m_radioBtnEditLegLeft->SetValue(true);
		break;
	case ProjectView::Side::Both:
		m_radioBtnEditBoth->SetValue(true);
		m_radioBtnEditLegBoth->SetValue(true);
		break;
	case ProjectView::Side::Right:
		m_radioBtnEditRight->SetValue(true);
		m_radioBtnEditLegRight->SetValue(true);
		break;
	}

	m_choicebookMeasurementSource->SetSelection(
			config.measurementSource->GetSelectionIdx());

	TransferParameterToTextCtrl(meas->footLength, m_textCtrlFootLength,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->ballWidth, m_textCtrlBallWidth,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->bigToeGirth, m_textCtrlBigToeGirth,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->littleToeGirth, m_textCtrlLittleToeGirth,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->waistGirth, m_textCtrlWaistGirth,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->heelGirth, m_textCtrlHeelGirth,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->heelWidth, m_textCtrlHeelWidth,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->angleMixing, m_textCtrlAngleMixing,
			UnitType::Percent);
	TransferParameterToTextCtrl(meas->legLengthDifference,
			m_textCtrlLegLengthDifference, UnitType::Distance);

	m_textCtrlShoeSizeEU->SetValue(
			wxString::Format(_T("%g"),
					round(meas->GetSize(FootMeasurements::Type::EU))));
	m_textCtrlShoeSizeUS->SetValue(
			wxString::Format(_T("%g"),
					round(meas->GetSize(FootMeasurements::Type::US))));
	m_textCtrlShoeSizeUK->SetValue(
			wxString::Format(_T("%g"),
					round(meas->GetSize(FootMeasurements::Type::UK))));
	m_textCtrlShoeSizeCN->SetValue(
			wxString::Format(_T("%g"),
					round(meas->GetSize(FootMeasurements::Type::CN))));
	m_textCtrlShoeSizeJP->SetValue(
			wxString::Format(_T("%g"),
					round(meas->GetSize(FootMeasurements::Type::JP))));
	m_textCtrlShoeSizeAU->SetValue(
			wxString::Format(_T("%g"),
					round(meas->GetSize(FootMeasurements::Type::AU))));

	m_filePickerScan->SetInitialDirectory(filepaths.measurementDirectory);
	if (!config.filenameScan->GetString().empty()) {
		wxFileName filenameScan;
		filenameScan = wxString(config.filenameScan->GetString());
		filenameScan.MakeAbsolute();
		m_filePickerScan->SetFileName(filenameScan);
	}

// On page "Leg":

	TransferParameterToTextCtrl(meas->belowCrotchGirth,
			m_textCtrlBelowCrotchGirth, UnitType::Distance);
	TransferParameterToTextCtrl(meas->belowCrotchLevel,
			m_textCtrlBelowCrotchLevel, UnitType::Distance);
	TransferParameterToTextCtrl(meas->middleOfThighGirth,
			m_textCtrlMiddleOfThighGirth, UnitType::Distance);
	TransferParameterToTextCtrl(meas->middleOfThighLevel,
			m_textCtrlMiddleOfThighLevel, UnitType::Distance);
	TransferParameterToTextCtrl(meas->aboveKneeGirth, m_textCtrlAboveKneeGirth,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->aboveKneeLevel, m_textCtrlAboveKneeLevel,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->overKneeCapGirth,
			m_textCtrlOverKneeCapGirth, UnitType::Distance);
	TransferParameterToTextCtrl(meas->overKneeCapLevel,
			m_textCtrlOverKneeCapLevel, UnitType::Distance);
	TransferParameterToTextCtrl(meas->belowKneeGirth, m_textCtrlBelowKneeGirth,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->belowKneeLevel, m_textCtrlBelowKneeLevel,
			UnitType::Distance);
	TransferParameterToTextCtrl(meas->middleOfShankGirth,
			m_textCtrlMiddleOfShankGirth, UnitType::Distance);
	TransferParameterToTextCtrl(meas->middleOfShankLevel,
			m_textCtrlMiddleOfShankLevel, UnitType::Distance);
	TransferParameterToTextCtrl(meas->aboveAnkleGirth,
			m_textCtrlAboveAnkleGirth, UnitType::Distance);
	TransferParameterToTextCtrl(meas->aboveAnkleLevel,
			m_textCtrlAboveAnkleLevel, UnitType::Distance);
	TransferParameterToTextCtrl(meas->overAnkleBoneGirth,
			m_textCtrlOverAnkleBoneGirth, UnitType::Distance);
	TransferParameterToTextCtrl(meas->overAnkleBoneLevel,
			m_textCtrlOverAnkleBoneLevel, UnitType::Distance);

	// On page "Last":

	m_choicebookLastConstructionType->SetSelection(
			config.lastConstructionType->GetSelectionIdx());

	TransferParameterToTextCtrl(config.bigToeAngle, m_textCtrlBigToeAngle,
			UnitType::Angle);
	TransferParameterToTextCtrl(config.littleToeAngle, m_textCtrlLittleToeAngle,
			UnitType::Angle);
	TransferParameterToTextCtrl(config.ballMeasurementAngle,
			m_textCtrlBallMeasurementAngle, UnitType::Angle);
	TransferParameterToTextCtrl(config.heelDirectionAngle,
			m_textCtrlHeelDirectionAngle, UnitType::Angle);
	TransferParameterToTextCtrl(config.tipSharpness, m_textCtrlTipSharpness,
			UnitType::Without);
	TransferParameterToTextCtrl(config.extraLength, m_textCtrlExtraLength,
			UnitType::Distance);
	TransferParameterToTextCtrl(config.footCompression,
			m_textCtrlFootCompression, UnitType::Percent);

	m_filePickerBoneModel->SetInitialDirectory(filepaths.measurementDirectory);
	if (!config.filenameBoneModel->GetString().empty()) {
		wxFileName filenameLast;
		filenameLast = wxString(config.filenameBoneModel->GetString());
		filenameLast.MakeAbsolute();
		m_filePickerBoneModel->SetFileName(filenameLast);
	}
	m_filePickerLast->SetInitialDirectory(filepaths.lastDirectory);
	if (!config.filenameLast->GetString().empty()) {
		wxFileName filenameLast;
		filenameLast = wxString(config.filenameLast->GetString());
		filenameLast.MakeAbsolute();
		m_filePickerLast->SetFileName(filenameLast);
	}

	m_checkBoxLastModify->SetValue(config.lastModify->IsSelection("true"));
	m_checkBoxLastReorient->SetValue(config.lastReorient->IsSelection("true"));

	m_choicebookHeelConstructionType->SetSelection(
			config.heelConstructionType->GetSelectionIdx());

	m_filePickerHeel->SetInitialDirectory(filepaths.lastDirectory);
	if (!config.filenameHeel->GetString().empty()) {
		wxFileName filenameLast;
		filenameLast = wxString(config.filenameHeel->GetString());
		filenameLast.MakeAbsolute();
		m_filePickerHeel->SetFileName(filenameLast);
	}

	m_checkBoxHeelReorient->SetValue(config.heelReorient->IsSelection("true"));

	// On page "Shoe":

	{	// Populate the wxChoiceCtrl for the shoe-type
		wxArrayString newStrings;
		newStrings.Add(_("Custom"));
		JSON &jsst = presets["Type"];
		if (jsst.IsArray()) {
			for (size_t n = 0; n < jsst.Size(); n++)
				newStrings.Add(jsst[n]["Name"].GetString());
		}
		wxArrayString temp = m_choicePresetShoeType->GetStrings();
		if (newStrings != temp) {
			m_choicePresetShoeType->Set(newStrings);
			m_choicePresetShoeType->SetSelection(0);
		}
	}
	{	// Populate the wxChoiceCtrl for the shaft-height.
		wxArrayString newStrings;
		newStrings.Add(_("Custom"));
		JSON &jsh = presets["Height"];
		if (jsh.IsArray()) {
			for (size_t n = 0; n < jsh.Size(); n++)
				newStrings.Add(jsh[n]["Name"].GetString());
		}
		wxArrayString temp = m_choicePresetShoeStyle->GetStrings();
		if (newStrings != temp) {
			m_choicePresetShoeStyle->Set(newStrings);
			m_choicePresetShoeStyle->SetSelection(0);
		}
	}

	TransferParameterToTextCtrl(config.heelHeight, m_textCtrlHeelHeight,
			UnitType::Distance);
	TransferParameterToTextCtrl(config.ballHeight, m_textCtrlBallHeight,
			UnitType::Distance);
	TransferParameterToTextCtrl(config.heelPitch, m_textCtrlHeelPitch,
			UnitType::Angle);
	TransferParameterToTextCtrl(config.toeSpring, m_textCtrlToeSpring,
			UnitType::Angle);
	TransferParameterToTextCtrl(config.upperLevel, m_textCtrlUpperLevel,
			UnitType::Without);

	const bool enable = config.heelConstructionType->IsSelection("construct");
	m_choicePresetShoeType->Enable(enable);
	m_textCtrlHeelHeight->Enable(enable);
	m_textCtrlBallHeight->Enable(enable);
	m_textCtrlHeelPitch->Enable(enable);
	m_textCtrlToeSpring->Enable(enable);

	m_choicebookConstruction->SetSelection(config.generator->GetSelectionIdx());

	TransferParameterToTextCtrl(config.thickness, m_textCtrlThickness,
			UnitType::Distance);
	TransferParameterToTextCtrl(config.weltSize, m_textCtrlWeltSize,
			UnitType::Distance);

// On page "Heel"

	TransferParameterToTextCtrl(config.supportHeelRadius,
			m_textCtrlSupportHeelRadius, UnitType::Distance);
	TransferParameterToTextCtrl(config.supportHeelOffset,
			m_textCtrlSupportHeelOffset, UnitType::Distance);
	TransferParameterToTextCtrl(config.supportToeRadius,
			m_textCtrlSupportToeRadius, UnitType::Distance);
	TransferParameterToTextCtrl(config.supportToeOffset,
			m_textCtrlSupportToeOffset, UnitType::Distance);

	{	// Populate the wxChoiceCtrl for the shoe-type
		wxArrayString preset;
		wxArrayString code;
		wxArrayString variant;
		preset.Add(_("Custom"));
		code.Add(_("Custom"));
		variant.Add(_("Custom"));

		if (presets.IsObject() && presets.HasKey("Heel")) {
			const JSON &js = presets["Heel"];
			if (js.IsArray()) {
				for (size_t n = 0; n < js.Size(); n++) {
					const JSON &jsa = js[n];
					wxString codeName;
					if (jsa.HasKey("Name"))
						codeName = jsa["Name"].GetString();
					else
						codeName = wxString::Format("#%u", n);
					code.Add(codeName);

					if (jsa.HasKey("Variant")) {
						const JSON &jsva = jsa["Variant"];
						if (jsva.IsArray()) {
							for (size_t m = 0; m < jsva.Size(); m++) {
								const JSON &jsv = jsva[m];
								wxString varname;
								if (jsv.HasKey("Name"))
									varname = jsv["Name"].GetString();
								else
									varname = wxString::Format("#%u", m);
								variant.Add(varname);
								preset.Add(codeName + " - " + varname);
							}
						}
					} else {
						preset.Add(codeName);
					}
				}
			}
		}
		if (preset != m_choicePresetHeel->GetStrings()) {
			m_choicePresetHeel->Set(preset);
			m_choicePresetHeel->SetSelection(0);
		}
		if (code != m_choicePresetHeelCode->GetStrings()) {
			m_choicePresetHeelCode->Set(code);
			m_choicePresetHeelCode->SetSelection(0);
		}
		if (variant != m_choicePresetHeelVariant->GetStrings()) {
			m_choicePresetHeelVariant->Set(variant);
			m_choicePresetHeelVariant->SetSelection(0);
		}
	}

// On page "Pattern"

// On page "Flattening"

// On page "Analysis"
	m_canvasGraph->values = project->builder.GetDebugMatrix();

	return true;
}

void FrameMain::TransferParameterToTextCtrl(
		const std::shared_ptr<ParameterFormula> parameter, wxTextCtrl *ctrl,
		UnitType type) {
	FrameParent *parent = wxStaticCast(GetParent(), FrameParent);
	CollectionUnits *units = &(parent->units);

	if (parameter->errorFlag) {
		ctrl->SetBackgroundColour(*wxRED);
		ctrl->SetValue(parameter->GetString());
		ctrl->SetToolTip(parameter->errorStr);
	} else {
		if (ctrl->HasFocus()) {
			ctrl->SetBackgroundColour(wxNullColour);
			ctrl->SetToolTip(parameter->GetDescription());
			ctrl->SetValue(parameter->GetString());
		} else {
			ctrl->SetBackgroundColour(wxNullColour);
			ctrl->SetToolTip(parameter->GetDescription());
			switch (type) {
			case UnitType::Without:
				ctrl->SetValue(
						wxString::Format(_T("%g"), parameter->ToDouble()));
				break;
			case UnitType::Time:
				ctrl->SetValue(
						units->Time.TextFromSIWithUnit(parameter->ToDouble()));
				break;
			case UnitType::Distance:
				ctrl->SetValue(
						units->Distance.TextFromSIWithUnit(
								parameter->ToDouble(), 1));
				break;
			case UnitType::SmallDistance:
				ctrl->SetValue(
						units->SmallDistance.TextFromSIWithUnit(
								parameter->ToDouble(), 1));
				break;
			case UnitType::LinearSpeed:
				ctrl->SetValue(
						units->Speed.TextFromSIWithUnit(parameter->ToDouble(),
								1));
				break;
			case UnitType::Tolerance:
				ctrl->SetValue(
						units->Tolerance.TextFromSIWithUnit(
								parameter->ToDouble(), 1));
				break;
			case UnitType::Angle:
				ctrl->SetValue(
						units->Angle.TextFromSIWithUnit(parameter->ToDouble(),
								1));
				break;
			case UnitType::Percent:
				ctrl->SetValue(
						wxString::Format(_T("%g %%"),
								parameter->ToDouble() * 100));
				break;
			default:
				throw(std::logic_error(
						__FILE__ "TransferParameterToTextCtrl(): Unhandeled Type of unit."));
			}
		}
	}
}

wxTextCtrl* FrameMain::GetTextCtrlByID(int id) {
	switch (id) {
	case ID_MEASUREMENT_FOOTLENGTH:
		return m_textCtrlFootLength;
	case ID_MEASUREMENT_BALLWIDTH:
		return m_textCtrlBallWidth;
	case ID_MEASUREMENT_BIGTOEGIRTH:
		return m_textCtrlBigToeGirth;
	case ID_MEASUREMENT_LITTLETOEGIRTH:
		return m_textCtrlLittleToeGirth;
	case ID_MEASUREMENT_WAISTGIRTH:
		return m_textCtrlWaistGirth;
	case ID_MEASUREMENT_HEELGIRTH:
		return m_textCtrlHeelGirth;
	case ID_MEASUREMENT_HEELWIDTH:
		return m_textCtrlHeelWidth;
	case ID_MEASUREMENT_ANGLEMIXING:
		return m_textCtrlAngleMixing;
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
		return m_textCtrlLegLengthDifference;

	case ID_MEASUREMENT_BELOWCROTCHGIRTH:
		return m_textCtrlBelowCrotchGirth;
	case ID_MEASUREMENT_BELOWCROTCHLEVEL:
		return m_textCtrlBelowCrotchLevel;
	case ID_MEASUREMENT_MIDDLEOFTHIGHGIRTH:
		return m_textCtrlMiddleOfThighGirth;
	case ID_MEASUREMENT_MIDDLEOFTHIGHLEVEL:
		return m_textCtrlMiddleOfThighLevel;
	case ID_MEASUREMENT_ABOVEKNEEGIRTH:
		return m_textCtrlAboveKneeGirth;
	case ID_MEASUREMENT_ABOVEKNEELEVEL:
		return m_textCtrlAboveKneeLevel;
	case ID_MEASUREMENT_OVERKNEECAPGIRTH:
		return m_textCtrlOverKneeCapGirth;
	case ID_MEASUREMENT_OVERKNEECAPLEVEL:
		return m_textCtrlOverKneeCapLevel;
	case ID_MEASUREMENT_BELOWKNEEGIRTH:
		return m_textCtrlBelowKneeGirth;
	case ID_MEASUREMENT_BELOWKNEELEVEL:
		return m_textCtrlBelowKneeLevel;
	case ID_MEASUREMENT_MIDDLEOFSHANKGIRTH:
		return m_textCtrlMiddleOfShankGirth;
	case ID_MEASUREMENT_MIDDLEOFSHANKLEVEL:
		return m_textCtrlMiddleOfShankLevel;
	case ID_MEASUREMENT_ABOVEANKLEGIRTH:
		return m_textCtrlAboveAnkleGirth;
	case ID_MEASUREMENT_ABOVEANKLELEVEL:
		return m_textCtrlAboveAnkleLevel;
	case ID_MEASUREMENT_OVERANKLEBONEGIRTH:
		return m_textCtrlOverAnkleBoneGirth;
	case ID_MEASUREMENT_OVERANKLEBONELEVEL:
		return m_textCtrlOverAnkleBoneLevel;

	case ID_HEELHEIGHT:
		return m_textCtrlHeelHeight;
	case ID_BALLHEIGHT:
		return m_textCtrlBallHeight;
	case ID_HEELPITCH:
		return m_textCtrlHeelPitch;
	case ID_TOESPRING:
		return m_textCtrlToeSpring;
	case ID_UPPERLEVEL:
		return m_textCtrlUpperLevel;
	case ID_EXTRALENGTH:
		return m_textCtrlExtraLength;
	case ID_FOOTCOMPRESSION:
		return m_textCtrlFootCompression;
	}
	return nullptr;
}

void FrameMain::OnClose(wxCloseEvent &event) {
	timer.Stop();
	wxDocument *doc = this->GetDocument();
	wxList tempDocs = doc->GetDocumentManager()->GetDocuments();
	wxList tempViews = doc->GetViews();
	Project *project = wxStaticCast(doc, Project);
	project->StopAllThreads();

	DEBUGOUT << "FrameMain: " << tempDocs.GetCount() << " docs, ";
	DEBUGOUT << tempViews.GetCount() << " views\n";

	if (tempDocs.GetCount() > 1) {
		event.Skip(); // Only close this window, by passing the event to the default handler.
		return;
	}
	if (tempViews.GetCount() > 1) {
		event.Skip(); // Only close this window, by passing the event to the default handler.
		return;
	}
	wxWindow *main = this->GetParent();
	DEBUGOUT << "FrameMain: parent->Close()\n";
	main->Close(); // Exit app by closing main window, this will close this window as well.
}

void FrameMain::OnMouseWheel(wxMouseEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) not implemented.\n";
}

void FrameMain::OnSize(wxSizeEvent &event) {
//	if(event.GetId() == ID_IMAGEFOOT){
	wxSize s = m_panelPageFoot->GetSize();
	if (s.GetWidth() > 450)
		m_bitmapFoot->SetBitmap(bm1);
	else
		m_bitmapFoot->SetBitmap(bm0);
//	this->Layout();
//	}
	event.Skip();
}

void FrameMain::OnTimer(wxTimerEvent &event) {
#ifdef USE_PORTMIDI
	wxDocument *doc = GetDocument();
	if (!doc || !doc->GetClassInfo()->IsKindOf(&Project::ms_classInfo))
		return;
	Project *project = wxStaticCast(doc, Project);

	FrameParent *parentframe = wxStaticCast(GetParent(), FrameParent);
	if (parentframe->mididevice && parentframe->mididevice->Poll()) {
		project->config.debugMIDI_48->SetValue(parentframe->mididevice->cc[48]);
		project->config.debugMIDI_49->SetValue(parentframe->mididevice->cc[49]);
		project->config.debugMIDI_50->SetValue(parentframe->mididevice->cc[50]);
		project->config.debugMIDI_51->SetValue(parentframe->mididevice->cc[51]);
		project->config.debugMIDI_52->SetValue(parentframe->mididevice->cc[52]);
		project->config.debugMIDI_53->SetValue(parentframe->mididevice->cc[53]);
		project->config.debugMIDI_54->SetValue(parentframe->mididevice->cc[54]);
		project->config.debugMIDI_55->SetValue(parentframe->mididevice->cc[55]);
		project->Update();
	}

//	wxString status = wxString::Format(_T("use_count=%ld"),
//			parentframe->mididevice.use_count());
//	SetStatusText(status, 1);

	this->Refresh();
#endif

	const auto x = m_canvas3D->unitAtOrigin;
	wxString stat = wxString::Format(_T("unitAtOrigin = %f"), x);
	SetStatusText(stat, 0);
}

void FrameMain::RefreshCanvas(wxCommandEvent&WXUNUSED(event)) {
	m_canvas3D->Refresh();
	Refresh();
}

void FrameMain::RefreshView(wxCommandEvent &event) {
	TransferDataToWindow();
	this->Refresh(); // FIXME: Recursive refresh does not work with GTK1. Call Refresh on every child-dialog by hand.
}

void FrameMain::UpdateProject(wxCommandEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);
	project->Update();
	RefreshView(event);
}

void FrameMain::OnChar(wxKeyEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) not implemented.\n";
	event.Skip();
}

void FrameMain::OnCheckBox(wxCommandEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);

	bool value = event.GetSelection();
	DEBUGOUT << __FUNCTION__ << "(" << value << ")\n";

	switch (event.GetId()) {
	case ID_LASTMODIFY:
	case ID_LASTREORIENT:
	case ID_HEELREORIENT: {
		CommandConfigSetEnum *cmd = new CommandConfigSetEnum(
				wxString::Format(_("Checkbox: %s"), event.GetString()), project,
				event.GetId(), value ? 1 : 0);
		project->GetCommandProcessor()->Submit(cmd);
	}
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented\n";
		break;
	}

	TransferDataToWindow();
}

void FrameMain::OnChoice(wxCommandEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);

	int n = event.GetSelection();
	n = n - 1; // The first selection is the "Custom" selection.
	if (n < 0)
		return;
	int id = event.GetId();
	std::string key;
	switch (id) {
	case
	ID_PRESETSHOETYPE:
		key = "Type";
		break;
	case ID_PRESETSHOEHEIGHT:
		key = "Height";
		break;
	case ID_PRESETHEEL:
		key = "Heel";
		break;
	case ID_PRESETHEELCODE:
		key = "HeelCode";
		break;
	case ID_PRESETHEELVARIANT:
		key = "HeelVariant";
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		event.Skip();
		return;
	}

	if (!presets.IsObject())
		return;
	std::string extra = "";
	if (key == "Heel") {
		if (!presets.HasKey(key))
			return;
		JSON &jsst = presets[key];
		if (!jsst.IsArray())
			return;
		JSON &jsp = jsst[n];
		if (!jsp.IsObject())
			return;

		key = "HeelCode";
	}
	if (!presets.HasKey(key))
		return;
	JSON &jsst = presets[key];
	if (!jsst.IsArray())
		return;
	JSON &jsp = jsst[n];
	if (!jsp.IsObject())
		return;

	CommandConfigSetParameter *cmd = new CommandConfigSetParameter(
			wxString::Format(_("Selected Preset %s"), event.GetString()),
			project);
	if (jsp.HasKey("HeelHeight"))
		cmd->AddValue(ID_HEELHEIGHT, jsp["HeelHeight"].GetString());
	if (jsp.HasKey("BallHeight"))
		cmd->AddValue(ID_BALLHEIGHT, jsp["BallHeight"].GetString());
	if (jsp.HasKey("HeelPitch"))
		cmd->AddValue(ID_HEELPITCH, jsp["HeelPitch"].GetString());
	if (jsp.HasKey("ToeSpring"))
		cmd->AddValue(ID_TOESPRING, jsp["ToeSpring"].GetString());
	if (jsp.HasKey("UpperLevel"))
		cmd->AddValue(ID_UPPERLEVEL, jsp["UpperLevel"].GetString());
	if (jsp.HasKey("ExtraLength"))
		cmd->AddValue(ID_EXTRALENGTH, jsp["ExtraLength"].GetString());
	if (jsp.HasKey("FootCompression"))
		cmd->AddValue(ID_FOOTCOMPRESSION, jsp["FootCompression"].GetString());
	project->GetCommandProcessor()->Submit(cmd);
	TransferDataToWindow();
}

void FrameMain::OnDataViewListCtrlItemEditingDone(wxDataViewEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) not implemented.\n";
}

void FrameMain::OnFileChanged(wxFileDirPickerEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	const std::string newFilename = event.GetPath().ToStdString();
	const int id = event.GetId();

	if (Configuration::IsValidID(id)) {
		CommandConfigSetString *cmd = new CommandConfigSetString(
				wxString::Format(_("Set %s to %s"), Configuration::GetName(id),
						newFilename), project, id, newFilename);
		project->GetCommandProcessor()->Submit(cmd);
	}
	project->Update();
	RefreshView(event);
}

void FrameMain::OnKillFocus(wxFocusEvent &event) {
	const int id = event.GetId();
	const Project *project = wxStaticCast(GetDocument(), Project);
	const ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	size_t pva = (size_t) (
			(projectview->active == ProjectView::Side::Both) ?
					ProjectView::Side::Left : projectview->active);
	if (!project->evaluator.HasID(id, (size_t) pva))
		return;

	TransferDataToWindow();
}

void FrameMain::OnListCtrlOnSelectionChanged(wxDataViewEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) not implemented.\n";
}

void FrameMain::OnNotebookPageChanged(wxNotebookEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);

	if (event.GetId() != ID_MAINVIEW)
		return;

	int newPage = event.GetSelection();
	if (newPage >= 0 && newPage <= 3) {
		projectview->display = (ProjectView::Display) newPage;
		project->Update();
		Refresh();
	}
}

void FrameMain::OnPageChanged(wxNotebookEvent &event) {
	SemaphoreTryLocker locker(loopGuard);
	if (!locker.IsOK())
		return;

	Project *project = wxStaticCast(GetDocument(), Project);
	switch (event.GetId()) {
	case ID_MEASUREMENTSOURCE:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Set measurement source."), project,
						event.GetId(),
						m_choicebookMeasurementSource->GetSelection()));
		break;
	case ID_LASTCONSTRUCTIONTYPE:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Set last generation."), project,
						event.GetId(),
						m_choicebookLastConstructionType->GetSelection()));
		break;
	case ID_HEELCONSTRUCTIONTYPE:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Set heel construction."), project,
						event.GetId(),
						m_choicebookHeelConstructionType->GetSelection()));
		break;
	case ID_GENERATOR:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Set shoe generator."), project,
						event.GetId(),
						m_choicebookConstruction->GetSelection()));
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		return;
	}

	locker.UnLock();
	TransferDataToWindow();
}

void FrameMain::OnRadioButton(wxCommandEvent &event) {
	SemaphoreTryLocker locker(loopGuard);
	if (!locker.IsOK())
		return;
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	switch (event.GetId()) {
	case ID_LEFT:
		projectview->active = ProjectView::Side::Left;
		break;
	case ID_BOTH:
		projectview->active = ProjectView::Side::Both;
		break;
	case ID_RIGHT:
		projectview->active = ProjectView::Side::Right;
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		break;
	}
	locker.UnLock();
	TransferDataToWindow();
}

void FrameMain::OnScroll(wxScrollEvent &event) {
	if (event.GetId() == ID_SPEED) {
		const double speed = ((double) event.GetPosition()) / 1000.0;
		std::string formula = std::to_string(speed) + " m/s";
		m_canvasCycle->speed->SetString(formula);
		m_canvasCycle->speed->Calculate();
		TransferParameterToTextCtrl(m_canvasCycle->speed, m_textCtrlSpeed,
				UnitType::LinearSpeed);
	} else {
		event.Skip();
	}
}

void FrameMain::OnSetFocus(wxFocusEvent &event) {
	const int id = event.GetId();
	const Project *project = wxStaticCast(GetDocument(), Project);
	const ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	size_t pva = (size_t) (
			(projectview->active == ProjectView::Side::Both) ?
					ProjectView::Side::Left : projectview->active);
	if (!project->evaluator.HasID(id, pva))
		return;
	TransferDataToWindow();
}

void FrameMain::OnTextEnter(wxCommandEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	const std::string newFormula = event.GetString().ToStdString();
	const int id = event.GetId();

	if (FootMeasurements::IsValidID(id)) {
		CommandFootMeasurementSet *cmd = new CommandFootMeasurementSet(
				wxString::Format(_("Set %s to %s"),
						FootMeasurements::GetName(id), newFormula), project,
				projectview->active, id, newFormula);
		project->GetCommandProcessor()->Submit(cmd);

	} else if (Configuration::IsValidID(id)) {
		CommandConfigSetParameter *cmd = new CommandConfigSetParameter(
				wxString::Format(_("Set %s to %s"), Configuration::GetName(id),
						newFormula), project);
		cmd->AddValue(id, newFormula);
		project->GetCommandProcessor()->Submit(cmd);
	}

	switch (event.GetId()) {
	case ID_MEASUREMENT_FOOTLENGTH:
	case ID_MEASUREMENT_BALLWIDTH:
	case ID_MEASUREMENT_BIGTOEGIRTH:
	case ID_MEASUREMENT_LITTLETOEGIRTH:
	case ID_MEASUREMENT_WAISTGIRTH:
	case ID_MEASUREMENT_HEELGIRTH:
	case ID_MEASUREMENT_HEELWIDTH:
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
		m_panelPageFoot->Navigate(
				wxNavigationKeyEvent::FromTab
						| wxNavigationKeyEvent::IsForward);
		break;
	case ID_MEASUREMENT_BELOWCROTCHGIRTH:
	case ID_MEASUREMENT_BELOWCROTCHLEVEL:
	case ID_MEASUREMENT_MIDDLEOFTHIGHGIRTH:
	case ID_MEASUREMENT_MIDDLEOFTHIGHLEVEL:
	case ID_MEASUREMENT_ABOVEKNEEGIRTH:
	case ID_MEASUREMENT_ABOVEKNEELEVEL:
	case ID_MEASUREMENT_OVERKNEECAPGIRTH:
	case ID_MEASUREMENT_OVERKNEECAPLEVEL:
	case ID_MEASUREMENT_BELOWKNEEGIRTH:
	case ID_MEASUREMENT_BELOWKNEELEVEL:
	case ID_MEASUREMENT_MIDDLEOFSHANKGIRTH:
	case ID_MEASUREMENT_MIDDLEOFSHANKLEVEL:
	case ID_MEASUREMENT_ABOVEANKLEGIRTH:
	case ID_MEASUREMENT_ABOVEANKLELEVEL:
	case ID_MEASUREMENT_OVERANKLEBONEGIRTH:
	case ID_MEASUREMENT_OVERANKLEBONELEVEL:
		m_panelPageLeg->Navigate(
				wxNavigationKeyEvent::FromTab
						| wxNavigationKeyEvent::IsForward);
		break;
	case ID_HEELHEIGHT:
	case ID_BALLHEIGHT:
	case ID_HEELPITCH:
	case ID_TOESPRING:
	case ID_UPPERLEVEL:
	case ID_EXTRALENGTH:
	case ID_FOOTCOMPRESSION:
	case ID_WELTSIZE:
	case ID_THICKNESS:
		m_panelPageShoe->Navigate(
				wxNavigationKeyEvent::FromTab
						| wxNavigationKeyEvent::IsForward);
		break;
	default:
		break;
	}
}

void FrameMain::On3DSelect(wxMouseEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) called.\n";
	int x = event.GetX();
	int y = event.GetY();





	return;
	OpenGLPick result;
	m_canvas3D->OnPick(result, x, y);
	if (result.HasHits()) {
		result.SortByNear();

		switch (result.Get(0, 0)) {
		case 1: {
//				const int boneNr = result.Get(2);
//				ProjectView* projectview = wxStaticCast(GetView(), ProjectView);
//				const Foot *foot = projectview->foot;
//				if(boneNr < foot->bones.size()){
//					m_statusBar->SetStatusText(foot->bones[boneNr].name);
//
//					m_gridBoneLength->SelectRow(boneNr);
//					m_gridBoneDiameter->SelectRow(boneNr);
//					m_gridSkin->SelectRow(boneNr);
//				}
			break;
		}
		case 3: {
//				wxString temp;
//				temp = wxString::Format(_T("U: %i, V: %i"), result.Get(2),
//						result.Get(3));
//				m_statusBar->SetStatusText(temp);
			break;
		}
		default: {
			m_statusBar->SetStatusText(wxString());
			break;
		}
		}

	} else {
		m_statusBar->SetStatusText(wxString());
	}
}

void FrameMain::OnBackgroundImagesSetup(wxCommandEvent &event) {
	(new FrameSetupBackgroundImages(this))->Show();
}

void FrameMain::OnCalculator(wxCommandEvent &event) {
	auto frame = new FrameCalculator(this);
	FrameParent *parentframe = wxStaticCast(GetParent(), FrameParent);
	frame->mididevice = parentframe->mididevice;
	frame->Show();
}

void FrameMain::OnConstructionChanged(wxCommandEvent &event) {
	SemaphoreTryLocker locker(loopGuard);
	if (!locker.IsOK())
		return;

	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) not implemented.\n";

	Project *project = wxStaticCast(GetDocument(), Project);

	switch (event.GetId()) {

	case ID_CONSTRUCTION_EXPERIMENTAL:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Select experimental generator"),
						project,
						ID_GENERATOR, (int) 0));
		break;
	case ID_CONSTRUCTION_WELDED:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Select welted construction"),
						project,
						ID_GENERATOR, (int) 1));
		break;
	case ID_CONSTRUCTION_CEMENTED:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Select cemented construction"),
						project,
						ID_GENERATOR, (int) 2));
		break;
	case ID_CONSTRUCTION_MOLDED:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Select molded sole construction"),
						project,
						ID_GENERATOR, (int) 3));
		break;
	case ID_CONSTRUCTION_DUTCH:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Select generator for Dutch clogs"),
						project,
						ID_GENERATOR, (int) 4));
		break;
	case ID_CONSTRUCTION_GETA:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(
						_("Select generator for Japanese getas"), project,
						ID_GENERATOR, (int) 5));
		break;
	}
	locker.UnLock();
	TransferDataToWindow();
}

void FrameMain::OnMeasurementsCopy(wxCommandEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	switch (event.GetId()) {
	case ID_ACTIVETOINACTIVE:
		project->GetCommandProcessor()->Submit(
				new CommandFootMeasurementsCopy(
						_("Copy active to inactive measurements"), project,
						projectview->active == ProjectView::Side::Left ?
								ProjectView::Side::Right :
								ProjectView::Side::Left));
		break;
	case ID_LEFTTORIGHT:
		project->GetCommandProcessor()->Submit(
				new CommandFootMeasurementsCopy(
						_("Copy left to right measurements"), project,
						ProjectView::Side::Right));
		break;
	case ID_RIGHTTOLEFT:
		project->GetCommandProcessor()->Submit(
				new CommandFootMeasurementsCopy(
						_("Copy right to left measurements"), project,
						ProjectView::Side::Left));
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		break;
		return;
	}
	TransferDataToWindow();
}

void FrameMain::OnMeasurementsQuickSetup(wxCommandEvent&WXUNUSED(event)) {
	DialogQuickInitFoot dialog(this);
	if (dialog.ShowModal() == wxID_OK) {
		Project *project = wxStaticCast(GetDocument(), Project);
		ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
		FrameParent *parent = wxStaticCast(GetParent(), FrameParent);
		CollectionUnits *units = &(parent->units);

		wxString lengthStr = units->Distance.TextFromSIWithUnit(dialog.length);

		project->GetCommandProcessor()->Submit(
				new CommandFootMeasurementSet(
				_("Set footlength to ") + lengthStr, project,
						projectview->active, ID_MEASUREMENT_FOOTLENGTH,
						lengthStr.ToStdString()));

		wxString widthStr = wxString::Format(_T("footLength/%g"),
				2.7 / dialog.width);
		project->GetCommandProcessor()->Submit(
				new CommandFootMeasurementSet(
				_("Set ballwidth to ") + widthStr, project,
						projectview->active, ID_MEASUREMENT_BALLWIDTH,
						widthStr.ToStdString()));
	}
}

void FrameMain::OnModelChanged(wxCommandEvent &event) {
	SemaphoreTryLocker locker(loopGuard);
	if (!locker.IsOK())
		return;
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " )\n";

	Project *project = wxStaticCast(GetDocument(), Project);
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	switch (event.GetId()) {
	case ID_MEASUREMENTS:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Use foot measurements"), project,
				ID_MEASUREMENTSOURCE, 0));
		break;
	case ID_SCAN:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Use foot scan"), project,
				ID_MEASUREMENTSOURCE, 1));
		break;
	case ID_BONES:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Use bone-based model"), project,
				ID_LASTCONSTRUCTIONTYPE, 0));
		break;
	case ID_LAST:
		project->GetCommandProcessor()->Submit(
				new CommandConfigSetEnum(_("Use last-based model"), project,
				ID_LASTCONSTRUCTIONTYPE, 1));
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		break;
	}
	locker.UnLock();
	TransferDataToWindow();
}

void FrameMain::OnPatternSelect(wxTreeListEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) not implemented.\n";
}

void FrameMain::OnPatternSelectFabric(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetSelection() << " ) not implemented.\n";
}

void FrameMain::OnToggleStereo3D(wxCommandEvent &event) {
	if (m_canvas3D->stereoMode == OpenGLCanvas::Stereo3D::Off) {
		m_canvas3D->stereoMode = OpenGLCanvas::Stereo3D::Anaglyph;
	} else {
		m_canvas3D->stereoMode = OpenGLCanvas::Stereo3D::Off;
	}
	m_menuView->Check(ID_STEREO3D,
			m_canvas3D->stereoMode != OpenGLCanvas::Stereo3D::Off);
	Refresh();
}

void FrameMain::OnViewChanged(wxCommandEvent &event) {
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	Project *project = wxStaticCast(GetDocument(), Project);
	TransferDataFromWindow();

	switch (event.GetId()) {
	case ID_LEFT:
		projectview->showLeft = event.IsChecked();
		break;
	case ID_RIGHT:
		projectview->showRight = event.IsChecked();
		break;
	case ID_BONES:
	case ID_SKIN:
	case ID_LEG:
	case ID_LAST:
	case ID_INSOLE:
	case ID_UPPER:
	case ID_HEEL:
	case ID_CUTAWAY:
	case ID_FLOOR:
	case ID_COORDINATESYSTEM:
	case ID_BACKGROUND:
		// Nothing to do. TransferDataFromWindow already did the read-in.
		// TODO Check if showLeft and showRight can also be merged into that function.
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
	}
	TransferDataToWindow();
	project->Update();
	Refresh();
}

void FrameMain::OnButtonAdd(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) not implemented.\n";
}

void FrameMain::OnButtonCopy(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) not implemented.\n";
}

void FrameMain::OnButtonDelete(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
			<< event.GetId() << " ) not implemented.\n";
}

void FrameMain::OnObjectEdit(wxCommandEvent &event) {
	switch (event.GetId()) {

	case ID_BONES_EDIT:
		try {
			DialogEditorFootModel *dialog = new DialogEditorFootModel(this);
#ifdef USE_PORTMIDI
			FrameParent *parentframe = wxStaticCast(GetParent(), FrameParent);
			dialog->SetMidi(parentframe->mididevice);
#endif
			dialog->Show();
		} catch (std::exception &e) {
			DEBUGOUT << "Exception: " << e.what() << "\n";
		}
		break;

	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		break;
	}
}

void FrameMain::OnObjectLoad(wxCommandEvent &event) {
	FrameParent *parent = wxStaticCast(GetParent(), FrameParent);

	switch (event.GetId()) {
	case ID_LAST: {
		wxFileDialog dialog(this, _("Open Bone Model..."), _T(""), _T(""),
				_("Bone Model (*.fmd; *.txt)|*.fmd;*.txt|All Files|*.*"),
				wxFD_OPEN | wxFD_FILE_MUST_EXIST);

		if (wxDir::Exists(filepaths.lastDirectory)) {
			dialog.SetDirectory(filepaths.lastDirectory);
		}

		if (dialog.ShowModal() == wxID_OK) {
			wxFileName fileName(dialog.GetPath());
			wxString fileString = fileName.GetFullPath();
			Project *project = wxStaticCast(GetDocument(), Project);

			try {
				int id = event.GetId();
				CommandConfigSetString *cmd = new CommandConfigSetString(
						wxString::Format(_("Set %s to %s"),
								Configuration::GetName(id), fileString),
						project, id, fileString.ToStdString());
				project->GetCommandProcessor()->Submit(cmd);

			} catch (const std::exception &ex) {
				std::cerr << "Could not load bone model: " << ex.what() << '\n';
			}
		}
	}
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		break;
	}
	TransferDataToWindow();
}

void FrameMain::OnObjectSave(wxCommandEvent &event) {

	FrameParent *parent = wxStaticCast(GetParent(), FrameParent);
	int id = event.GetId();

	switch (id) {
	case ID_BONES: {
		wxFileDialog dialog(this, _("Save Foot Model As..."), _T(""), _T(""),
				_("Foot Model (*.fmd; *.txt)|*.fmd;*.txt|All Files|*.*"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (wxDir::Exists(filepaths.lastDirectory)) {
			dialog.SetDirectory(filepaths.lastDirectory);
		}

//	if(project.fileName.IsOk()) dialog.SetFilename(
//			project.fileName.GetFullPath());

		if (dialog.ShowModal() == wxID_OK) {
			wxFileName fileName(dialog.GetPath());
			Project *project = wxStaticCast(GetDocument(), Project);
			try {
				project->SaveFootModel(fileName.GetFullPath());
				filepaths.lastDirectory = fileName.GetPath();
				TransferDataToWindow();
			} catch (const std::exception &ex) {
				std::cerr << "Could not save bone-model: " << ex.what() << '\n';
			}
			DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
					<< event.GetId() << " ) not implemented.\n";
		}
	}
		break;

	case ID_LAST: {

		wxFileDialog dialog(this, _("Save last..."), _T(""), _T(""),
				_("STL file (*.stl)|*.stl|All files|*.*"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (dialog.ShowModal() == wxID_OK) {
			wxFileName fileName;
			fileName = dialog.GetPath();
			Project *project = wxStaticCast(GetDocument(), Project);
			ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
			//		if (project->modeltype == Project::ModelType::boneBased) {
			//			project->SaveSkin(fileName.GetFullPath(),
			//					projectview->active != ProjectView::Side::Right,
			//					projectview->active == ProjectView::Side::Right);
			//		} else {
			//			project->SaveLast(fileName.GetFullPath(),
			//					projectview->active != ProjectView::Side::Right,
			//					projectview->active == ProjectView::Side::Right);
			//		}
			DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
					<< event.GetId() << " ) not implemented.\n";
		}
	}
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		break;
	}
}

