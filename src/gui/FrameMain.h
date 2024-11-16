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

public:
	bool TransferDataFromWindow();
	bool TransferDataToWindow();
	void TransferParameterToTextCtrl(const std::shared_ptr<Parameter>,
			wxTextCtrl *ctrl, UnitType type);
	wxTextCtrl* GetTextCtrlByID(int id);

	void RefreshCanvas(wxCommandEvent &event);
	void RefreshView(wxCommandEvent &event);
	void UpdateProject(wxCommandEvent &event);

	void OnClose(wxCloseEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnTimer(wxTimerEvent &event);
	void OnSetFocus(wxFocusEvent &event);
	void OnKillFocus(wxFocusEvent &event);
	void OnTextEnter(wxCommandEvent &event);
	void OnMouseWheel(wxMouseEvent &event);
	void OnScroll(wxScrollEvent &event);
	void OnChoice(wxCommandEvent &event);
	void OnToggleButton(wxCommandEvent &event);
	void OnCheckBox(wxCommandEvent &event);
	void OnListCtrlOnSelectionChanged(wxDataViewEvent &event);
	void OnPageChange( wxNotebookEvent &event);
	void OnViewChange(wxCommandEvent &event);

	void On3DSelect(wxMouseEvent &event);
	void OnToggleStereo3D(wxCommandEvent &event);
	void OnChangeModel(wxCommandEvent &event);
	void OnConstructionSelection(wxCommandEvent &event);
	void OnChoiceDisplay(wxCommandEvent &event);
	void OnFileChangedScanFile(wxFileDirPickerEvent &event);
	void OnFileChangedLastFile(wxFileDirPickerEvent &event);
	void OnSetSymmetry(wxCommandEvent &event);
	void OnCopyMeasurements(wxCommandEvent &event);

	void OnQuickSetupMeasurements(wxCommandEvent &event);
	void OnSetupBackgroundImages(wxCommandEvent &event);
	void OnDebugParser(wxCommandEvent &event);

	void OnToggleAnkleLock(wxCommandEvent &event);
	void OnAddBridge(wxCommandEvent &event);
	void OnDeleteBridge(wxCommandEvent &event);
	void OnEditBoneModel(wxCommandEvent &event);
	void OnEditShape(wxCommandEvent &event);
	void OnPatternSelect(wxTreeListEvent &event);
	void OnPatternAdd(wxCommandEvent &event);
	void OnPatternSelectFabric(wxCommandEvent &event);

	void OnLoadFootSTL(wxCommandEvent &event);
	void OnLoadPattern(wxCommandEvent &event);
	void OnLoadBoneModel(wxCommandEvent &event);
	void OnSaveBoneModel(wxCommandEvent &event);
	void OnSavePattern(wxCommandEvent &event);
	void OnSaveLast(wxCommandEvent &event);
	void OnSaveInsole(wxCommandEvent &event);
	void OnSaveSole(wxCommandEvent &event);
	void OnSaveCutaway(wxCommandEvent &event);
	void OnPackZip(wxCommandEvent &event);

};

#endif /* FRAMEMAIN_H */
