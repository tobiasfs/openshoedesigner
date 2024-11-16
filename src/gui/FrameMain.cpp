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

#include "IDs.h"
#include "DialogQuickInitFoot.h"
#include "FrameDebugParser.h"
#include "DialogEditorFootModel.h"

#include "../3D/FileSTL.h"
#include "../languages.h"
#include <wx/cmdproc.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <math.h>
#include <cfloat>

#include "../project/command/CommandFootMeasurementSet.h"
#include "../project/command/CommandFootModelSetParameter.h"
#include "../project/command/CommandProjectSetLegLengthDifference.h"
#include "../project/command/CommandShoePreset.h"
#include "../project/command/CommandShoeSetParameter.h"

#include "../icons/FootMeasurements.xpm"
#include "../icons/FootMeasurements_small.xpm"
#include "../project/command/CommandFootMeasurementsCopy.h"
#include "../project/command/CommandProjectSetParameter.h"

#include "FrameParent.h"

FrameMain::FrameMain(wxDocument *doc, wxView *view, wxConfig *config,
		wxDocParentFrame *parent) :
		GUIFrameMain(doc, view, parent), bm0(FootMeasurements_small_xpm), bm1(
				FootMeasurements_xpm), loopGuard(1, 1) {
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

	m_menuPreferences->Append(ID_SETUPLANGUAGE, _T("Change Language"));
#ifdef USE_6DOFCONTROLLER
	m_menuPreferences->Append(ID_SETUPCONTROLLER, _("Setup 6DOF &Controller"));
#endif
	m_menuPreferences->Append(ID_SETUPSTEREO3D, _("Setup &Stereo 3D"));
#ifdef USE_PORTMIDI
	m_menuPreferences->Append(ID_SETUPMIDI, _("Setup &MIDI"));
#endif
	m_menuPreferences->Append(ID_SETUPUNITS,
	_("Setup &Units") + wxT("\tCtrl+U"));

	m_menuHelp->AppendSeparator();
	m_menuHelp->Append(wxID_HELP, _("&Help") + wxT("\tF1"));
	m_menuHelp->Append(wxID_ABOUT, _("&About") + wxT("\tF2"));

	doc->GetCommandProcessor()->SetEditMenu(m_menuEdit);
	doc->GetCommandProcessor()->Initialize();

	FrameParent *parentframe = wxStaticCast(parent, FrameParent);
	SettingsStereo3D *settings = &(parentframe->settingsStereo3D);

	m_canvas3D->SetProjectView(wxStaticCast(view, ProjectView));
	settings->WriteToCanvas(m_canvas3D);

	filepaths.Load(config);

	TransferDataToWindow();

	this->Connect(ID_REFRESHVIEW3D, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::RefreshCanvas));
	this->Connect(ID_REFRESHVIEW, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::RefreshView));
	this->Connect(ID_UPDATEPROJECT, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::UpdateProject));

	timer.SetOwner(this);
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(FrameMain::OnTimer), NULL,
			this);
	timer.Start(500);

	Project *project = wxStaticCast(doc, Project);
	project->Update();
}

FrameMain::~FrameMain() {
	printf("FrameMain: Destructor called\n");
	this->Disconnect(ID_UPDATEPROJECT, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::UpdateProject));
	this->Disconnect(ID_REFRESHVIEW, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::RefreshView));
	this->Disconnect(ID_REFRESHVIEW3D, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(FrameMain::RefreshCanvas));

	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(FrameMain::OnTimer), NULL,
			this);

	filepaths.Save(config);
}

bool FrameMain::TransferDataFromWindow() {
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	projectview->showBones = m_menuView->IsChecked(ID_SHOWBONES);
	projectview->showSkin = m_menuView->IsChecked(ID_SHOWSKIN);
	projectview->showLeg = m_menuView->IsChecked(ID_SHOWLEG);
	projectview->showLast = m_menuView->IsChecked(ID_SHOWLAST);
	projectview->showInsole = m_menuView->IsChecked(ID_SHOWINSOLE);
	projectview->showUpper = m_menuView->IsChecked(ID_SHOWUPPER);
	projectview->showSole = m_menuView->IsChecked(ID_SHOWSOLE);
	projectview->showCutaway = m_menuView->IsChecked(ID_SHOWCUTAWAY);
	projectview->showFloor = m_menuView->IsChecked(ID_SHOWFLOOR);
	projectview->showCoordinateSystem = m_menuView->IsChecked(
	ID_SHOWCOORDINATESYSTEM);
	projectview->showBackground = m_menuView->IsChecked(ID_SHOWBACKGROUND);
	return true;
}

bool FrameMain::TransferDataToWindow() {
	if (loopGuard.TryWait() != wxSEMA_NO_ERROR)
		return false;

	const ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	const Project *project = wxStaticCast(GetDocument(), Project);
	const FootMeasurements *meas = projectview->GetActiveFootMeasurements();
	const Configuration *config = &(project->config);

	// Set checkboxes and selections in main menu

//	if (project->measurementsource
//			== Project::MeasurementSource::fromMeasurements)
//		m_menuFoot->Check(
//		ID_USEFOOTMEASUREMENTS,
//				project->measurementsource
//						== Project::MeasurementSource::fromMeasurements);
//	if (project->measurementsource == Project::MeasurementSource::fromFootScan)
//		m_menuFoot->Check(
//		ID_USEFOOTSCAN,
//				project->measurementsource
//						== Project::MeasurementSource::fromFootScan);
//	m_menuFoot->Check(ID_MEASUREMENTSYMMETRY, project->measurementsSymmetric);
//
//	if (project->modeltype == Project::ModelType::boneBased)
//		m_menuFoot->Check(
//		ID_USEBONEBASEDMODEL,
//				project->modeltype == Project::ModelType::boneBased);
//	if (project->modeltype == Project::ModelType::lastBased)
//		m_menuFoot->Check(
//		ID_USELASTBASEDMODEL,
//				project->modeltype == Project::ModelType::lastBased);
//
//	if (project->generator == Project::Generator::Experimental)
//		m_menuConstruction->Check(
//		ID_CONSTRUCTIONEXPERIMENTAL,
//				project->generator == Project::Generator::Experimental);
//	if (project->generator == Project::Generator::Welted)
//		m_menuConstruction->Check(
//		ID_CONSTRUCTIONWELDED,
//				project->generator == Project::Generator::Welted);
//	if (project->generator == Project::Generator::Cemented)
//		m_menuConstruction->Check(
//		ID_CONSTRUCTIONCEMENTED,
//				project->generator == Project::Generator::Cemented);
//	if (project->generator == Project::Generator::Molded)
//		m_menuConstruction->Check(
//		ID_CONSTRUCTIONMOLDED,
//				project->generator == Project::Generator::Molded);
//	if (project->generator == Project::Generator::Dutch)
//		m_menuConstruction->Check(
//		ID_CONSTRUCTIONDUTCH, project->generator == Project::Generator::Dutch);
//	if (project->generator == Project::Generator::Geta)
//		m_menuConstruction->Check(
//		ID_CONSTRUCTIONGETA, project->generator == Project::Generator::Geta);

	m_menuView->Check(ID_STEREO3D,
			m_canvas3D->stereoMode != OpenGLCanvas::Stereo3D::Off);
	m_menuView->Check(ID_SHOWLEFT, projectview->showLeft);
	m_menuView->Check(ID_SHOWRIGHT, projectview->showRight);
	m_menuView->Check(ID_SHOWBONES, projectview->showBones);
	m_menuView->Check(ID_SHOWSKIN, projectview->showSkin);
	m_menuView->Check(ID_SHOWLEG, projectview->showLeg);
	m_menuView->Check(ID_SHOWLAST, projectview->showLast);
	m_menuView->Check(ID_SHOWINSOLE, projectview->showInsole);
	m_menuView->Check(ID_SHOWSOLE, projectview->showSole);
	m_menuView->Check(ID_SHOWUPPER, projectview->showUpper);
	m_menuView->Check(ID_SHOWCUTAWAY, projectview->showCutaway);
	m_menuView->Check(ID_SHOWFLOOR, projectview->showFloor);
	m_menuView->Check(ID_SHOWCOORDINATESYSTEM,
			projectview->showCoordinateSystem);
	m_menuView->Check(ID_SHOWBACKGROUND, projectview->showBackground);

	// On Page "Foot":
	if (projectview->active == ProjectView::Side::Left
			|| projectview->active == ProjectView::Side::Both) {
		m_toggleBtnEditLeft->SetValue(true);
		m_toggleBtnEditLeft1->SetValue(true);
	} else {
		m_toggleBtnEditLeft->SetValue(false);
		m_toggleBtnEditLeft1->SetValue(false);
	}
	if (projectview->active == ProjectView::Side::Right
			|| projectview->active == ProjectView::Side::Both) {
		m_toggleBtnEditRight->SetValue(true);
		m_toggleBtnEditRight1->SetValue(true);
	} else {
		m_toggleBtnEditRight->SetValue(false);
		m_toggleBtnEditRight1->SetValue(false);
	}

//	if (project->measurementsource
//			== Project::MeasurementSource::fromMeasurements) {
//		m_choicebookMeasurement->SetSelection(0);
//	} else {
//		m_choicebookMeasurement->SetSelection(1);
//	}
//	if (project->modeltype == Project::ModelType::boneBased) {
//		m_choicebookFootModel->SetSelection(0);
//	} else {
//		m_choicebookFootModel->SetSelection(1);
//	}
//
//	TransferParameterToTextCtrl(foot->footLength, m_textCtrlFootLength,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->ballWidth, m_textCtrlBallWidth,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->bigToeGirth, m_textCtrlBigToeGirth,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->littleToeGirth, m_textCtrlLittleToeGirth,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->waistGirth, m_textCtrlWaistGirth,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->heelGirth, m_textCtrlHeelGirth,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->heelWidth, m_textCtrlHeelWidth,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->angleMixing, m_textCtrlAngleMixing,
//			UnitType::Percent);
//	TransferParameterToTextCtrl(project->legLengthDifference,
//			m_textCtrlLegLengthDifference, UnitType::Distance);
//
//	m_textCtrlShoeSizeEU->SetValue(
//			wxString::Format(_T("%g"),
//					round(foot->GetSize(FootMeasurements::Type::EU))));
//	m_textCtrlShoeSizeUS->SetValue(
//			wxString::Format(_T("%g"),
//					round(foot->GetSize(FootMeasurements::Type::US))));
//	m_textCtrlShoeSizeUK->SetValue(
//			wxString::Format(_T("%g"),
//					round(foot->GetSize(FootMeasurements::Type::UK))));
//	m_textCtrlShoeSizeCN->SetValue(
//			wxString::Format(_T("%g"),
//					round(foot->GetSize(FootMeasurements::Type::CN))));
//	m_textCtrlShoeSizeJP->SetValue(
//			wxString::Format(_T("%g"),
//					round(foot->GetSize(FootMeasurements::Type::JP))));
//	m_textCtrlShoeSizeAU->SetValue(
//			wxString::Format(_T("%g"),
//					round(foot->GetSize(FootMeasurements::Type::AU))));
//
//	m_filePickerLastModel->SetFileName(
//			wxFileName(project->lastModelL.filename));
//
//	// On page "Leg":
//
//	TransferParameterToTextCtrl(foot->belowCrutchGirth,
//			m_textCtrlBelowCrutchGirth, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->belowCrutchLevel,
//			m_textCtrlBelowCrutchLevel, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->middleOfCalfGirth,
//			m_textCtrlMiddleOfCalfGirth, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->middleOfCalfLevel,
//			m_textCtrlMiddleOfCalfLevel, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->aboveKneeGirth, m_textCtrlAboveKneeGirth,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->aboveKneeLevel, m_textCtrlAboveKneeLevel,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->overKneeCapGirth,
//			m_textCtrlOverKneeCapGirth, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->overKneeCapLevel,
//			m_textCtrlOverKneeCapLevel, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->belowKneeGirth, m_textCtrlBelowKneeGirth,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->belowKneeLevel, m_textCtrlBelowKneeLevel,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(foot->middleOfShankGirth,
//			m_textCtrlMiddleOfShankGirth, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->middleOfShankLevel,
//			m_textCtrlMiddleOfShankLevel, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->aboveAnkleGirth,
//			m_textCtrlAboveAnkleGirth, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->aboveAnkleLevel,
//			m_textCtrlAboveAnkleLevel, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->overAnkleBoneGirth,
//			m_textCtrlOverAnkleBoneGirth, UnitType::Distance);
//	TransferParameterToTextCtrl(foot->overAnkleBoneLevel,
//			m_textCtrlOverAnkleBoneLevel, UnitType::Distance);
//
//	// On page "Insole":
//
//	TransferParameterToTextCtrl(shoe->bigToeAngle, m_textCtrlBigToeAngle,
//			UnitType::Angle);
//	TransferParameterToTextCtrl(shoe->littleToeAngle, m_textCtrlLittleToeAngle,
//			UnitType::Angle);
//	TransferParameterToTextCtrl(shoe->ballMeasurementAngle,
//			m_textCtrlBallMeasurementAngle, UnitType::Angle);
//	TransferParameterToTextCtrl(shoe->heelDirectionAngle,
//			m_textCtrlHeelDirectionAngle, UnitType::Angle);
//
//	m_sliderTipSharpness->SetValue(shoe->tipSharpness->ToDouble());
//
//	TransferParameterToTextCtrl(shoe->extraLength, m_textCtrlExtraLength,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(shoe->footCompression,
//			m_textCtrlFootCompression, UnitType::Percent);

	// On page "Shoe":

	{	// Populate the wxChoiceCtrl for the shoe-type
		wxArrayString newStrings;
		newStrings.Add(_("Custom"));
		JSON &jsst = presets["Type"];
		if (jsst.IsArray()) {
			for (size_t n = 0; n < jsst.Size(); ++n)
				newStrings.Add(jsst[n]["Name"].GetString());
		}
		wxArrayString temp = m_choiceShoeType->GetStrings();
		if (newStrings != temp) {
			m_choiceShoeType->Set(newStrings);
			m_choiceShoeType->SetSelection(0);
		}
	}
	{	// Populate the wxChoiceCtrl for the shaft-height.
		wxArrayString newStrings;
		newStrings.Add(_("Custom"));
		JSON &jsh = presets["Height"];
		if (jsh.IsArray()) {
			for (size_t n = 0; n < jsh.Size(); ++n)
				newStrings.Add(jsh[n]["Name"].GetString());
		}
		wxArrayString temp = m_choiceShoeHeight->GetStrings();
		if (newStrings != temp) {
			m_choiceShoeHeight->Set(newStrings);
			m_choiceShoeHeight->SetSelection(0);
		}
	}

//	TransferParameterToTextCtrl(shoe->heelHeight, m_textCtrlHeelHeight,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(shoe->ballHeight, m_textCtrlBallHeight,
//			UnitType::Distance);
//	TransferParameterToTextCtrl(shoe->heelPitch, m_textCtrlHeelPitch,
//			UnitType::Angle);
//	TransferParameterToTextCtrl(shoe->toeSpring, m_textCtrlToeSpring,
//			UnitType::Angle);
//	TransferParameterToTextCtrl(shoe->upperLevel, m_textCtrlUpperLevel,
//			UnitType::Without);

	loopGuard.Post();

	return true;
}

void FrameMain::TransferParameterToTextCtrl(
		const std::shared_ptr<Parameter> parameter, wxTextCtrl *ctrl,
		UnitType type) {
	FrameParent *parent = wxStaticCast(GetParent(), FrameParent);
	CollectionUnits *units = &(parent->units);

	ctrl->SetToolTip(parameter->GetDescription());

//	if (ctrl->HasFocus()) {
//		ctrl->SetBackgroundColour(wxNullColour);
//		ctrl->SetValue(parameter->GetFormula());
//	} else {
//		if (parameter->errorFlag) {
//			ctrl->SetBackgroundColour(*wxRED);
//			ctrl->SetValue(parameter->errorStr);
//		} else {
//			ctrl->SetBackgroundColour(wxNullColour);
//			switch (type) {
//			case UnitType::Without:
//				ctrl->SetValue(
//						wxString::Format(_T("%g"), parameter->ToDouble()));
//				break;
//			case UnitType::Time:
//				ctrl->SetValue(
//						units->Time.TextFromSIWithUnit(parameter->ToDouble()));
//				break;
//			case UnitType::Distance:
//				ctrl->SetValue(
//						units->Distance.TextFromSIWithUnit(
//								parameter->ToDouble(), 1));
//				break;
//			case UnitType::SmallDistance:
//				ctrl->SetValue(
//						units->SmallDistance.TextFromSIWithUnit(
//								parameter->ToDouble(), 1));
//				break;
//			case UnitType::Tolerance:
//				ctrl->SetValue(
//						units->Tolerance.TextFromSIWithUnit(
//								parameter->ToDouble(), 1));
//				break;
//			case UnitType::Angle:
//				ctrl->SetValue(
//						units->Angle.TextFromSIWithUnit(parameter->ToDouble(),
//								1));
//				break;
//			case UnitType::Percent:
//				ctrl->SetValue(
//						wxString::Format(_T("%g %%"),
//								parameter->ToDouble() * 100));
//				break;
//			default:
//				throw(std::logic_error(
//						__FILE__ "TransferParameterToTextCtrl(): Unhandeled Type of unit."));
//			}
//		}
//	}
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

	case ID_MEASUREMENT_BELOWCRUTCHGIRTH:
		return m_textCtrlBelowCrutchGirth;
	case ID_MEASUREMENT_BELOWCRUTCHLEVEL:
		return m_textCtrlBelowCrutchLevel;
	case ID_MEASUREMENT_MIDDLEOFCALFGIRTH:
		return m_textCtrlMiddleOfCalfGirth;
	case ID_MEASUREMENT_MIDDLEOFCALFLEVEL:
		return m_textCtrlMiddleOfCalfLevel;
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
	return NULL;
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

void FrameMain::OnClose(wxCloseEvent &event) {
	wxDocument *doc = this->GetDocument();
	wxList tempDocs = doc->GetDocumentManager()->GetDocuments();
	wxList tempViews = doc->GetViews();
	Project *project = wxStaticCast(doc, Project);
	project->StopAllThreads();

	printf("FrameMain: %lu docs, %lu views\n", tempDocs.GetCount(),
			tempViews.GetCount());

	if (tempDocs.GetCount() > 1) {
		event.Skip(); // Only close this window, by passing the event to the default handler.
		return;
	}
	if (tempViews.GetCount() > 1) {
		event.Skip(); // Only close this window, by passing the event to the default handler.
		return;
	}
	wxWindow *main = this->GetParent();
	printf("FrameMain: parent->Close()\n");
	main->Close(); // Exit app by closing main window, this will close this window as well.
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
	Project *project = wxStaticCast(GetDocument(), Project);
	midi.Poll();
	if(project->vol.Numel() == 8){
		for(size_t n = 0; n < 8; n++)
			project->vol[n] = ((double) midi.cc[n + 1] - 64.0) / 64.0;
	}else{
		project->vol.At(1, 1, 1) = ((double) midi.cc[1] - 64.0) / 64.0;
		project->vol.At(2, 1, 1) = ((double) midi.cc[2] - 64.0) / 64.0;
		project->vol.At(1, 2, 1) = ((double) midi.cc[3] - 64.0) / 64.0;
		project->vol.At(2, 2, 1) = ((double) midi.cc[4] - 64.0) / 64.0;
		project->vol.At(1, 1, 2) = ((double) midi.cc[5] - 64.0) / 64.0;
		project->vol.At(2, 1, 2) = ((double) midi.cc[6] - 64.0) / 64.0;
		project->vol.At(1, 2, 2) = ((double) midi.cc[7] - 64.0) / 64.0;
		project->vol.At(2, 2, 2) = ((double) midi.cc[8] - 64.0) / 64.0;
	}
	project->vol.CalcSurface();

	this->Refresh();
#endif
}

void FrameMain::OnSetFocus(wxFocusEvent &event) {
	const int id = event.GetId();
	const Project *project = wxStaticCast(GetDocument(), Project);
	const ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	if (!project->parameter.HasID(id, (size_t) projectview->active))
		return;

	if (id == ID_TIPSHARPNESS)
		return;
	TransferDataToWindow();
}

void FrameMain::OnKillFocus(wxFocusEvent &event) {
	const int id = event.GetId();
	const Project *project = wxStaticCast(GetDocument(), Project);
	const ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	if (!project->parameter.HasID(id, (size_t) projectview->active))
		return;
	if (id == ID_TIPSHARPNESS)
		return;
	TransferDataToWindow();
}

void FrameMain::OnTextEnter(wxCommandEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	const std::string newFormula = event.GetString().ToStdString();
	const int id = event.GetId();

//	if(Shoe::IsValidID(id)){
//
//	}
//	if(FootMeasurements::IsValidID(id)){
//		CommandFootMeasurementSet * cmd = new CommandFootMeasurementSet(
//				wxString::Format(_("Set %s to %s"),
//						FootMeasurements::GetName(id), newFormula), project,
//				projectview->active, id, newFormula);
//		project->GetCommandProcessor()->Submit(cmd);
//
//	}
//	if(Shoe::IsValidID(id)){
//		CommandShoeSetParameter * cmd = new CommandShoeSetParameter(
//				wxString::Format(_("Set %s to %s"), Shoe::GetName(id),
//						newFormula), project);
//		cmd->AddValue(id, newFormula.ToStdString());
//		project->GetCommandProcessor()->Submit(cmd);
//	}
//	if (id == ID_MEASUREMENT_LEGLENGTHDIFFERENCE) {
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetLegLengthDifference(
//						wxString::Format(_("Set leg length difference to %s"),
//								newFormula), project, newFormula));
//	}

	switch (event.GetId()) {
	case ID_MEASUREMENT_FOOTLENGTH:
	case ID_MEASUREMENT_BALLWIDTH:
	case ID_MEASUREMENT_BIGTOEGIRTH:
	case ID_MEASUREMENT_LITTLETOEGIRTH:
	case ID_MEASUREMENT_WAISTGIRTH:
	case ID_MEASUREMENT_HEELGIRTH:
	case ID_MEASUREMENT_HEELWIDTH:
	case ID_MEASUREMENT_ANGLEMIXING:
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
		m_panelPageFoot->Navigate(
				wxNavigationKeyEvent::FromTab
						| wxNavigationKeyEvent::IsForward);
		break;
	case ID_MEASUREMENT_BELOWCRUTCHGIRTH:
	case ID_MEASUREMENT_BELOWCRUTCHLEVEL:
	case ID_MEASUREMENT_MIDDLEOFCALFGIRTH:
	case ID_MEASUREMENT_MIDDLEOFCALFLEVEL:
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
		m_panelPageShoe->Navigate(
				wxNavigationKeyEvent::FromTab
						| wxNavigationKeyEvent::IsForward);
		break;
	}
}

void FrameMain::OnMouseWheel(wxMouseEvent &event) {
	DEBUGOUT << "OnMouseWheel( " << event.GetId() << " )\n";

}

void FrameMain::OnScroll(wxScrollEvent &event) {
	DEBUGOUT << "OnScroll( " << event.GetId() << " )\n";
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
	default:
		DEBUGOUT << "OnChoice( " << event.GetId() << " )\n";
		event.Skip();
		return;
	}
	JSON &jsst = presets[key];
	if (!jsst.IsArray())
		return;
	JSON &jsp = jsst[n];
	if (!jsp.IsObject())
		return;

//	CommandShoeSetParameter *cmd = new CommandShoeSetParameter(
//			wxString::Format(_("Selected Preset %s"), event.GetString()),
//			project);
//	if (jsp.HasKey("HeelHeight"))
//		cmd->AddValue(ID_HEELHEIGHT, jsp["HeelHeight"].GetString());
//	if (jsp.HasKey("BallHeight"))
//		cmd->AddValue(ID_BALLHEIGHT, jsp["BallHeight"].GetString());
//	if (jsp.HasKey("HeelPitch"))
//		cmd->AddValue(ID_HEELPITCH, jsp["HeelPitch"].GetString());
//	if (jsp.HasKey("ToeSpring"))
//		cmd->AddValue(ID_TOESPRING, jsp["ToeSpring"].GetString());
//	if (jsp.HasKey("UpperLevel"))
//		cmd->AddValue(ID_UPPERLEVEL, jsp["UpperLevel"].GetString());
//	if (jsp.HasKey("ExtraLength"))
//		cmd->AddValue(ID_EXTRALENGTH, jsp["ExtraLength"].GetString());
//	if (jsp.HasKey("FootCompression"))
//		cmd->AddValue(ID_FOOTCOMPRESSION, jsp["FootCompression"].GetString());
//	project->GetCommandProcessor()->Submit(cmd);
	TransferDataToWindow();
}

void FrameMain::OnToggleButton(wxCommandEvent &event) {
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	Project *project = wxStaticCast(GetDocument(), Project);
//	if (project->measurementsSymmetric) {
//		projectview->active = ProjectView::Side::Both;
//		TransferDataToWindow();
//		return;
//	}
	switch (event.GetId()) {
	case ID_EDITLEFT:
		if (projectview->active == ProjectView::Side::Right) {
			projectview->active = ProjectView::Side::Both;
		} else {
			projectview->active = ProjectView::Side::Right;
		}
		break;
	case ID_EDITRIGHT:
		if (projectview->active == ProjectView::Side::Left) {
			projectview->active = ProjectView::Side::Both;
		} else {
			projectview->active = ProjectView::Side::Left;
		}
		break;
	default:
		printf("ToggleButton %u pressed.\n", event.GetId());
		break;
	}
	TransferDataToWindow();
}

void FrameMain::OnCheckBox(wxCommandEvent &event) {
	DEBUGOUT << "OnCheckBox( " << event.GetId() << " )\n";
}

void FrameMain::OnListCtrlOnSelectionChanged(wxDataViewEvent &event) {
}

void FrameMain::OnPageChange(wxNotebookEvent &event) {
	if (loopGuard.TryWait() != wxSEMA_NO_ERROR)
		return;
	DEBUGOUT << "OnPageChange( " << event.GetId() << " )\n";

	Project *project = wxStaticCast(GetDocument(), Project);
//	switch (event.GetId()) {
//	case ID_MEASUREMENTSOURCE:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(_("Set source."), project,
//						event.GetId(),
//						m_choicebookMeasurement->GetSelection()));
//		break;
//	case ID_FOOTMODEL:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(_("Set footmodel."), project,
//						event.GetId(), m_choicebookFootModel->GetSelection()));
//		break;
//	default:
//		return;
//	}
	loopGuard.Post();
	TransferDataToWindow();

}

void FrameMain::OnViewChange(wxCommandEvent &event) {
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
	TransferDataFromWindow();

	switch (event.GetId()) {
	case ID_SHOWLEFT:
		projectview->showLeft = event.IsChecked();
		break;
	case ID_SHOWRIGHT:
		projectview->showRight = event.IsChecked();
		break;
	}
	TransferDataToWindow();
	Refresh();
}

void FrameMain::On3DSelect(wxMouseEvent &event) {
	int x = event.GetX();
	int y = event.GetY();
	OpenGLPick result;
	m_canvas3D->OnPick(result, x, y);
	if (result.HasHits()) {
		result.SortByNear();

		switch (result.Get(0, 0)) {
//		case 1:
//			{
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
//				break;
//			}
//		case 3:
//			{
//				wxString temp;
//				temp = wxString::Format(_T("U: %i, V: %i"), result.Get(2),
//						result.Get(3));
//				m_statusBar->SetStatusText(temp);
//				break;
//			}
		default: {
			m_statusBar->SetStatusText(wxString());
			break;
		}
		}

	} else {
		m_statusBar->SetStatusText(wxString());
	}
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

void FrameMain::OnSetSymmetry(wxCommandEvent &event) {
}

void FrameMain::OnEditShape(wxCommandEvent &event) {
}

void FrameMain::OnAddBridge(wxCommandEvent &event) {
}

void FrameMain::OnDeleteBridge(wxCommandEvent &event) {
}

void FrameMain::OnPatternSelect(wxTreeListEvent &event) {
}

void FrameMain::OnPatternAdd(wxCommandEvent &event) {
}

void FrameMain::OnPatternSelectFabric(wxCommandEvent &event) {
}

void FrameMain::OnChoiceDisplay(wxCommandEvent &event) {
	DEBUGOUT << "OnChoiceDisplay( " << event.GetId() << " )\n";

}

void FrameMain::OnToggleAnkleLock(wxCommandEvent &event) {
	DEBUGOUT << "OnToggleAnkleLock( " << event.GetId() << " )\n";

}

void FrameMain::OnFileChangedScanFile(wxFileDirPickerEvent &event) {
	DEBUGOUT << "OnFileChangedScanFile( " << event.GetId() << " )\n";
}

void FrameMain::OnFileChangedLastFile(wxFileDirPickerEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);
	project->LoadLastModel(event.GetPath());
	Refresh();
}

void FrameMain::OnQuickSetupMeasurements(wxCommandEvent&WXUNUSED(event)) {
	DialogQuickInitFoot dialog(this);
	if (dialog.ShowModal() == wxID_OK) {
		Project *project = wxStaticCast(GetDocument(), Project);
		ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
		FrameParent *parent = wxStaticCast(GetParent(), FrameParent);
		CollectionUnits *units = &(parent->units);

		wxString lengthStr = units->Distance.TextFromSIWithUnit(dialog.length);

//		project->GetCommandProcessor()->Submit(
//				new CommandFootMeasurementSet(
//				_("Set footlength to ") + lengthStr, project,
//						projectview->active, ID_MEASUREMENT_FOOTLENGTH,
//						lengthStr));
//
//		wxString widthStr = wxString::Format(_T("footLength/%g"),
//				2.7 / dialog.width);
//		project->GetCommandProcessor()->Submit(
//				new CommandFootMeasurementSet(
//				_("Set ballwidth to ") + widthStr, project,
//						projectview->active, ID_MEASUREMENT_BALLWIDTH,
//						widthStr));
	}
}

void FrameMain::OnChangeModel(wxCommandEvent &event) {
	if (loopGuard.TryWait() != wxSEMA_NO_ERROR)
		return;
	DEBUGOUT << "OnChangeModel( " << event.GetId() << " )\n";
	Project *project = wxStaticCast(GetDocument(), Project);
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
//	switch (event.GetId()) {
//	case ID_MEASUREMENTSYMMETRY:
//		projectview->active = ProjectView::Side::Both;
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(_("Change symmetry"), project,
//				ID_MEASUREMENTSYMMETRY, (int) !project->measurementsSymmetric));
//		break;
//	case ID_USEFOOTMEASUREMENTS:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(_("Use foot measurements"),
//						project,
//						ID_MEASUREMENTSOURCE,
//						(int) Project::MeasurementSource::fromMeasurements));
//		break;
//	case ID_USEFOOTSCAN:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(_("Use foot scan"), project,
//				ID_MEASUREMENTSOURCE,
//						(int) Project::MeasurementSource::fromFootScan));
//		break;
//	case ID_USEBONEBASEDMODEL:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(_("Use bone-based model"),
//						project,
//						ID_FOOTMODEL, (int) Project::ModelType::boneBased));
//		break;
//	case ID_USELASTBASEDMODEL:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(_("Use last-based model"),
//						project,
//						ID_FOOTMODEL, (int) Project::ModelType::lastBased));
//		break;
//	}
	loopGuard.Post();
}

void FrameMain::OnCopyMeasurements(wxCommandEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);
	ProjectView *projectview = wxStaticCast(GetView(), ProjectView);
//	switch (event.GetId()) {
//	case ID_COPYACTIVETOINACTIVE:
//		project->GetCommandProcessor()->Submit(
//				new CommandFootMeasurementsCopy(
//						_("Copy active to inactive measurements"), project,
//						projectview->active != ProjectView::Side::Right));
//		break;
//	case ID_COPYLEFTTORIGHT:
//		project->GetCommandProcessor()->Submit(
//				new CommandFootMeasurementsCopy(
//						_("Copy left to right measurements"), project, true));
//		break;
//	case ID_COPYRIGHTTOLEFT:
//		project->GetCommandProcessor()->Submit(
//				new CommandFootMeasurementsCopy(
//						_("Copy right to left measurements"), project, false));
//		break;
//	}
}

void FrameMain::OnLoadFootSTL(wxCommandEvent &event) {
	DEBUGOUT << "OnLoadFootSTL( " << event.GetId() << " )\n";
}

void FrameMain::OnEditBoneModel(wxCommandEvent &event) {
	DialogEditorFootModel *dialog = new DialogEditorFootModel(this);

	FrameParent *parentframe = wxStaticCast(GetParent(), FrameParent);
	try {
#ifdef USE_PORTMIDI
		dialog->SetMidi(&(parentframe->midi));
		dialog->Show();
#endif
	} catch (std::exception &e) {
		DEBUGOUT << "Exception: " << e.what() << "\n";
	}
}

void FrameMain::OnLoadBoneModel(wxCommandEvent &event) {
	FrameParent *parent = wxStaticCast(GetParent(), FrameParent);

	wxFileDialog dialog(this, _("Open Foot Model..."), _T(""), _T(""),
			_("Foot Model (*.fmd; *.txt)|*.fmd;*.txt|All Files|*.*"),
			wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (wxDir::Exists(filepaths.lastFootDirectory)) {
		dialog.SetDirectory(filepaths.lastFootDirectory);
	}

	if (dialog.ShowModal() == wxID_OK) {
		wxFileName fileName(dialog.GetPath());
		Project *project = wxStaticCast(GetDocument(), Project);

		try {
			project->LoadFootModel(fileName.GetFullPath());
			filepaths.lastFootDirectory = fileName.GetPath();
			TransferDataToWindow();
		} catch (const std::exception &ex) {
			std::cerr << "Could not save Foot model: " << ex.what() << '\n';
		}
	}
}

void FrameMain::OnSaveBoneModel(wxCommandEvent &event) {
	FrameParent *parent = wxStaticCast(GetParent(), FrameParent);

	wxFileDialog dialog(this, _("Save Foot Model As..."), _T(""), _T(""),
			_("Foot Model (*.fmd; *.txt)|*.fmd;*.txt|All Files|*.*"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (wxDir::Exists(filepaths.lastFootDirectory)) {
		dialog.SetDirectory(filepaths.lastFootDirectory);
	}

//	if(project.fileName.IsOk()) dialog.SetFilename(
//			project.fileName.GetFullPath());

	if (dialog.ShowModal() == wxID_OK) {
		wxFileName fileName(dialog.GetPath());
		Project *project = wxStaticCast(GetDocument(), Project);
		try {
			project->SaveFootModel(fileName.GetFullPath());
			filepaths.lastFootDirectory = fileName.GetPath();
			TransferDataToWindow();
		} catch (const std::exception &ex) {
			std::cerr << "Could not save bone-model: " << ex.what() << '\n';
		}
	}
}

void FrameMain::OnConstructionSelection(wxCommandEvent &event) {
	Project *project = wxStaticCast(GetDocument(), Project);

//	switch (event.GetId()) {
//	case ID_SELECTCONSTRUCTION:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(_("Set construction method"),
//						project,
//						ID_SELECTCONSTRUCTION, event.GetSelection()));
//		break;
//	case ID_CONSTRUCTIONEXPERIMENTAL:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(
//						_("Select experimental generator"), project,
//						ID_SELECTCONSTRUCTION,
//						(int) Project::Generator::Experimental));
//		break;
//	case ID_CONSTRUCTIONWELDED:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(_("Select welted construction"),
//						project,
//						ID_SELECTCONSTRUCTION,
//						(int) Project::Generator::Welted));
//		break;
//	case ID_CONSTRUCTIONCEMENTED:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(
//						_("Select cemented construction"), project,
//						ID_SELECTCONSTRUCTION,
//						(int) Project::Generator::Cemented));
//		break;
//	case ID_CONSTRUCTIONMOLDED:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(
//						_("Select molded sole construction"), project,
//						ID_SELECTCONSTRUCTION,
//						(int) Project::Generator::Molded));
//		break;
//	case ID_CONSTRUCTIONDUTCH:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(
//						_("Select generator for Dutch clogs"), project,
//						ID_SELECTCONSTRUCTION,
//						(int) Project::Generator::Dutch));
//		break;
//	case ID_CONSTRUCTIONGETA:
//		project->GetCommandProcessor()->Submit(
//				new CommandProjectSetParameter(
//						_("Select generator for Japanese getas"), project,
//						ID_SELECTCONSTRUCTION, (int) Project::Generator::Geta));
//		break;
//	}
	TransferDataToWindow();
}

void FrameMain::OnLoadPattern(wxCommandEvent &event) {
	DEBUGOUT << "OnLoadPattern( " << event.GetId() << " )\n";
}

void FrameMain::OnSavePattern(wxCommandEvent &event) {
	DEBUGOUT << "OnSavePattern( " << event.GetId() << " )\n";
}

void FrameMain::OnSaveLast(wxCommandEvent &event) {
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
	}
}

void FrameMain::OnSaveInsole(wxCommandEvent &event) {
	DEBUGOUT << "OnSaveInsole( " << event.GetId() << " )\n";

}

void FrameMain::OnSaveSole(wxCommandEvent &event) {
	DEBUGOUT << "OnSaveSole( " << event.GetId() << " )\n";

}

void FrameMain::OnSaveCutaway(wxCommandEvent &event) {
	DEBUGOUT << "OnSaveCutaway( " << event.GetId() << " )\n";

}

void FrameMain::OnPackZip(wxCommandEvent &event) {
	DEBUGOUT << "OnPackZip( " << event.GetId() << " )\n";

}

void FrameMain::OnSetupBackgroundImages(wxCommandEvent &event) {
	DEBUGOUT << "OnSetupBackgroundImages( " << event.GetId() << " )\n";
}

void FrameMain::OnDebugParser(wxCommandEvent &event) {
	(new FrameDebugParser(this))->Show();
}

