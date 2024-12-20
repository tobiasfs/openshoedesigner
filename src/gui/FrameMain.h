///////////////////////////////////////////////////////////////////////////////
// Name               : FrameMain.h
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

#ifndef FRAMEMAIN_H
#define FRAMEMAIN_H

/*!\class FrameMain
 * \brief Main Window
 *
 * Main window of the application. Has the main 3D display and work area.
 * All other windows and dialogs are children of this frame.
 */

#include <wx/cmdproc.h>
#include <wx/intl.h>
#include <wx/config.h>
#include <wx/thread.h>

#include "gui.h"

#include "../project/Project.h"
#include "../system/JSON.h"
#include "../system/MidiPort.h"

#include "CollectionUnits.h"
#include "CollectionFilepaths.h"
#include "SettingsStereo3D.h"

#include "DialogSetupStereo3D.h"

class ProjectView;

class FrameMain: public GUIFrameMain {
	friend class ProjectView;
public:
	enum class UnitType {
		Without, Time, Distance, SmallDistance, Tolerance, Angle, Percent
	};

	FrameMain(wxDocument *doc, wxView *view, wxConfig *config,
			wxDocParentFrame *parent);
	virtual ~FrameMain();

	wxConfig *config;
	CollectionFilepaths filepaths;

	JSON presets;

protected:
	const wxBitmap bm0;
	const wxBitmap bm1;
	wxSemaphore loopGuard;

	wxTimer timer;

protected:
	bool TransferDataFromWindow() override;
	bool TransferDataToWindow() override;

private:
	void TransferParameterToTextCtrl(const std::shared_ptr<ParameterFormula>,
			wxTextCtrl *ctrl, UnitType type);
	wxTextCtrl* GetTextCtrlByID(int id);

protected:
	void OnClose(wxCloseEvent &event) override;
	void OnMouseWheel(wxMouseEvent &event) override;
	void OnSize(wxSizeEvent &event) override;
	void OnTimer(wxTimerEvent &event);

	void RefreshCanvas(wxCommandEvent &event);
	void RefreshView(wxCommandEvent &event);
	void UpdateProject(wxCommandEvent &event);

	void OnCheckBox(wxCommandEvent &event) override;
	void OnChoice(wxCommandEvent &event) override;
	void OnFileChanged(wxFileDirPickerEvent &event) override;
	void OnKillFocus(wxFocusEvent &event) override;
	void OnListCtrlOnSelectionChanged(wxDataViewEvent &event) override;
	void OnPageChanged( wxNotebookEvent &event) override;
	void OnRadioButton(wxCommandEvent &event) override;
	void OnSetFocus(wxFocusEvent &event) override;
	void OnTextEnter(wxCommandEvent &event) override;

	void On3DSelect(wxMouseEvent &event) override;
	void OnBackgroundImagesSetup(wxCommandEvent &event) override;
	void OnConstructionChanged(wxCommandEvent &event) override;
	void OnMeasurementsCopy(wxCommandEvent &event) override;
	void OnMeasurementsQuickSetup(wxCommandEvent &event) override;
	void OnModelChanged(wxCommandEvent &event) override;
	void OnParserDebug(wxCommandEvent &event) override;
	void OnPatternSelect(wxTreeListEvent &event) override;
	void OnPatternSelectFabric(wxCommandEvent &event) override;
	void OnToggleStereo3D(wxCommandEvent &event) override;
	void OnViewChanged(wxCommandEvent &event) override;

	void OnButtonAdd(wxCommandEvent &event) override;
	void OnButtonCopy(wxCommandEvent &event) override;
	void OnButtonDelete(wxCommandEvent &event) override;
	void OnButtonTestStitch(wxCommandEvent &event) override;

	void OnObjectEdit(wxCommandEvent &event) override;
	void OnObjectLoad(wxCommandEvent &event) override;
	void OnObjectSave(wxCommandEvent &event) override;

};

#endif /* FRAMEMAIN_H */
